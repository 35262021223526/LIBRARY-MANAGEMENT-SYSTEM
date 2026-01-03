#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 50
#define MAX_MEMBERS 20
#define MAX_ISSUES 50

/* ---------------- BOOK STRUCTURE ---------------- */
struct Book {
    int bookID;
    char title[100];
    char author[50];
    char ISBN[20];
    char category[30];
    int totalCopies;
    int availableCopies;
    float price;
    char publisher[50];
    int publicationYear;
};

/* ---------------- MEMBER STRUCTURE ---------------- */
struct Member {
    int memberID;
    char name[50];
    char memberType[20]; // Student, Faculty, Public
    char phone[15];
    char email[50];
    char address[100];
    char joinDate[20];
    int booksIssued;
};

/* ---------------- ISSUE STRUCTURE ---------------- */
struct IssueRecord {
    int issueID;
    int bookID;
    int memberID;
    char issueDate[20];
    char dueDate[20];
    char returnDate[20];
    int isReturned;
    float fine;
};

/* ---------------- GLOBAL DATA ---------------- */
struct Book books[MAX_BOOKS];
struct Member members[MAX_MEMBERS];
struct IssueRecord issues[MAX_ISSUES];

int bookCount = 0;
int memberCount = 0;
int issueCount = 0;

/* ---------------- DATE DIFFERENCE FUNCTION ---------------- */
int dateDifference(char d1[], char d2[]) {
    struct tm date1 = {0}, date2 = {0};

    sscanf(d1, "%d-%d-%d", &date1.tm_mday, &date1.tm_mon, &date1.tm_year);
    sscanf(d2, "%d-%d-%d", &date2.tm_mday, &date2.tm_mon, &date2.tm_year);

    date1.tm_mon -= 1;
    date2.tm_mon -= 1;
    date1.tm_year -= 1900;
    date2.tm_year -= 1900;

    time_t t1 = mktime(&date1);
    time_t t2 = mktime(&date2);

    return (int)difftime(t2, t1) / (60 * 60 * 24);
}

/* ---------------- FINE CALCULATION ---------------- */
float calculateFine(int daysLate, char memberType[]) {
    float fine = 0;

    if (strcmp(memberType, "Faculty") == 0)
        return 0;

    if (daysLate <= 7)
        fine = daysLate * 5;
    else
        fine = (7 * 5) + ((daysLate - 7) * 10);

    if (fine > 500)
        fine = 500;

    return fine;
}

/* ---------------- ADD SAMPLE DATA ---------------- */
void loadSampleData() {
    books[0] = (struct Book){1, "C Programming", "Dennis Ritchie", "ISBN001", "Programming", 5, 5, 450, "PHI", 1988};
    books[1] = (struct Book){2, "Data Structures", "Mark Allen", "ISBN002", "CS", 4, 4, 500, "Pearson", 2015};
    bookCount = 2;

    members[0] = (struct Member){1, "Rahul Sharma", "Student", "9876543210", "rahul@gmail.com", "Delhi", "01-01-2024", 0};
    members[1] = (struct Member){2, "Dr. Mehta", "Faculty", "9123456780", "mehta@college.com", "Mumbai", "15-12-2023", 0};
    memberCount = 2;
}

/* ---------------- DISPLAY BOOKS ---------------- */
void displayBooks() {
    printf("\n--- Book List ---\n");
    for (int i = 0; i < bookCount; i++) {
        printf("ID:%d | %s | Author:%s | Available:%d\n",
               books[i].bookID, books[i].title,
               books[i].author, books[i].availableCopies);
    }
}

/* ---------------- DISPLAY MEMBERS ---------------- */
void displayMembers() {
    printf("\n--- Member List ---\n");
    for (int i = 0; i < memberCount; i++) {
        printf("ID:%d | %s | Type:%s | Issued:%d\n",
               members[i].memberID, members[i].name,
               members[i].memberType, members[i].booksIssued);
    }
}

/* ---------------- ISSUE BOOK ---------------- */
void issueBook() {
    int bookID, memberID;
    char issueDate[20], dueDate[20];

    printf("Enter Book ID: ");
    scanf("%d", &bookID);
    printf("Enter Member ID: ");
    scanf("%d", &memberID);

    int b = -1, m = -1;
    for (int i = 0; i < bookCount; i++)
        if (books[i].bookID == bookID) b = i;

    for (int i = 0; i < memberCount; i++)
        if (members[i].memberID == memberID) m = i;

    if (b == -1 || m == -1 || books[b].availableCopies == 0) {
        printf("Issue Failed!\n");
        return;
    }

    printf("Enter Issue Date (dd-mm-yyyy): ");
    scanf("%s", issueDate);
    printf("Enter Due Date (dd-mm-yyyy): ");
    scanf("%s", dueDate);

    issues[issueCount++] = (struct IssueRecord){
        issueCount, bookID, memberID,
        "", "", "", 0, 0
    };

    strcpy(issues[issueCount - 1].issueDate, issueDate);
    strcpy(issues[issueCount - 1].dueDate, dueDate);

    books[b].availableCopies--;
    members[m].booksIssued++;

    printf("Book Issued Successfully!\n");
}

/* ---------------- RETURN BOOK ---------------- */
void returnBook() {
    int issueID;
    char returnDate[20];

    printf("Enter Issue ID: ");
    scanf("%d", &issueID);
    printf("Enter Return Date (dd-mm-yyyy): ");
    scanf("%s", returnDate);

    for (int i = 0; i < issueCount; i++) {
        if (issues[i].issueID == issueID && issues[i].isReturned == 0) {
            strcpy(issues[i].returnDate, returnDate);

            int daysLate = dateDifference(issues[i].dueDate, returnDate);

            int mIndex = -1;
            for (int j = 0; j < memberCount; j++)
                if (members[j].memberID == issues[i].memberID)
                    mIndex = j;

            issues[i].fine = daysLate > 0 ? calculateFine(daysLate, members[mIndex].memberType) : 0;
            issues[i].isReturned = 1;

            for (int b = 0; b < bookCount; b++)
                if (books[b].bookID == issues[i].bookID)
                    books[b].availableCopies++;

            members[mIndex].booksIssued--;

            printf("Book Returned | Fine: ₹%.2f\n", issues[i].fine);
            return;
        }
    }
    printf("Invalid Issue ID\n");
}

/* ---------------- DISPLAY ISSUED BOOKS ---------------- */
void displayIssuedBooks() {
    printf("\n--- Issued Books ---\n");
    for (int i = 0; i < issueCount; i++) {
        printf("IssueID:%d BookID:%d MemberID:%d Returned:%d Fine:₹%.2f\n",
               issues[i].issueID, issues[i].bookID,
               issues[i].memberID, issues[i].isReturned, issues[i].fine);
    }
}

/* ---------------- MAIN MENU ---------------- */
int main() {
    int choice;
    loadSampleData();

    do {
        printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
        printf("1. Display Books\n");
        printf("2. Display Members\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Display Issued Books\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: displayBooks(); break;
        case 2: displayMembers(); break;
        case 3: issueBook(); break;
        case 4: returnBook(); break;
        case 5: displayIssuedBooks(); break;
        case 0: printf("Exiting...\n"); break;
        default: printf("Invalid choice\n");
        }
    } while (choice != 0);

    return 0;
}
