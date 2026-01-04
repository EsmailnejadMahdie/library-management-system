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

/* ============================
   Queue Functions 
============================ */
void initQueue() {
    queueSize = 0;
}

bool isQueueEmpty() {
    return queueSize == 0;
}

bool isQueueFull() {
    return queueSize == 100;
}

// Add to end of queue
void enqueue(int bookId, string borrowerName) {
    if (isQueueFull()) {
        cout << "Request queue is full!" << endl;
        return;
    }
    
    // Check for duplicate request
    for (int i = 0; i < queueSize; i++) {
        if (borrowQueue[i].bookId == bookId && borrowQueue[i].borrowerName == borrowerName) {
            cout << "You already have a pending request for this book!" << endl;
            return;
        }
    }
    
    borrowQueue[queueSize] = {bookId, borrowerName};
    queueSize++;
    cout << "Request added to waiting queue." << endl;
}
