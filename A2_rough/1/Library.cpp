#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>
#include <cctype>

using namespace std;

class LibraryItem
{
public:
    string identity;
    string title;

    LibraryItem(const string &title)
    {
        this->title = title;
    }

    virtual void display() const
    {
        cout << "Title: " << title << "\n";
        cout << "Item FOUND !" << endl;
    }

    virtual string getType() const = 0;
    virtual string getIdentity() const = 0;
};

class Book : public LibraryItem
{
public:
    int book_id;
    int books_count;
    long int isbn;
    string authors;
    bool isElectronic;

    Book(int book_id, int books_count, long int isbn, const string &authors, const string &title, bool isElectronic = false)
        : LibraryItem(title), isElectronic(isElectronic)
    {
        this->book_id = book_id;
        this->books_count = books_count;
        this->isbn = isbn;
        this->authors = authors;
    }

    void display() const override
    {
        LibraryItem::display();
        cout << "Authors: " << authors << "\n";
    }

    string getType() const override
    {
        return "Book";
    }

    string getIdentity() const override
    {
        return "B" + string(1, title[0]);
    }
};

class Magazine : public LibraryItem
{
public:
    string rank;
    bool isElectronic;

    Magazine(const string &rank, const string &title, bool isElectronic = true)
        : LibraryItem(title), isElectronic(isElectronic)
    {
        this->rank = rank;
    }

    void display() const override
    {
        LibraryItem::display();
        cout << "Rank: " << rank << "\n";
    }

    string getType() const override
    {
        return "Magazine";
    }

    string getIdentity() const override
    {
        return "M" + string(1, title[0]);
    }
};

class Journal : public LibraryItem
{
public:
    string title;
    bool isElectronic;

    Journal(string &title, bool isElectronic = true)
        : LibraryItem(title), isElectronic(isElectronic)
    {
    }

    void display() const override
    {
        LibraryItem::display();
    }

    string getType() const override
    {
        return "Journal";
    }

    string getIdentity() const override
    {
        return "J" + string(1, title[0]);
    }
};

class User
{
public:
    string userName;
    string userType;
    struct BorrowedItem
    {
        string title;
        time_t issueDate;
        time_t returnDate;
    };
    BorrowedItem borrowedItems[6]; // Assuming maximum number of items to be borrowed is 6

    User(const string &userName, const string &userType)
    {
        this->userName = userName;
        this->userType = userType;
    }
    void registerUser()
    {
        cout << "Enter your name: ";
        getline(cin, userName);
        cout << "Enter your identity \n Faculty or Student: "<<endl;
        getline(cin, userType);
    }

    void borrow(LibraryItem *item)
    {
        if (item->getType() == "Book" && static_cast<Book *>(item)->books_count > 0)
        {
            time_t currentTime = time(nullptr);
            int i = 0;
            while (i < 4 && !borrowedItems[i].title.empty())
            {
                i++;
            }
            if (i < 6)
            {
                borrowedItems[i].title = item->title;
                borrowedItems[i].issueDate = currentTime; // Set issue date to the current time
                if (userType == "Student")
                {
                    borrowedItems[i].returnDate = currentTime + 30 * 24 * 60 * 60; // 30 days
                }
                else if (userType == "Faculty")
                {
                    borrowedItems[i].returnDate = currentTime + 6 * 30 * 24 * 60 * 60; // 6 months
                }
                static_cast<Book *>(item)->books_count--;
                static_cast<Book *>(item)->isbn++;
                cout << "Item successfully borrowed." << endl;
                cout << "Return Date: " << ctime(&borrowedItems[i].returnDate); // Display return date
            }
            else
            {
                cout << "You have borrowed the maximum number of items." << endl;
            }
        }

        else if (item->getType() == "Magazine")
        {
            time_t currentTime = time(nullptr);
            int i = 0;
            while (i < 4 && !borrowedItems[i].title.empty())
            {
                i++;
            }
            if (i < 4)
            {
                borrowedItems[i].title = item->title;
                borrowedItems[i].issueDate = currentTime;
                // No return date for magazines
                cout << "Item successfully borrowed." << endl;
            }
            else
            {
                cout << "You have borrowed the maximum number of items." << endl;
            }
        }
        else if (item->getType() == "Journal")
        {
            time_t currentTime = time(nullptr);
            int i = 0;
            while (i < 4 && !borrowedItems[i].title.empty())
            {
                i++;
            }
            if (i < 4)
            {
                borrowedItems[i].title = item->title;
                borrowedItems[i].issueDate = currentTime;
                // No return date for journals
                cout << "Item successfully borrowed." << endl;
            }
            else
            {
                cout << "You have borrowed the maximum number of items." << endl;
            }
        }
        else
        {
            cout << "Sorry, the item is not available for borrowing." << endl;
        }
    }
    
};
 bool compareItemsByTitle(const LibraryItem *a, const LibraryItem *b)
    {
        return a->title < b->title;
    }

int main()
{
    int Max_books = 99;
    int Max_magazines = 80;
    int Max_journals = 99; // change

    LibraryItem *items[Max_books + Max_magazines + Max_journals]; // change
    int itemCount = 0;

    ifstream book_file;
    book_file.open("D:\\IIITD\\OOPD\\Assignment2\\books.csv");
    string line;

    
    if (!book_file.is_open())
    {
        cerr << "Error: Unable to open the book file." << endl;
        return 1;
    }

    getline(book_file, line);

    while (getline(book_file, line) && itemCount < Max_books)
    {
        string substring;
        string substrings[22];
        bool in_quotes = false;
        int i = 0;
        string col_value;

        for (char c : line)
        {
            if (c == '"')
            {
                in_quotes = !in_quotes;
            }
            else if (c == ',' && !in_quotes)
            {
                substrings[i] = col_value;
                col_value.clear();
                i++;
            }
            else
            {
                col_value += c;
            }
        }

        try
        {
            int book_id = stoi(substrings[0]);
            int books_count = stoi(substrings[4]);
            int isbn = stoi(substrings[5]);
            string authors = substrings[7];
            string title = substrings[9];

            Book *book = new Book(book_id, books_count, isbn, authors, title, 0);
            items[itemCount++] = book;
        }
        catch (const std::invalid_argument &e)
        {
            cerr << "Error parsing CSV: " << e.what() << endl;
            // Handle the error or skip the line
        }
    }

    book_file.close();

    ifstream magazine_file;
    magazine_file.open("D:\\IIITD\\OOPD\\Assignment2\\magzine.csv");

    if (!magazine_file.is_open())
    {
        cerr << "Error: Unable to open the magazine file." << endl;
        return 1;
    }

    getline(magazine_file, line);

    while (getline(magazine_file, line) && itemCount < Max_books + Max_magazines)
    {
        string substring;
        string substrings[10];
        bool in_quotes = false;
        int i = 0;
        string col_value;

        for (char c : line)
        {
            if (c == '"')
            {
                in_quotes = !in_quotes;
            }
            else if (c == ',' && !in_quotes)
            {
                substrings[i] = col_value;
                col_value.clear();
                i++;
            }
            else
            {
                col_value += c;
            }
        }

        string title = substrings[0];
        string rank = substrings[1];

        // bool isElectronic = (substrings[9] == "1"); // Check if the magazine is electronic

        Magazine *magazine = new Magazine(rank, title, 1);
        items[itemCount++] = magazine;
    }

    magazine_file.close();

    ifstream journal_file;
    journal_file.open("D:\\IIITD\\OOPD\\Assignment2\\journals.csv");

    if (!journal_file.is_open())
    {
        cerr << "Error: Unable to open the magazine file." << endl;
        return 1;
    }

    while (getline(journal_file, line) && itemCount < Max_books + Max_magazines + Max_journals)
    {

        string title = line;
        Journal *journal = new Journal(title, 1);
        items[itemCount++] = journal;
    }

    journal_file.close();

    


    sort(items, items + itemCount, compareItemsByTitle);


    // Create a user and show borrowing of items from library
 
    User user("", "");

    user.registerUser();
    cout << "User registered successfully." << endl;
    cout << "User: " << user.userName << ", Type: " << user.userType << endl;

    // Prompt the user to search for an item
    bool foundMagazine = false;
    bool foundBook = false;
    bool foundJournal = false;

    while (true)
    {
        cout << "Welcome to IIITD Library"<< endl;
        cout << "What would you like to search for? " << endl;
        cout << "Press 1 for Magazine" << endl;
        cout << "Press 2 for Book" << endl;
        cout << "Prres 3 for Journal" << endl;
        cout << "Press 4 to Exit:" << endl;
        int searchChoice;
        cin >> searchChoice;
        cin.ignore(); // Ignore the newline character

        switch (searchChoice)
        {
        case 1:
            cout << "Enter the title of the magzine you want to search: ";
            getline(cin, line);

            foundMagazine = false;
            for (int i = 0; i < itemCount; i++)
            {
                if (items[i]->getType() == "Magazine" && items[i]->title == line)
                {
                    foundMagazine = true;
                    // cout <<"Search in "<<items[i]->getType() << " section, find it on shelf: " << items[i]->getIdentity() << endl;
                    items[i]->display();
                    cout << "Do you want to borrow this magazine? (y/n): ";
                    char choice;
                    cin >> choice;
                    cin.ignore(); // Ignore the newline character
                    if (choice == 'y' || choice == 'Y')
                    {
                        user.borrow(items[i]);
                    }
                    break;
                }
            }

            if (!foundMagazine)
            {
                cout << "No magazine found with the title: " << line << "\n";
            }
            break;

        case 2:
            cout << "Enter the title, author, or ISBN of the book you want to search: ";
            getline(cin, line);

            foundBook = false;
            for (int i = 0; i < itemCount; i++)
            {
                if (items[i]->getType() == "Book")
                {
                    Book *book = static_cast<Book *>(items[i]);

                    // Check if the input appears to be an ISBN (consists only of digits and has a valid length)
                    bool isISBN = true;
                    for (char c : line)
                    {
                        if (!isdigit(c))
                        {
                            isISBN = false;
                            break;
                        }
                    }

                    if (isISBN && line.length() == 13) // ISBN-13 has 13 digits
                    {
                        if (book->isbn == stol(line))
                        {
                            foundBook = true;
                            cout << "Search in " << items[i]->getType() << " section, find it on shelf: " << items[i]->getIdentity() << endl;
                            items[i]->display();
                            cout << "Do you want to borrow this book? (y/n): ";
                            char choice;
                            cin >> choice;
                            cin.ignore(); // Ignore the newline character
                            if (choice == 'y' || choice == 'Y')
                            {
                                user.borrow(items[i]);
                            }
                            break;
                        }
                    }
                    else if ((book->title == line || book->authors == line))
                    {
                        foundBook = true;
                        cout << "Search in " << items[i]->getType() << " section, find it on shelf: " << items[i]->getIdentity() << endl;
                        items[i]->display();
                        cout << "Do you want to borrow this book? (y/n): ";
                        char choice;
                        cin >> choice;
                        cin.ignore(); // Ignore the newline character
                        if (choice == 'y' || choice == 'Y')
                        {
                            user.borrow(items[i]);
                        }
                        break;
                    }
                }
            }

            if (!foundBook)
            {
                cout << "No book found with the title, author, or ISBN: " << line << "\n";
            }
            break;

        case 3:
            cout << "Enter the title of the journal you want to search: ";
            getline(cin, line);

            foundJournal = false;
            for (int i = 0; i < itemCount; i++)
            {
                if (items[i]->getType() == "Journal" && items[i]->title == line)
                {
                    foundJournal = true;
                    items[i]->display();
                    cout << "Do you want to borrow this journal? (y/n): ";
                    char choice;
                    cin >> choice;
                    cin.ignore(); // Ignore the newline character
                    if (choice == 'y' || choice == 'Y')
                    {
                        user.borrow(items[i]);
                    }
                    break;
                }
            }

            if (!foundJournal)
            {
                cout << "No journal found with the title: " << line << "\n";
            }
            break;
        case 4:
            cout << "Exiting the library.\n";
            return 0;

        default:
            cout << "Invalid choice. Please enter 1 for Magazine, 2 for Book, or 3 to Exit.\n";
            break;
        }
    }
}
