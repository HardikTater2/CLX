#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // For transform
#include <cctype>    // For tolower

using namespace std;

// Define data structures for users, books, and extra materials
struct User {
    string username;
    string password;
    string role; // "admin" or "user"
};

struct Book {
    int id;
    string title;
    string author;
    string subject;
    string fileType;
};

struct ExtraMaterial {
    int id;
    string title;
    string author;
    string type;
    string subject;
};

// Define node structures for linked lists
struct BookNode {
    Book data;
    BookNode* next;
    BookNode(const Book& book) : data(book), next(nullptr) {}
};

struct ExtraMaterialNode {
    ExtraMaterial data;
    ExtraMaterialNode* next;
    ExtraMaterialNode(const ExtraMaterial& material) : data(material), next(nullptr) {}
};

// Helper function to convert string to lowercase
string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
            [](unsigned char c) { return tolower(c); });
    return lowerStr;
}

// Define linked list classes
class BookList {
private:
    BookNode* head;

public:
    BookList() : head(nullptr) {}

    ~BookList() {
        BookNode* current = head;
        while (current) {
            BookNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addBook(const Book& book) {
        // Check for duplicate ID
        if (findBookById(book.id)) {
            cout << "Book with ID " << book.id << " already exists. Cannot add duplicate." << endl;
            return;
        }

        BookNode* newNode = new BookNode(book);
        if (!head) {
            head = newNode;
        } else {
            BookNode* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        cout << "Book uploaded successfully." << endl;
    }

    bool deleteBook(int id) {
        BookNode* current = head;
        BookNode* prev = nullptr;
        while (current) {
            if (current->data.id == id) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    head = current->next;
                }
                delete current;
                cout << "Book with ID " << id << " deleted successfully." << endl;
                return true;
            }
            prev = current;
            current = current->next;
        }
        cout << "Book with ID " << id << " not found." << endl;
        return false;
    }

    bool editBook(int id, const Book& updatedBook) {
        BookNode* current = head;
        while (current) {
            if (current->data.id == id) {
                current->data = updatedBook;
                cout << "Book with ID " << id << " updated successfully." << endl;
                return true;
            }
            current = current->next;
        }
        cout << "Book with ID " << id << " not found." << endl;
        return false;
    }

    Book* findBookById(int id) const {
        BookNode* current = head;
        while (current) {
            if (current->data.id == id) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    vector<Book> findBooksBySubject(const string& subject) const {
        vector<Book> foundBooks;
        BookNode* current = head;
        while (current) {
            if (toLower(current->data.subject) == toLower(subject)) {
                foundBooks.push_back(current->data);
            }
            current = current->next;
        }
        return foundBooks;
    }

    vector<Book> findBooksByTitleOrAuthor(const string& keyword) const {
        vector<Book> foundBooks;
        BookNode* current = head;
        string lowerKeyword = toLower(keyword);
        while (current) {
            string lowerTitle = toLower(current->data.title);
            string lowerAuthor = toLower(current->data.author);
            if (lowerTitle.find(lowerKeyword) != string::npos || lowerAuthor.find(lowerKeyword) != string::npos) {
                foundBooks.push_back(current->data);
            }
            current = current->next;
        }
        return foundBooks;
    }

    void displayBooks() const {
        BookNode* current = head;
        if (!current) {
            cout << "No books available." << endl;
            return;
        }
        cout << "\n--- List of Books ---\n";
        while (current) {
            const Book& book = current->data;
            cout << "ID: " << book.id 
                << ", Title: " << book.title 
                << ", Author: " << book.author 
                << ", Subject: " << book.subject 
                << ", FileType: " << book.fileType << endl;
            current = current->next;
        }
    }
};

// The ExtraMaterialList class would follow a similar pattern to the BookList

// Define fixed-size array for users
const int MAX_USERS = 100;
User users[MAX_USERS] = {
    {"admin", "password", "admin"},
    {"user1", "12345", "user"}
};
int userCount = 2;

// Function to authenticate user login
bool authenticateUser(const string& username, const string& password, string& role) {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username && users[i].password == password) {
            role = users[i].role;
            return true;
        }
    }
    return false;
}

// Function to register a new user
bool registerUser(const string& username, const string& password, const string& role = "user") {
    if (userCount >= MAX_USERS) {
        cout << "User limit reached. Cannot register more users." << endl;
        return false;
    }
    // Check for duplicate username
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username) {
            cout << "Username already exists. Choose a different username." << endl;
            return false;
        }
    }
    users[userCount++] = User{username, password, role};
    cout << "User registered successfully as " << role << "." << endl;
    return true;
}

// Function to handle invalid input and clear stream
void clearInputStream() {
    cin.clear(); // Clear the error flag set when input fails
    cin.ignore(10000, '\n'); // Ignore any remaining invalid input in the stream
}

// Main function
int main() {
    string username, password;
    string role;

    cout << "=== Welcome to the College Library Exchange ===\n";
    cout << "1. Login\n2. Register\nChoose an option: ";
    int initialChoice;
    cin >> initialChoice;
    if (cin.fail()) {
        clearInputStream();
        cout << "Invalid input. Please enter a number." << endl;
        return 0;
    }

    if (initialChoice == 2) {
        // Registration process
        cout << "\n--- User Registration ---\n";
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        if (!registerUser(username, password)) {
            return 0; // Registration failed
        }
    }

    cout << "\n=== Login ===\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (!authenticateUser(username, password, role)) {
        cout << "Login failed. Please try again." << endl;
        return 0;
    }

    cout << "Login successful! Role: " << role << endl;

    // Initialize linked lists with predefined data if first login
    static bool initialized = false;
    static BookList library;
    if (!initialized) {
        // Add predefined books
        library.addBook(Book{1, "Introduction to Algorithms", "Cormen et al.", "Computer Science", "PDF"});
        library.addBook(Book{2, "Pride and Prejudice", "Jane Austen", "Literature", "EPUB"});
        library.addBook(Book{3, "The Lord of the Rings", "J.R.R. Tolkien", "Fantasy", "PDF"});
        initialized = true;
    }

   // Main menu
while (true) {
    cout << "\n=== Main Menu ===\n";
    cout << "1. Upload Book\n"
        << "2. Delete Book\n"
        << "3. Edit Book\n"
        << "4. Find Books by Subject\n"
        << "5. Search Books by Title or Author\n"
        << "6. Display All Books\n";
    if (role == "admin") {
        cout << "7. Register New User\n";
    }
    cout << "0. Exit\n";
    cout << "Choose an option: ";

    int choice;
    cin >> choice;

    // User input actions
    if (choice == 1) {
        // Upload book
        int id;
        string title, author, subject, fileType;
        cout << "Enter book ID: ";
        cin >> id;
        cin.ignore(); // To handle newline character from cin
        cout << "Enter title: ";
        getline(cin, title);
        cout << "Enter author: ";
        getline(cin, author);
        cout << "Enter subject: ";
        getline(cin, subject);
        cout << "Enter file type (PDF, EPUB, etc.): ";
        getline(cin, fileType);
        library.addBook(Book{id, title, author, subject, fileType});
    }
    else if (choice == 2) {
        // Delete book
        int id;
        cout << "Enter book ID to delete: ";
        cin >> id;
        library.deleteBook(id);
    }
    else if (choice == 3) {
        // Edit book
        int id;
        cout << "Enter book ID to edit: ";
        cin >> id;
        cin.ignore(); // To handle newline character from cin
        string title, author, subject, fileType;
        cout << "Enter new title: ";
        getline(cin, title);
        cout << "Enter new author: ";
        getline(cin, author);
        cout << "Enter new subject: ";
        getline(cin, subject);
        cout << "Enter new file type: ";
        getline(cin, fileType);
        library.editBook(id, Book{id, title, author, subject, fileType});
    }
    else if (choice == 4) {
        // Find books by subject
        string subject;
        cout << "Enter subject to search: ";
        cin.ignore();
        getline(cin, subject);
        vector<Book> foundBooks = library.findBooksBySubject(subject);
        if (foundBooks.empty()) {
            cout << "No books found for the subject: " << subject << endl;
        } else {
            cout << "--- Books on '" << subject << "' ---" << endl;
            for (const Book& book : foundBooks) {
                cout << "ID: " << book.id << ", Title: " << book.title << ", Author: " << book.author << endl;
            }
        }
    }
    else if (choice == 5) {
        // Search books by title or author
        string keyword;
        cout << "Enter title or author keyword to search: ";
        cin.ignore();
        getline(cin, keyword);
        vector<Book> foundBooks = library.findBooksByTitleOrAuthor(keyword);
        if (foundBooks.empty()) {
            cout << "No books found matching the keyword: " << keyword << endl;
        } else {
            cout << "--- Search Results ---" << endl;
            for (const Book& book : foundBooks) {
                cout << "ID: " << book.id << ", Title: " << book.title << ", Author: " << book.author << endl;
            }
        }
    }
    else if (choice == 6) {
        // Display all books
        library.displayBooks();
    }
    else if (choice == 7 && role == "admin") {
        // Register new user (admin only)
        string newUsername, newPassword;
        cout << "Enter new username: ";
        cin >> newUsername;
        cout << "Enter new password: ";
        cin >> newPassword;
        if (!registerUser(newUsername, newPassword)) {
            cout << "Failed to register user." << endl;
        }
    }
    else if (choice == 0) {
        // Exit
        cout << "Exiting program." << endl;
        break;
    }
    else {
        cout << "Invalid option. Please try again." << endl;
    }

}