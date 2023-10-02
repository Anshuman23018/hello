#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>
#include <cctype>
#include <iomanip>

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
    virtual Library *copy() const = 0;
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

    string getIdentity() const override
    {
        return "M" + string(1, title[0]);
    }
    string getType() const override
    {
        return "Magazine";
    }

    Library *copy() const override
    {
        return new Magazine(rank, title, isElectronic);
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

    int getBooksCount()
    {
        return books_count;
    }

    Library *copy() const override
    {
        return new Book(book_id, books_count, isbn, authors, title, isElectronic);
    }
};

class Journal : public Library
{
public:
    string title;
    bool isElectronic;

        Journal(const string &title, bool isElectronic = true)
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
    Library *copy() const override
    {
        return new Journal(title, isElectronic);
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
    string borrowedLocations[6];
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

    void borrowOnLoan(Library *item, int libraryChoice)
    {
        if (libraryChoice >= 1 && libraryChoice <= 3)
        {
            // Borrow the item on loan
            int delay = 7; // 7 days delay
            time_t currentTime = time(nullptr);
            currentTime = currentTime + delay * 24 * 60 * 60;

            int i = 0;
            while (i < 4 && !borrowedItems[i].title.empty())
            {
                i++;
            }

            if (i < 6)
            {

                borrowedItems[i].title = item->title;
                borrowedItems[i].issueDate = currentTime;

                if (userType == "Student")
                {
                    borrowedItems[i].returnDate = currentTime + 30 * 24 * 60 * 60; // 30 days
                }
                else if (userType == "Faculty")
                {
                    borrowedItems[i].returnDate = currentTime + 6 * 30 * 24 * 60 * 60; // 6 months
                }

                // Record the location of borrowing
                borrowedLocations[i] = "Library " + to_string(libraryChoice);

                cout << "Item successfully borrowed on loan from Library " << libraryChoice << "." << endl;
                cout << "Return Date: " << ctime(&borrowedItems[i].returnDate) << endl; // Display return date
            }
            else
            {
                cout << "You have borrowed the maximum number of items." << endl;
            }
        }
        else
        {
            cout << "Invalid library choice." << endl;
        }
    }

    void borrow(Library *item)
    {
        if (item->getType() == "Book" && static_cast<Book *>(item)->books_count > 0)
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
                borrowedItems[i].issueDate = currentTime; // Set issue date to the current time
                if (userType == "Student")
                {
                    borrowedItems[i].returnDate = currentTime + 30 * 24 * 60 * 60; // 30 days
                }
                else if (userType == "Faculty")
                {
                    borrowedItems[i].returnDate = currentTime + 6 * 30 * 24 * 60 * 60; // 6 months
                }
                static_cast<Book *>(item)->books_count++;
                cout << "Item successfully borrowed." << endl;

                // Get the day of the month with proper suffix
                int day = localtime(&borrowedItems[i].returnDate)->tm_mday;
                std::string daySuffix;
                if ((day >= 11 && day <= 13) || (day % 10 == 0) || (day % 10 >= 4 && day % 10 <= 9))
                {
                    daySuffix = "th";
                }
                else if (day % 10 == 1)
                {
                    daySuffix = "st";
                }
                else if (day % 10 == 2)
                {
                    daySuffix = "nd";
                }
                else if (day % 10 == 3)
                {
                    daySuffix = "rd";
                }

                // Format the return date
                struct tm *timeInfo = localtime(&borrowedItems[i].returnDate);
                char buffer[80];
                strftime(buffer, 80, "%d %B %Y Time: %I:%M:%S %p", timeInfo);
                std::string formattedDate(buffer);

                cout << "Return Date: " << formattedDate << endl;
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
            while (i < 6 && !borrowedItems[i].title.empty())
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

class TieUpLibrary
{
private:
    Library *items[278];
    int itemCount;

public:
    // Constructor that makes copy of LinrayItems(to dipect the functionality)
    TieUpLibrary(Library *items[], int itemCount)
    {
        for (int i = 0; i < itemCount; i++)
        {
            this->items[i] = items[i];
        }
        this->itemCount = itemCount;
    }

    Library *getItemByTitle(const string &title)
    {
        for (int i = 0; i < itemCount; i++)
        {
            if (items[i]->title == title)
            {
                return items[i];
            }
        }
        return nullptr; // Item not found
    }

    bool searchItem(const string &title)
    {
        for (int i = 0; i < itemCount; i++)
        {
            if (items[i]->title == title)
            {
                return true;
            }
        }
        return false; // Item not found
    }
};

int main()
{
    int Max_books = 99;
    int Max_magazines = 80;
    int Max_journals = 99;
    bool foundInOtherLibraries = false;
    Library *items[Max_books + Max_magazines + Max_journals]; // change
    int itemCount = 0;

    // Call the function to read books from a CSV file
    readBooksFromFile(items, itemCount, Max_books, "D:\\IIITD\\OOPD\\Assignment_2\\books.csv");

    // Call the function to read magazines from a CSV file
    readMagazinesFromFile(items, itemCount, Max_books + Max_magazines, "D:\\IIITD\\OOPD\\Assignment_2\\magazine.csv");

    // Call the function to read journals from a CSV file
    readJournalsFromFile(items, itemCount, Max_books + Max_magazines + Max_journals, "D:\\IIITD\\OOPD\\Assignment_2\\journals.csv");

    sort(items, items + itemCount, compareItemsByTitle);

    TieUpLibrary tieUpLibrary1(items, itemCount);
    TieUpLibrary tieUpLibrary2(items, itemCount); // populating TieUpLibrary
    TieUpLibrary tieUpLibrary3(items, itemCount);
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
        cout << "Press 5 to Borrow a Book on loan from Other University Libraries" << endl;
        cout << "Press 6 to exit " << endl;
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
        {
            cout << "Enter the title of the item you want to search in tie-up libraries: ";
            getline(cin, line);

            // Search for the item in tie-up libraries
            bool foundInTieUpLibrary1 = tieUpLibrary1.searchItem(line);
            bool foundInTieUpLibrary2 = tieUpLibrary2.searchItem(line);
            bool foundInTieUpLibrary3 = tieUpLibrary3.searchItem(line);

            if (foundInTieUpLibrary1 || foundInTieUpLibrary2 || foundInTieUpLibrary3)
            {
                cout << "Item found in the following librarie(s):" << endl;
                if (foundInTieUpLibrary1)
                {
                    cout << "IIT Delhi Library" << endl;
                }
                if (foundInTieUpLibrary2)
                {
                    cout << "DTU Library" << endl;
                }
                if (foundInTieUpLibrary3)
                {
                    cout << "NSUIT Library" << endl;
                }

                cout << "From which library do you want to borrow? ";
                cout<<"Press 1 for IIT Delhi"<<endl;
                cout<<"Press 2 for DTU"<<endl;
                cout<<"Press 3 for NSUIT"<<endl;
                int libraryChoice;
                cin >> libraryChoice;
                cin.ignore(); // Ignore the newline character

                if (libraryChoice >= 1 && libraryChoice <= 3)
                {
                    cout << "Do you want to borrow this item from Library " << libraryChoice << "? (y/n): ";
                    string borrowChoice;
                    getline(cin, borrowChoice);
                    if (borrowChoice == "y")
                    {
                        user.borrowOnLoan((libraryChoice == 1) ? tieUpLibrary1.getItemByTitle(line) : (libraryChoice == 2) ? tieUpLibrary2.getItemByTitle(line)
                                                                                                                           : tieUpLibrary3.getItemByTitle(line),
                                          libraryChoice);
                    }
                    else
                    {
                        cout << "Borrowing is  canceled." << endl;
                    }
                }
                else
                {
                    cout << "Invalid library choice." << endl;
                }
            }
            else
            {
                cout << "Item not found in any nearby libraries." << endl;
            }
            break;
        }

        case 6:
            cout << "Thankyou for Visiting Us" << endl;
            return 0;

        default:
            cout << "Invalid choice. Please enter 1 for Magazine, 2 for Book, 3 for Journal or press 4 to Exit" << endl;
            break;
        }
    }
}
