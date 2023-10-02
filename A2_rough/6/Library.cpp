#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>
#include <cctype>

using namespace std;

class Library
{
public:
    string identity;
    string title;

    Library(const string &title)
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

class Magazine : public Library
{
public:
    string rank;
    bool isElectronic;

    Magazine(const string &rank, const string &title, bool isElectronic = true)
        : Library(title), isElectronic(isElectronic)
    {
        this->rank = rank;
    }

    void display() const override
    {
        Library::display();
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

class Book : public Library
{
public:
    int book_id;
    int books_count;
    long int isbn;
    string authors;
    bool isElectronic;

    Book(int book_id, int books_count, long int isbn, const string &authors, const string &title, bool isElectronic = false)
        : Library(title), isElectronic(isElectronic)
    {
        this->book_id = book_id;
        this->books_count = books_count;
        this->isbn = isbn;
        this->authors = authors;
    }

    void display() const override
    {
        Library::display();
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

class Journal : public Library
{
public:
    string title;
    bool isElectronic;

    Journal(string &title, bool isElectronic = true)
        : Library(title), isElectronic(isElectronic)
    {
    }

    void display() const override
    {
        Library::display();
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
    BorrowedItem borrowedItems[6]; // Let the  maximum number of items to be borrowed be 6

    User(const string &userName, const string &userType)
    {
        this->userName = userName;
        this->userType = userType;
    }
    void registerUser()
    {
        cout << "Enter your identity \n Faculty or Student: " << endl;
        getline(cin, userType);
        if (userType == "Student")
        {
            cout << "Hello student" << endl;
        }
        else if (userType == "Faculty")
        {
            cout << "Hello Faculty" << endl;
        }
        else
        {
            cout << "Invalid identity" << endl;
        }
        cout << "PLease Enter your name: ";
        getline(cin, userName);
    }

    void borrow(Library *item)
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
            while (i < 6 && !borrowedItems[i].title.empty())
            {
                i++;
            }
            if (i < 6)
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
            if (i < 6)
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
            cout << "Sorry, the item is not available." << endl;
        }
    }
};
bool compareItemsByTitle(const Library *a, const Library *b)
{
    return a->title < b->title;
}

// function to read magazine from file
int readMagazinesFromFile(Library *items[], int &itemCount, int maxItems, const string &filename)
{
    ifstream magazine_file;
    magazine_file.open(filename);
    string line;

    if (!magazine_file.is_open())
    {
        cerr << "Error: Unable to open the magazine file." << endl;
        return -1;
    }

    getline(magazine_file, line);

    while (getline(magazine_file, line) && itemCount < maxItems)
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
    return itemCount;
}

// Function to read books from a CSV file and add them to the items array
int readBooksFromFile(Library *items[], int &itemCount, int maxItems, const string &filename)
{
    ifstream book_file;
    book_file.open(filename);
    string line;

    if (!book_file.is_open())
    {
        cerr << "Error: Unable to open the book file." << endl;
        return -1;
    }

    getline(book_file, line);

    while (getline(book_file, line) && itemCount < maxItems)
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
    return itemCount;
}

// Function to read journals from a CSV file and add them to the items array
int readJournalsFromFile(Library *items[], int &itemCount, int maxItems, const string &filename)
{
    ifstream journal_file;
    journal_file.open(filename);
    string line;

    if (!journal_file.is_open())
    {
        cerr << "Error: Unable to open the journal file." << endl;
        return -1;
    }

    while (getline(journal_file, line) && itemCount < maxItems)
    {
        string title = line;
        Journal *journal = new Journal(title, 1);
        items[itemCount++] = journal;
    }

    journal_file.close();
    return itemCount;
}

int main()
{
    int Max_books = 99;
    int Max_magazines = 80;
    int Max_journals = 99;

    Library *items[Max_books + Max_magazines + Max_journals]; // change
    int itemCount = 0;

    // Call the function to read books from a CSV file
    readBooksFromFile(items, itemCount, Max_books, "D:\\IIITD\\OOPD\\Assignment_2\\books.csv");

    // Call the function to read magazines from a CSV file
    readMagazinesFromFile(items, itemCount, Max_books + Max_magazines, "D:\\IIITD\\OOPD\\Assignment_2\\magazine.csv");

    // Call the function to read journals from a CSV file
    readJournalsFromFile(items, itemCount, Max_books + Max_magazines + Max_journals, "D:\\IIITD\\OOPD\\Assignment_2\\journals.csv");

    sort(items, items + itemCount, compareItemsByTitle);

    // Prompt the user to search for an item
    bool foundMagazine = false;
    bool foundBook = false;
    bool foundJournal = false;

    // Creating a user and to show borrowing of items from library

    User user("", "");

    user.registerUser();
    cout << "User registered successfully." << endl;
    cout << "User Name : " << user.userName << ", Type: " << user.userType << endl;

    while (true)
    {
        string line;
        const char *boldText = "\033[1m";
        // ANSI escape code to reset text formatting
        const char *resetText = "\033[0m";
        std::cout << boldText << "Welcome to IIITD Library" << resetText << std::endl;
        cout << "What would you like to search for? " << endl;
        cout << "Press 1 for Book" << endl;
        cout << "Press 2 for Magazine" << endl;
        cout << "Prres 3 for Journal" << endl;
        cout << "Press 4 to purchase a new Book:" << endl;
        cout << "Press 5 to exit " << endl;
        int searchChoice;
        cin >> searchChoice;
        cin.ignore(); // Ignore the newline character

        switch (searchChoice)
        {

        case 1:
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
                            cout << "Would you like to borrow this book?" << endl;
                            cout << "Press y/n ?" << endl;
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
                cout << "No book found with the title, author, or ISBN: " << line << endl;
            }
            break;

        case 2:
            cout << "Enter the title of the magazine you want to search: ";
            getline(cin, line);

            foundMagazine = false;
            for (int i = 0; i < itemCount; i++)
            {
                if (items[i]->getType() == "Magazine")
                {
                    Magazine *magazine = static_cast<Magazine *>(items[i]);

                    // Convert both the entered title and stored titles to lowercase
                    string enteredTitle = line;
                    transform(enteredTitle.begin(), enteredTitle.end(), enteredTitle.begin(), ::tolower);
                    string storedTitle = magazine->title; // Use magazine->title here
                    transform(storedTitle.begin(), storedTitle.end(), storedTitle.begin(), ::tolower);

                    if (enteredTitle == storedTitle)
                    {
                        foundMagazine = true;
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
            }

            if (!foundMagazine)
            {
                cout << "No magazine found with the title: " << line << endl;
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
                    cout << "Would you like to borrow this journal? (y/n): " << endl;
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
                cout << "No journal found " << line << endl;
            }
            break;
        case 4:
            cout << "Enter the title, author, or ISBN of the book you want to Purchase: ";
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

                    if ((isISBN && line.length() == 13 && to_string(book->isbn) == line) || book->title == line || book->authors == line)
                    {
                        foundBook = true;
                        items[i]->display();
                        cout << "Do you want to purchase this book?  press (y/n): " << endl;
                        char choice;
                        cin >> choice;
                        cin.ignore(); // Ignore the newline character
                        if (choice == 'y' || choice == 'Y')
                        {
                            book->books_count++;
                            cout << "Book purchased successfully. Updated books count: " << book->books_count << endl;
                        }
                        break;
                    }
                }
            }

            if (!foundBook)
            {
                cout << "No book found with the title, author, or ISBN: " << line << endl;
            }
            break;

        case 5:
            cout << "Thankyou for Visiting Us" << endl;
            return 0;

        default:
            cout << "Invalid choice. Please enter 1 for Magazine, 2 for Book, 3 for Journal or press 4 to Exit" << endl;
            break;
        }
    }
}
