// Console Based Banking Application
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

#define MIN_BALANCE 500

class Account
{
private:
    long accountNumber;
    string fname;
    string lname;
    float balance;
    static long NextAccountNumber;

public:
    Account() {}
    Account(string fname, string lname, float balance);

    long getAccountNumber() { return accountNumber; }
    string getFirstName() { return fname; }
    string getLastName() { return lname; }
    float getBalance() { return balance; }

    void Deposit(float amount);
    void Withdrawal(float amount);

    static void setNextAccountNumber(long accountNumber);
    static long getNextAccountNumber() { return NextAccountNumber; }

    friend ofstream &operator<<(ofstream &ofs, Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, Account &acc);
};

long Account::NextAccountNumber = 0;

class Bank
{
private:
    map<long, Account> accounts;

public:
    Bank();
    Account OpenAccount(string fname, string lname, float balance);
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdrawal(long accountNumber, float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();
    ~Bank();
};

int main()
{
    Bank B;
    Account acc;
    int accountNumber, balance, choice = 0;
    float amount;
    string fname, lname;
    cout << "\t\t\t\t\t Banking System \n";
    do
    {
        cout << "************************************************************************************************";
        cout << "\n\t Select One option from below:";
        cout << "\n\t\t\t\t\t1. Open Account";
        cout << "\n\t\t\t\t\t2. Balance Enquiry";
        cout << "\n\t\t\t\t\t3. Deposit";
        cout << "\n\t\t\t\t\t4. Withdrawal";
        cout << "\n\t\t\t\t\t5. Close Account";
        cout << "\n\t\t\t\t\t6. Show All Accounts";
        cout << "\n\t\t\t\t\t7. Quit";
        cout << "\n\n\t Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "\n\t Enter First Name: ";
            cin >> fname;
            cout << "\t Enter Last Name: ";
            cin >> lname;
            cout << "\t Enter Initial Balance: ";
            cin >> balance;
            acc = B.OpenAccount(fname, lname, balance);
            cout << "\n\t Congratulations, Account is created";
            cout << acc;
            break;
        case 2:
            cout << "\n\t Enter Account Number: ";
            cin >> accountNumber;
            acc = B.BalanceEnquiry(accountNumber);
            cout << acc;
            break;
        case 3:
            cout << "\n\t Enter Account Number: ";
            cin >> accountNumber;
            cout << "\t Enter Amount to Deposit: ";
            cin >> amount;
            acc = B.Deposit(accountNumber, amount);
            cout << acc;
            break;
        case 4:
            cout << "\n\t Enter Account Number: ";
            cin >> accountNumber;
            cout << "\t Enter Amount to Withdraw: ";
            cin >> amount;
            try
            {
                acc = B.Withdrawal(accountNumber, amount);
                cout << acc;
            }
            catch (const char *message)
            {
                cout << "\n\t Error: " << message << endl;
            }
            break;
        case 5:
            cout << "\n\t Enter Account Number: ";
            cin >> accountNumber;
            cout << "\n\t Account Deleted!";
            B.CloseAccount(accountNumber);
            break;
        case 6:
            B.ShowAllAccounts();
            break;
        case 7:
            break;
        default:
            cout << "\n\t Enter Correct choice !";
            break;
        }

    } while (choice != 7);
    return 0;
}

Account::Account(string fname, string lname, float balance)
{
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
    this->fname = fname;
    this->lname = lname;
    this->balance = balance;
}

void Account::Deposit(float amount)
{
    balance += amount;
}

void Account::Withdrawal(float amount)
{
    if (balance - amount < MIN_BALANCE)
        throw "Insufficient balance";
    balance -= amount;
}

void Account::setNextAccountNumber(long accountNumber)
{
    NextAccountNumber = accountNumber;
}

ofstream &operator<<(ofstream &ofs, Account &acc)
{
    ofs << acc.accountNumber << endl;
    ofs << acc.fname << endl;
    ofs << acc.lname << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream &operator>>(ifstream &ifs, Account &acc)
{
    ifs >> acc.accountNumber;
    ifs >> acc.fname;
    ifs >> acc.lname;
    ifs >> acc.balance;
    return ifs;
}

ostream &operator<<(ostream &os, Account &acc)
{
    os << "\n\t Account Number: " << acc.getAccountNumber();
    os << "\n\t First Name: " << acc.getFirstName();
    os << "\n\t Last Name: " << acc.getLastName();
    os << "\n\t Balance: " << acc.getBalance();
    os << endl;
}

Bank::Bank()
{
    Account acc;

    ifstream ifile;
    ifile.open("Accounts.txt");

    if (!ifile)
        return;

    while (!ifile.eof())
    {
        ifile >> acc;
        accounts.insert(pair<long, Account>(acc.getAccountNumber(), acc));
    }
    Account::setNextAccountNumber(acc.getAccountNumber());

    ifile.close();
}

Account Bank::OpenAccount(string fname, string lname, float balance)
{
    Account acc(fname, lname, balance);
    accounts.insert(pair<long, Account>(acc.getAccountNumber(), acc));
    return acc;
}

Account Bank::BalanceEnquiry(long accountNumber)
{
    auto itr = accounts.find(accountNumber);
    return itr->second;
}

Account Bank::Deposit(long accountNumber, float amount)
{
    auto itr = accounts.find(accountNumber);
    itr->second.Deposit(amount);
    return itr->second;
}

Account Bank::Withdrawal(long accountNumber, float amount)
{
    auto itr = accounts.find(accountNumber);
    itr->second.Withdrawal(amount);
    return itr->second;
}
void Bank::CloseAccount(long accountNumber)
{
    auto itr = accounts.find(accountNumber);
    cout << itr->second;
    accounts.erase(accountNumber);
}
void Bank::ShowAllAccounts()
{
    for (auto &itr : accounts)
    {
        cout << itr.second;
    }
}

Bank::~Bank()
{
    ofstream file("Accounts.txt", ios::trunc);
    for (auto &itr : accounts)
    {
        file << itr.second;
    }
    file.close();
}