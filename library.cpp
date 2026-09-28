#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

// Class representing a Book
class Book {
public:
    int id;
    string title;
    string author;
    bool isIssued;

    Book() : id(0), title(""), author(""), isIssued(false) {}

    Book(int id, string title, string author, bool isIssued = false) {
        this->id = id;
        this->title = title;
        this->author = author;
        this->isIssued = isIssued;
    }

    void displayBook() const {
        cout << left << setw(10) << id 
             << setw(30) << title 
             << setw(25) << author 
             << setw(15) << (isIssued ? "Issued" : "Available") << endl;
    }
};

// Class representing a Member
class Member {
public:
    int memberId;
    string name;
    int issuedBookId; // 0 if no book issued

    Member() : memberId(0), name(""), issuedBookId(0) {}

    Member(int id, string name, int bookId = 0) {
        this->memberId = id;
        this->name = name;
        this->issuedBookId = bookId;
    }

    void displayMember() const {
        cout << left << setw(12) << memberId 
             << setw(30) << name 
             << setw(15) << (issuedBookId == 0 ? "None" : to_string(issuedBookId)) << endl;
    }
};

// Core Library Management System Class
class LibrarySystem {
private:
    vector<Book> books;
    vector<Member> members;

    void loadBooks() {
        books.clear();
        ifstream file("books.txt");
        if (!file) return;

        int id;
        string title, author;
        bool isIssued;

        while (file >> id) {
            file.ignore();
            getline(file, title);
            getline(file, author);
            file >> isIssued;
            books.push_back(Book(id, title, author, isIssued));
        }
        file.close();
    }

    void saveBooks() {
        ofstream file("books.txt");
        for (const auto &b : books) {
            file << b.id << "\n" << b.title << "\n" << b.author << "\n" << b.isIssued << "\n";
        }
        file.close();
    }

    void loadMembers() {
        members.clear();
        ifstream file("members.txt");
        if (!file) return;

        int id, bookId;
        string name;

        while (file >> id) {
            file.ignore();
            getline(file, name);
            file >> bookId;
            members.push_back(Member(id, name, bookId));
        }
        file.close();
    }

    void saveMembers() {
        ofstream file("members.txt");
        for (const auto &m : members) {
            file << m.memberId << "\n" << m.name << "\n" << m.issuedBookId << "\n";
        }
        file.close();
    }

    string toLower(string str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

public:
    LibrarySystem() {
        loadBooks();
        loadMembers();
    }

    void addBook() {
        int id;
        string title, author;

        cout << "\nEnter Book ID: ";
        cin >> id;

        for (const auto &b : books) {
            if (b.id == id) {
                cout << "Error: Book ID already exists!\n";
                return;
            }
        }

        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);

        books.push_back(Book(id, title, author, false));
        saveBooks();
        cout << "\nBook added successfully!\n";
    }

    void addMember() {
        int id;
        string name;

        cout << "\nEnter Member ID: ";
        cin >> id;

        for (const auto &m : members) {
            if (m.memberId == id) {
                cout << "Error: Member ID already exists!\n";
                return;
            }
        }

        cin.ignore();
        cout << "Enter Member Name: ";
        getline(cin, name);

        members.push_back(Member(id, name, 0));
        saveMembers();
        cout << "\nMember added successfully!\n";
    }

    void displayAllBooks() {
        if (books.empty()) {
            cout << "\nNo books found in library.\n";
            return;
        }

        cout << "\n=========================================================================\n";
        cout << left << setw(10) << "Book ID" << setw(30) << "Title" << setw(25) << "Author" << setw(15) << "Status" << endl;
        cout << "=========================================================================\n";
        for (const auto &b : books) {
            b.displayBook();
        }
        cout << "=========================================================================\n";
    }

    void displayAllMembers() {
        if (members.empty()) {
            cout << "\nNo registered members found.\n";
            return;
        }

        cout << "\n=========================================================\n";
        cout << left << setw(12) << "Member ID" << setw(30) << "Name" << setw(15) << "Issued Book ID" << endl;
        cout << "=========================================================\n";
        for (const auto &m : members) {
            m.displayMember();
        }
        cout << "=========================================================\n";
    }

    void searchBook() {
        if (books.empty()) {
            cout << "\nNo books available to search.\n";
            return;
        }

        int option;
        cout << "\nSearch By:\n1. Title\n2. Author\nSelect Choice (1-2): ";
        cin >> option;
        cin.ignore();

        string query;
        bool found = false;

        if (option == 1) {
            cout << "Enter Book Title to Search: ";
            getline(cin, query);
            query = toLower(query);

            cout << "\n=========================================================================\n";
            cout << left << setw(10) << "Book ID" << setw(30) << "Title" << setw(25) << "Author" << setw(15) << "Status" << endl;
            cout << "=========================================================================\n";

            for (const auto &b : books) {
                if (toLower(b.title).find(query) != string::npos) {
                    b.displayBook();
                    found = true;
                }
            }
        } else if (option == 2) {
            cout << "Enter Author Name to Search: ";
            getline(cin, query);
            query = toLower(query);

            cout << "\n=========================================================================\n";
            cout << left << setw(10) << "Book ID" << setw(30) << "Title" << setw(25) << "Author" << setw(15) << "Status" << endl;
            cout << "=========================================================================\n";

            for (const auto &b : books) {
                if (toLower(b.author).find(query) != string::npos) {
                    b.displayBook();
                    found = true;
                }
            }
        } else {
            cout << "Invalid search choice!\n";
            return;
        }

        if (!found) {
            cout << "No matching books found.\n";
        }
        cout << "=========================================================================\n";
    }

    void issueBook() {
        int bookId, memberId;
        cout << "\nEnter Book ID to Issue: ";
        cin >> bookId;
        cout << "Enter Member ID: ";
        cin >> memberId;

        Book* bookPtr = nullptr;
        Member* memberPtr = nullptr;

        for (auto &b : books) {
            if (b.id == bookId) {
                bookPtr = &b;
                break;
            }
        }

        for (auto &m : members) {
            if (m.memberId == memberId) {
                memberPtr = &m;
                break;
            }
        }

        if (!bookPtr) {
            cout << "Error: Book not found!\n";
            return;
        }
        if (!memberPtr) {
            cout << "Error: Member not found!\n";
            return;
        }
        if (bookPtr->isIssued) {
            cout << "Error: Book is already issued!\n";
            return;
        }
        if (memberPtr->issuedBookId != 0) {
            cout << "Error: Member already has an issued book!\n";
            return;
        }

        bookPtr->isIssued = true;
        memberPtr->issuedBookId = bookId;

        saveBooks();
        saveMembers();
        cout << "\nBook successfully issued to " << memberPtr->name << "!\n";
    }

    void returnBook() {
        int bookId;
        cout << "\nEnter Book ID to Return: ";
        cin >> bookId;

        Book* bookPtr = nullptr;
        Member* memberPtr = nullptr;

        for (auto &b : books) {
            if (b.id == bookId) {
                bookPtr = &b;
                break;
            }
        }

        if (!bookPtr) {
            cout << "Error: Book not found!\n";
            return;
        }
        if (!bookPtr->isIssued) {
            cout << "Error: Book was not issued!\n";
            return;
        }

        for (auto &m : members) {
            if (m.issuedBookId == bookId) {
                memberPtr = &m;
                break;
            }
        }

        bookPtr->isIssued = false;
        if (memberPtr) {
            memberPtr->issuedBookId = 0;
        }

        saveBooks();
        saveMembers();
        cout << "\nBook successfully returned!\n";
    }
};

int main() {
    LibrarySystem lib;
    int choice;

    while (true) {
        cout << "\n==========================================\n";
        cout << "        LIBRARY MANAGEMENT SYSTEM         \n";
        cout << "==========================================\n";
        cout << "1. Add New Book\n";
        cout << "2. Add New Member\n";
        cout << "3. Display All Books\n";
        cout << "4. Display All Members\n";
        cout << "5. Search Book by Title or Author\n";
        cout << "6. Issue Book\n";
        cout << "7. Return Book\n";
        cout << "8. Exit\n";
        cout << "Select Option (1-8): ";
        cin >> choice;

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.addMember(); break;
            case 3: lib.displayAllBooks(); break;
            case 4: lib.displayAllMembers(); break;
            case 5: lib.searchBook(); break;
            case 6: lib.issueBook(); break;
            case 7: lib.returnBook(); break;
            case 8:
                cout << "\nExiting Library System. Goodbye!\n";
                return 0;
            default:
                cout << "\nInvalid Choice! Please try again.\n";
        }
    }
}