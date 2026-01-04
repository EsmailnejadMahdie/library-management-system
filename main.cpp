#include <iostream>
#include <string>
using namespace std;

/* ============================
   Book Structure
============================ */
struct Book {
    int id;
    string title;
    string author;
    int year;
    bool available;
    string borrower;
};

/* ============================
   Linked List Node (for books)
============================ */
struct BookNode {
    Book data;
    BookNode* next;
};

/* ============================
   Borrow Queue 
============================ */
struct BorrowRequest {
    int bookId;
    string borrowerName;
};

BorrowRequest borrowQueue[100];  
int queueSize = 0;               // Number of elements in queue

/* ============================
   Undo Stack 
============================ */
struct Action {
    int type; // 1=Add, 2=Delete
    Book snapshot;
};

Action undoStack[5];
int stackTop = -1;

/* ============================
   Global Variables
============================ */
BookNode* head = nullptr;
