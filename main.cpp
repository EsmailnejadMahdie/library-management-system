#include <iostream>
#include <string>
#include <iomanip>
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
            // Shift all elements after i one position left
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
        // Shift all elements left (remove oldest)
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

void displayHistory() {
    if (stackTop == -1) {
        cout << "No history available." << endl;
        return;
    }
    
    cout << "\n=== Operation History (Last 5) ===" << endl;
    for (int i = stackTop; i >= 0; i--) {
        string opType = (undoStack[i].type == 1) ? "Add Book" : "Delete Book";
        cout << "Operation: " << opType 
             << " | Book: " << undoStack[i].snapshot.title
             << " (ID: " << undoStack[i].snapshot.id << ")" << endl;
    }
    cout << "==================================" << endl;
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

bool borrowerHasBook(int bookId, string borrowerName) {
    BookNode* book = findBook(bookId);
    return (book && !book->data.available && book->data.borrower == borrowerName);
}

bool hasPendingRequest(int bookId, string borrowerName) {
    for (int i = 0; i < queueSize; i++) {
        if (borrowQueue[i].bookId == bookId && borrowQueue[i].borrowerName == borrowerName) {
            return true;
        }
    }
    return false;
}

/* ============================
   Core Library Functions
============================ */
void addBook() {
    Book b;
    cout << "Enter Book ID: ";
    cin >> b.id;

    if (findBook(b.id)) {
        cout << "Error: Book with this ID already exists!" << endl;
        return;
    }

    cin.ignore();
    cout << "Book Title: ";
    getline(cin, b.title);
    cout << "Author: ";
    getline(cin, b.author);
    cout << "Publication Year: ";
    cin >> b.year;

    b.available = true;
    b.borrower = "";

    BookNode* node = new BookNode{b, nullptr};

    // Add to end of list
    if (!head) {
        head = node;
    } else {
        BookNode* curr = head;
        while (curr->next)
            curr = curr->next;
        curr->next = node;
    }

    pushAction({1, b});
    cout << "Book added successfully!" << endl;
}

void deleteBook() {
    int id;
    cout << "Enter Book ID to delete: ";
    cin >> id;

    BookNode* curr = head;
    BookNode* prev = nullptr;

    while (curr && curr->data.id != id) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        cout << "Book not found!" << endl;
        return;
    }
    
    if (!curr->data.available) {
        cout << "Error: Cannot delete a borrowed book!" << endl;
        return;
    }

    // Remove all pending requests for this book
    removeAllRequestsForBook(id);
    
    // Save for undo
    pushAction({2, curr->data});

    // Remove from list
    if (!prev)
        head = curr->next;
    else
        prev->next = curr->next;

    delete curr;
    cout << "Book and all its pending requests deleted successfully!" << endl;
}

void searchBook() {
    int id;
    cout << "Enter Book ID to search: ";
    cin >> id;

    BookNode* book = findBook(id);
    if (!book) {
        cout << "Book not found!" << endl;
        return;
    }

    cout << "\n=== Book Details ===" << endl;
    cout << "ID: " << book->data.id << endl;
    cout << "Title: " << book->data.title << endl;
    cout << "Author: " << book->data.author << endl;
    cout << "Year: " << book->data.year << endl;
    cout << "Status: " << (book->data.available ? "Available" : string("Borrowed by ") + book->data.borrower) << endl;
    cout << "===================" << endl;
}

void sortBooks(int mode) {
    if (!head || !head->next) {
        cout << "Not enough books to sort." << endl;
        return;
    }

    bool swapped;
    do {
        swapped = false;
        BookNode* curr = head;
        while (curr->next) {
            bool shouldSwap = false;
            
            if (mode == 1) { // Sort by ID
                shouldSwap = (curr->data.id > curr->next->data.id);
            } else { // Sort by Year
                shouldSwap = (curr->data.year > curr->next->data.year);
            }
            
            if (shouldSwap) {
                Book temp = curr->data;
                curr->data = curr->next->data;
                curr->next->data = temp;
                swapped = true;
            }
            curr = curr->next;
        }
    } while (swapped);

    cout << "Books sorted successfully by " 
         << (mode == 1 ? "ID" : "Publication Year") << "!" << endl;
}

void borrowBook() {
    int id;
    string borrowerName;
    
    cout << "Enter Book ID to borrow: ";
    cin >> id;
    cin.ignore();
    cout << "Borrower Name: ";
    getline(cin, borrowerName);

    BookNode* book = findBook(id);
    if (!book) {
        cout << "Book not found!" << endl;
        return;
    }

    // Check if borrower already has this book
    if (borrowerHasBook(id, borrowerName)) {
        cout << "You already have this book borrowed!" << endl;
        return;
    }

    // Check for duplicate pending request
    if (hasPendingRequest(id, borrowerName)) {
        cout << "You already have a pending request for this book!" << endl;
        return;
    }

    if (book->data.available) {
        book->data.available = false;
        book->data.borrower = borrowerName;
        cout << "Book successfully borrowed by " << borrowerName << "!" << endl;
    } else {
        enqueue(id, borrowerName);
    }
}

void returnBook() {
    int id;
    
    cout << "Enter Book ID to return: ";
    cin >> id;
    

    BookNode* book = findBook(id);
    if (!book) {
        cout << "Book not found!" << endl;
        return;
    }

    if (book->data.available) {
        cout << "This book is already available!" << endl;
        return;
    }

    cout << "Book \"" << book->data.title << "\" returned." << endl;
    
    // Check for pending requests
    string nextRequester = getFirstRequester(id);
    
    if (nextRequester != "") {
        // Give book to next requester
        book->data.borrower = nextRequester;
        cout << "Book automatically borrowed by " << nextRequester << " (next in queue)." << endl;
        
        // Remove this request from queue
        removeFirstRequestForBook(id);
    } else {
        // No pending requests - make book available
        book->data.available = true;
        book->data.borrower = "";
    }
}

void undo() {
    Action act;
    if (!popAction(act)) {
        cout << "No actions to undo!" << endl;
        return;
    }

    string message;
    
    if (act.type == 1) { // Undo Add (delete the book)
        BookNode* curr = head;
        BookNode* prev = nullptr;
        while (curr && curr->data.id != act.snapshot.id) {
            prev = curr;
            curr = curr->next;
        }
        
        if (curr) {
            // Also remove pending requests
            removeAllRequestsForBook(curr->data.id);
            
            if (!prev) head = curr->next;
            else prev->next = curr->next;
            
            delete curr;
            message = "Add operation undone. Book removed.";
        }
    }
    else if (act.type == 2) { // Undo Delete (restore the book)
        if (findBook(act.snapshot.id)) {
            cout << "Cannot undo: A book with ID " << act.snapshot.id 
                 << " already exists!" << endl;
            return;
        } else {
            BookNode* node = new BookNode{act.snapshot, head};
            head = node;
            message = "Delete operation undone. Book restored.";
        }
    }

    cout << "SUCCESS: " << message << endl;
}

void showBooks() {
    BookNode* curr = head;
    if (!curr) {
        cout << "Library is empty!" << endl;
        return;
    }

    cout << "\n=== All Books in Library ===\n";
    cout << left
         << setw(5)  << "ID"
         << setw(27) << "Title"
         << setw(15) << "Author"
         << setw(8)  << "Year"
         << "Status\n";

    cout << string(70, '-') << endl;

    while (curr) {
        cout << left
             << setw(5)  << curr->data.id
             << setw(27) << curr->data.title
             << setw(15) << curr->data.author
             << setw(8)  << curr->data.year;

        if (curr->data.available) {
            cout << "Available";
        } else {
            cout << "Borrowed by " << curr->data.borrower;
        }
        cout << endl;

        curr = curr->next;
    }

    cout << string(70, '-') << endl;

}

void showQueue() {
    if (isQueueEmpty()) {
        cout << "No pending borrow requests." << endl;
        return;
    }
    
    cout << "\n=== Pending Borrow Requests ===" << endl;
    for (int i = 0; i < queueSize; i++) {
        BookNode* book = findBook(borrowQueue[i].bookId);
        string bookTitle = book ? book->data.title : "Book not found";
        
        cout << i+1 << ". Book: " << bookTitle
             << " (ID: " << borrowQueue[i].bookId << ")"
             << " - Requester: " << borrowQueue[i].borrowerName << endl;
    }
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
        cout << "1. Add Book" << endl;
        cout << "2. Delete Book" << endl;
        cout << "3. Search Book" << endl;
        cout << "4. Sort by ID" << endl;
        cout << "5. Sort by Year" << endl;
        cout << "6. Borrow Book" << endl;
        cout << "7. Return Book" << endl;
        cout << "8. Show All Books" << endl;
        cout << "9. Undo Last Operation" << endl;
        cout << "10. Show Operation History" << endl;
        cout << "11. Show Pending Requests" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: deleteBook(); break;
            case 3: searchBook(); break;
            case 4: sortBooks(1); break;
            case 5: sortBooks(2); break;
            case 6: borrowBook(); break;
            case 7: returnBook(); break;
            case 8: showBooks(); break;
            case 9: undo(); break;
            case 10: displayHistory(); break;
            case 11: showQueue(); break;
            case 0: cout << "Goodbye!" << endl; break;
            default: cout << "Invalid choice!" << endl;
        }
    } while (choice != 0);

    // Cleanup memory
    BookNode* curr = head;
    while (curr) {
        BookNode* temp = curr;
        curr = curr->next;
        delete temp;
    }

    return 0;
}