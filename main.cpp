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

// Remove FIRST request for a specific book
bool removeFirstRequestForBook(int bookId) {
    for (int i = 0; i < queueSize; i++) {
        if (borrowQueue[i].bookId == bookId) {
            for (int j = i; j < queueSize - 1; j++) {
                borrowQueue[j] = borrowQueue[j + 1];
            }
            queueSize--;
            return true;
        }
    }
    return false;
}

// Remove ALL requests for a specific book
void removeAllRequestsForBook(int bookId) {
    int writeIndex = 0;
    
    for (int readIndex = 0; readIndex < queueSize; readIndex++) {
        if (borrowQueue[readIndex].bookId != bookId) {
            borrowQueue[writeIndex] = borrowQueue[readIndex];
            writeIndex++;
        }
    }
    
    queueSize = writeIndex;
}

// Get first requester for a book
string getFirstRequester(int bookId) {
    for (int i = 0; i < queueSize; i++) {
        if (borrowQueue[i].bookId == bookId) {
            return borrowQueue[i].borrowerName;
        }
    }
    return "";
}

/* ============================
   Stack Functions
============================ */
void initStack() {
    stackTop = -1;
}

void pushAction(Action act) {
    if (stackTop == 4) {
        for (int i = 0; i < 4; i++) {
            undoStack[i] = undoStack[i + 1];
        }
        stackTop = 3;
    } else {
        stackTop++;
    }
    undoStack[stackTop] = act;
}

bool popAction(Action& act) {
    if (stackTop == -1)
        return false;
    act = undoStack[stackTop];
    stackTop--;
    return true;
}

/* ============================
   Book List Functions
============================ */
BookNode* findBook(int id) {
    BookNode* curr = head;
    while (curr) {
        if (curr->data.id == id)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

/* ============================
   Main Function
============================ */
int main() {
    initQueue();
    initStack();

    int choice;
    do {
        cout << "\n=== LIBRARY MANAGEMENT SYSTEM ===" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice != 0)
            cout << "Feature under development." << endl;

    } while (choice != 0);

    return 0;
}
