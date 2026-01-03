#include <stdio.h>
#include <string.h>

/* ================= STRUCTURES ================= */

struct Book {
    int bookID;
    char title[100];
    char author[50];
    char ISBN[20];
    char category[30];
    int totalCopies;
    int availableCopies;
};

struct Member {
    int memberID;
    char name[50];
    char memberType[20]; // Student / Faculty / Public
    int booksIssued;
};

struct IssueRecord {
    int issueID;
    int bookID;
    int memberID;
    int daysLate;
    int isReturned;
    float fine;
};

/* ================= ARRAYS ================= */

struct Book books[50];
struct Member members[20];
struct IssueRecord issues[50];

int bookCount = 0, memberCount = 0, issueCount = 0;

/* ================= FUNCTIONS ================= */

int memberLimit(char type[]) {
    if(strcmp(type,"Student")==0) return 3;
    if(strcmp(type,"Faculty")==0) return 5;
    return 2;
}

float calculateFine(int daysLate, char type[]) {
    float fine = 0;
    if(strcmp(type,"Faculty")==0) return 0;

    if(daysLate <= 7)
        fine = daysLate * 5;
    else
        fine = (7 * 5) + (daysLate - 7) * 10;

    if(fine > 500) fine = 500;
    return fine;
}

/* ================= BOOK FUNCTIONS ================= */

void addBook() {
    struct Book b;
    printf("\nBook ID: ");
    scanf("%d",&b.bookID);
    getchar();

    printf("Title: ");
    gets(b.title);
    printf("Author: ");
    gets(b.author);
    printf("ISBN: ");
    gets(b.ISBN);
    printf("Category: ");
    gets(b.category);
    printf("Total Copies: ");
    scanf("%d",&b.totalCopies);

    b.availableCopies = b.totalCopies;
    books[bookCount++] = b;

    printf("✅ Book Added\n");
}

void displayBooks() {
    printf("\n--- BOOK LIST ---\n");
    for(int i=0;i<bookCount;i++) {
        printf("ID:%d | %s | %s | Available:%d\n",
               books[i].bookID,
               books[i].title,
               books[i].author,
               books[i].availableCopies);
    }
}

int findBook(int id) {
    for(int i=0;i<bookCount;i++)
        if(books[i].bookID == id)
            return i;
    return -1;
}

/* ================= MEMBER FUNCTIONS ================= */

void addMember() {
    struct Member m;
    printf("\nMember ID: ");
    scanf("%d",&m.memberID);
    getchar();

    printf("Name: ");
    gets(m.name);
    printf("Type (Student/Faculty/Public): ");
    gets(m.memberType);

    m.booksIssued = 0;
    members[memberCount++] = m;

    printf("✅ Member Registered\n");
}

int findMember(int id) {
    for(int i=0;i<memberCount;i++)
        if(members[i].memberID == id)
            return i;
    return -1;
}

/* ================= ISSUE FUNCTIONS ================= */

void issueBook() {
    int bid, mid;
    printf("\nBook ID: ");
    scanf("%d",&bid);
    printf("Member ID: ");
    scanf("%d",&mid);

    int b = findBook(bid);
    int m = findMember(mid);

    if(b==-1 || m==-1) {
        printf("❌ Invalid ID\n");
        return;
    }

    if(books[b].availableCopies == 0) {
        printf("❌ Book Not Available\n");
        return;
    }

    if(members[m].booksIssued >= memberLimit(members[m].memberType)) {
        printf("❌ Issue Limit Reached\n");
        return;
    }

    issues[issueCount].issueID = issueCount + 1;
    issues[issueCount].bookID = bid;
    issues[issueCount].memberID = mid;
    issues[issueCount].isReturned = 0;
    issues[issueCount].fine = 0;

    books[b].availableCopies--;
    members[m].booksIssued++;
    issueCount++;

    printf("✅ Book Issued\n");
}

void returnBook() {
    int id, days;
    printf("\nIssue ID: ");
    scanf("%d",&id);
    printf("Days Late: ");
    scanf("%d",&days);

    struct IssueRecord *ir = &issues[id-1];
    int m = findMember(ir->memberID);
    int b = findBook(ir->bookID);

    ir->fine = calculateFine(days, members[m].memberType);
    ir->isReturned = 1;

    books[b].availableCopies++;
    members[m].booksIssued--;

    printf("✅ Returned | Fine ₹%.2f\n", ir->fine);
}

void showIssuedBooks() {
    printf("\n--- ISSUED BOOKS ---\n");
    for(int i=0;i<issueCount;i++) {
        if(!issues[i].isReturned)
            printf("IssueID:%d BookID:%d MemberID:%d\n",
                   issues[i].issueID,
                   issues[i].bookID,
                   issues[i].memberID);
    }
}

/* ================= MAIN ================= */

int main() {
    int choice;

    while(1) {
        printf("\n===== LIBRARY MANAGEMENT SYSTEM =====");
        printf("\n1. Add Book");
        printf("\n2. Display Books");
        printf("\n3. Add Member");
        printf("\n4. Issue Book");
        printf("\n5. Return Book");
        printf("\n6. View Issued Books");
        printf("\n0. Exit");
        printf("\nChoice: ");
        scanf("%d",&choice);

        switch(choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: addMember(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: showIssuedBooks(); break;
            case 0: return 0;
            default: printf("❌ Invalid Choice");
        }
    }
}
