#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <ctime>
#include <iomanip>

// Using the standard namespace to remove 'std::' prefixes
using namespace std;

// Utility function to get current date as string
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    sprintf(buffer, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(buffer);
}

// Transaction Structure
struct Transaction {
    char type; // 'D' for deposit, 'W' for withdrawal, 'T' for transfer, 'L' for loan
    double amount;
    string date;
    string description;

    Transaction(char t, double amt, const string& desc)
        : type(t), amount(amt), date(getCurrentDate()), description(desc) {}
};

// Base Account Class
class Account {
protected:
    string accountNumber;
    string accountHolder;
    double balance;

public:
    // Constructor
    Account(const string& accNum, const string& holder, double initialBalance)
        : accountNumber(accNum), accountHolder(holder), balance(initialBalance) {}

    // Getter methods
    string getAccountNumber() const { return accountNumber; }
    string getAccountHolder() const { return accountHolder; }
    double getBalance() const { return balance; }

    // Deposit method
    virtual void deposit(double amount) {
        if(amount <= 0) {
            throw invalid_argument("Deposit amount must be positive.");
        }
        balance += amount;
    }

    // Withdraw method (pure virtual)
    virtual void withdraw(double amount) = 0;

    // Display account details
    virtual void display() const {
        cout << "Account Number: " << accountNumber << "\n"
             << "Account Holder: " << accountHolder << "\n"
             << fixed << setprecision(2)
             << "Balance: $" << balance << "\n";
    }

    // Virtual destructor
    virtual ~Account() {}
};

// SavingsAccount Derived Class
class SavingsAccount : public Account {
private:
    double interestRate;
    vector<Transaction> transactions;

public:
    SavingsAccount(const string& accNum, const string& holder, double initialBalance, double rate)
        : Account(accNum, holder, initialBalance), interestRate(rate) {}

    // Apply interest
    void applyInterest() {
        double interest = balance * interestRate;
        balance += interest;
        transactions.emplace_back('D', interest, "Interest Applied");
    }

    // Override withdraw method
    void withdraw(double amount) override {
        if(amount <= 0) {
            throw invalid_argument("Withdrawal amount must be positive.");
        }
        if(amount > balance) {
            throw runtime_error("Insufficient funds.");
        }
        balance -= amount;
        transactions.emplace_back('W', amount, "Withdrawal");
    }

    // Override deposit method to record transactions
    void deposit(double amount) override {
        Account::deposit(amount);
        transactions.emplace_back('D', amount, "Deposit");
    }

    // Display account details
    void display() const override {
        Account::display();
        cout << "Account Type: Savings\n"
             << "Interest Rate: " << interestRate * 100 << "%\n";
    }

    // Display transaction history
    void displayHistory() const {
        cout << "Transaction History for Savings Account " << accountNumber << ":\n";
        for(const auto& txn : transactions) {
            string typeStr;
            switch(txn.type) {
                case 'D': typeStr = "Deposit"; break;
                case 'W': typeStr = "Withdrawal"; break;
                case 'T': typeStr = "Transfer"; break;
                case 'L': typeStr = "Loan"; break;
                default: typeStr = "Unknown"; break;
            }
            cout << txn.date << " - " << typeStr << " of $" << fixed << setprecision(2) << txn.amount 
                 << " - " << txn.description << "\n";
        }
    }
};

// CheckingAccount Derived Class
class CheckingAccount : public Account {
private:
    double overdraftLimit;
    vector<Transaction> transactions;

public:
    CheckingAccount(const string& accNum, const string& holder, double initialBalance, double overdraft)
        : Account(accNum, holder, initialBalance), overdraftLimit(overdraft) {}

    // Override withdraw method
    void withdraw(double amount) override {
        if(amount <= 0) {
            throw invalid_argument("Withdrawal amount must be positive.");
        }
        if(amount > balance + overdraftLimit) {
            throw runtime_error("Overdraft limit exceeded.");
        }
        balance -= amount;
        transactions.emplace_back('W', amount, "Withdrawal");
    }

    // Override deposit method to record transactions
    void deposit(double amount) override {
        Account::deposit(amount);
        transactions.emplace_back('D', amount, "Deposit");
    }

    // Display account details
    void display() const override {
        Account::display();
        cout << "Account Type: Checking\n"
             << "Overdraft Limit: $" << fixed << setprecision(2) << overdraftLimit << "\n";
    }

    // Display transaction history
    void displayHistory() const {
        cout << "Transaction History for Checking Account " << accountNumber << ":\n";
        for(const auto& txn : transactions) {
            string typeStr;
            switch(txn.type) {
                case 'D': typeStr = "Deposit"; break;
                case 'W': typeStr = "Withdrawal"; break;
                case 'T': typeStr = "Transfer"; break;
                case 'L': typeStr = "Loan"; break;
                default: typeStr = "Unknown"; break;
            }
            cout << txn.date << " - " << typeStr << " of $" << fixed << setprecision(2) << txn.amount 
                 << " - " << txn.description << "\n";
        }
    }
};

// LoanAccount Derived Class
class LoanAccount : public Account {
private:
    double loanAmount;
    double interestRate;
    double monthlyPayment;
    vector<Transaction> transactions;

public:
    LoanAccount(const string& accNum, const string& holder, double loanAmt, double rate)
        : Account(accNum, holder, 0.0), loanAmount(loanAmt), interestRate(rate) {}

    // Apply interest and calculate monthly payment
    void processMonthlyPayment() {
        double interest = loanAmount * interestRate;
        loanAmount += interest;
        monthlyPayment = loanAmount * 0.01; // Example: 1% monthly payment
        transactions.emplace_back('L', monthlyPayment, "Monthly Loan Payment");
        balance -= monthlyPayment;
    }

    // Override deposit to handle loan repayment
    void deposit(double amount) override {
        if(amount <= 0) {
            throw invalid_argument("Repayment amount must be positive.");
        }
        loanAmount -= amount;
        balance += amount;
        transactions.emplace_back('D', amount, "Loan Repayment");
    }

    // Override withdraw (not applicable for loans)
    void withdraw(double amount) override {
        throw runtime_error("Withdrawals are not allowed from a loan account.");
    }

    // Display account details
    void display() const override {
        cout << "Loan Account Number: " << accountNumber << "\n"
             << "Loan Holder: " << accountHolder << "\n"
             << fixed << setprecision(2)
             << "Loan Amount: $" << loanAmount << "\n"
             << "Interest Rate: " << interestRate * 100 << "%\n"
             << "Monthly Payment: $" << monthlyPayment << "\n";
    }

    // Display transaction history
    void displayHistory() const {
        cout << "Transaction History for Loan Account " << accountNumber << ":\n";
        for(const auto& txn : transactions) {
            string typeStr;
            switch(txn.type) {
                case 'D': typeStr = "Repayment"; break;
                case 'L': typeStr = "Loan Payment"; break;
                default: typeStr = "Unknown"; break;
            }
            cout << txn.date << " - " << typeStr << " of $" << fixed << setprecision(2) << txn.amount 
                 << " - " << txn.description << "\n";
        }
    }
};

// Customer Class
class Customer {
private:
    string username;
    string password;
    string name;
    string email;
    vector<shared_ptr<Account>> accounts;

public:
    Customer(const string& uname, const string& pwd, const string& nm, const string& mail)
        : username(uname), password(pwd), name(nm), email(mail) {}

    // Authenticate user
    bool authenticate(const string& uname, const string& pwd) const {
        return (uname == username && pwd == password);
    }

    // Add account
    void addAccount(shared_ptr<Account> account) {
        accounts.push_back(account);
    }

    // Get accounts
    vector<shared_ptr<Account>> getAccounts() const {
        return accounts;
    }

    // Display customer details
    void displayCustomer() const {
        cout << "Customer Name: " << name << "\n"
             << "Email: " << email << "\n"
             << "Username: " << username << "\n";
    }
};

// Bank Class
class Bank {
private:
    vector<shared_ptr<Customer>> customers;

public:
    // Add customer
    void addCustomer(shared_ptr<Customer> customer) {
        customers.push_back(customer);
    }

    // Authenticate customer
    shared_ptr<Customer> authenticateCustomer(const string& uname, const string& pwd) const {
        for(const auto& customer : customers) {
            if(customer->authenticate(uname, pwd)) {
                return customer;
            }
        }
        return nullptr;
    }

    // Find account by account number
    shared_ptr<Account> findAccount(const string& accNum) const {
        for(const auto& customer : customers) {
            for(const auto& account : customer->getAccounts()) {
                if(account->getAccountNumber() == accNum) {
                    return account;
                }
            }
        }
        return nullptr;
    }

    // Transfer funds between accounts
    void transferFunds(const string& fromAcc, const string& toAcc, double amount) {
        auto source = findAccount(fromAcc);
        auto destination = findAccount(toAcc);

        if(!source || !destination) {
            throw runtime_error("One or both accounts not found.");
        }

        // Attempt withdrawal from source
        source->withdraw(amount);
        // Deposit into destination
        destination->deposit(amount);

        // Record transfer transactions
        // Assuming Account class has a way to record transactions; otherwise, casting to derived classes
        // For simplicity, we will handle only Savings and Checking accounts here
        SavingsAccount* srcSav = dynamic_cast<SavingsAccount*>(source.get());
        if(srcSav) {
            srcSav->deposit(-amount); // Negative deposit to indicate transfer out
            srcSav->displayHistory(); // Optionally record transfer
        }

        CheckingAccount* srcChk = dynamic_cast<CheckingAccount*>(source.get());
        if(srcChk) {
            srcChk->deposit(-amount); // Negative deposit to indicate transfer out
            srcChk->displayHistory(); // Optionally record transfer
        }

        SavingsAccount* destSav = dynamic_cast<SavingsAccount*>(destination.get());
        if(destSav) {
            destSav->deposit(amount);
            destSav->displayHistory(); // Optionally record transfer
        }

        CheckingAccount* destChk = dynamic_cast<CheckingAccount*>(destination.get());
        if(destChk) {
            destChk->deposit(amount);
            destChk->displayHistory(); // Optionally record transfer
        }

        cout << "Transferred $" << fixed << setprecision(2) << amount 
             << " from " << fromAcc << " to " << toAcc << " successfully.\n";
    }

    // Display all customers
    void displayAllCustomers() const {
        for(const auto& customer : customers) {
            customer->displayCustomer();
            cout << "Accounts:\n";
            for(const auto& account : customer->getAccounts()) {
                account->display();
                cout << "\n";
            }
            cout << "-----------------------------\n";
        }
    }
};

// Function to process transactions
void processTransaction(shared_ptr<Account> account, char type, double amount, const string& description = "") {
    try {
        switch(type) {
            case 'D':
                account->deposit(amount);
                cout << "Deposited $" << fixed << setprecision(2) << amount << " successfully.\n";
                break;
            case 'W':
                account->withdraw(amount);
                cout << "Withdrew $" << fixed << setprecision(2) << amount << " successfully.\n";
                break;
            case 'T':
                // Transfer handled separately
                break;
            default:
                throw invalid_argument("Invalid transaction type.");
        }
    }
    catch(const exception& e) {
        cerr << "Transaction failed: " << e.what() << "\n";
    }
}

// Sample Main Function
int main() {
    Bank bank;

    // Create Customers
    auto customer1 = make_shared<Customer>("alice", "password123", "Alice Smith", "alice@example.com");
    auto customer2 = make_shared<Customer>("bob", "securepwd", "Bob Johnson", "bob@example.com");

    // Create Accounts for Customer 1
    auto savings1 = make_shared<SavingsAccount>("SA1001", "Alice Smith", 5000.0, 0.03);
    auto checking1 = make_shared<CheckingAccount>("CA1001", "Alice Smith", 2000.0, 500.0);
    customer1->addAccount(savings1);
    customer1->addAccount(checking1);

    // Create Accounts for Customer 2
    auto savings2 = make_shared<SavingsAccount>("SA2001", "Bob Johnson", 3000.0, 0.02);
    auto loan2 = make_shared<LoanAccount>("LA2001", "Bob Johnson", 10000.0, 0.05);
    customer2->addAccount(savings2);
    customer2->addAccount(loan2);

    // Add Customers to Bank
    bank.addCustomer(customer1);
    bank.addCustomer(customer2);

    // Display All Customers
    cout << "Welcome to SafeTransact Banking System!\n";
    cout << "--------------------------------------\n";
    bank.displayAllCustomers();

    // Simulate User Login
    string uname, pwd;
    cout << "Login to your account\n";
    cout << "Username: ";
    cin >> uname;
    cout << "Password: ";
    cin >> pwd;

    auto loggedInCustomer = bank.authenticateCustomer(uname, pwd);
    if(!loggedInCustomer) {
        cerr << "Authentication failed. Exiting.\n";
        return 1;
    }

    cout << "Welcome, " << loggedInCustomer->getAccounts().front()->getAccountHolder() << "!\n";

    // Simple Menu for Logged-in User
    int choice;
    do {
        cout << "\n--- Menu ---\n";
        cout << "1. View Accounts\n";
        cout << "2. Deposit Funds\n";
        cout << "3. Withdraw Funds\n";
        cout << "4. Transfer Funds\n";
        cout << "5. View Transaction History\n";
        cout << "6. Apply Interest (Savings Accounts)\n";
        cout << "7. Process Loan Payment (Loan Accounts)\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if(choice == 1) {
            // View Accounts
            cout << "\nYour Accounts:\n";
            for(const auto& acc : loggedInCustomer->getAccounts()) {
                acc->display();
                cout << "\n";
            }
        }
        else if(choice == 2) {
            // Deposit Funds
            string accNum;
            double amount;
            cout << "Enter Account Number to Deposit Into: ";
            cin >> accNum;
            cout << "Enter Amount to Deposit: ";
            cin >> amount;

            auto account = bank.findAccount(accNum);
            if(account) {
                processTransaction(account, 'D', amount);
            }
            else {
                cerr << "Account not found.\n";
            }
        }
        else if(choice == 3) {
            // Withdraw Funds
            string accNum;
            double amount;
            cout << "Enter Account Number to Withdraw From: ";
            cin >> accNum;
            cout << "Enter Amount to Withdraw: ";
            cin >> amount;

            auto account = bank.findAccount(accNum);
            if(account) {
                processTransaction(account, 'W', amount);
            }
            else {
                cerr << "Account not found.\n";
            }
        }
        else if(choice == 4) {
            // Transfer Funds
            string fromAcc, toAcc;
            double amount;
            cout << "Enter Source Account Number: ";
            cin >> fromAcc;
            cout << "Enter Destination Account Number: ";
            cin >> toAcc;
            cout << "Enter Amount to Transfer: ";
            cin >> amount;

            try {
                bank.transferFunds(fromAcc, toAcc, amount);
            }
            catch(const exception& e) {
                cerr << "Transfer failed: " << e.what() << "\n";
            }
        }
        else if(choice == 5) {
            // View Transaction History
            string accNum;
            cout << "Enter Account Number to View History: ";
            cin >> accNum;

            auto account = bank.findAccount(accNum);
            if(account) {
                // Dynamic casting to access transaction history
                if(auto sav = dynamic_cast<SavingsAccount*>(account.get())) {
                    sav->displayHistory();
                }
                else if(auto chk = dynamic_cast<CheckingAccount*>(account.get())) {
                    chk->displayHistory();
                }
                else if(auto loan = dynamic_cast<LoanAccount*>(account.get())) {
                    loan->displayHistory();
                }
                else {
                    cout << "No transaction history available for this account type.\n";
                }
            }
            else {
                cerr << "Account not found.\n";
            }
        }
        else if(choice == 6) {
            // Apply Interest for Savings Accounts
            for(const auto& acc : loggedInCustomer->getAccounts()) {
                if(auto sav = dynamic_cast<SavingsAccount*>(acc.get())) {
                    sav->applyInterest();
                    cout << "Interest applied to Savings Account " << sav->getAccountNumber() << ".\n";
                }
            }
        }
        else if(choice == 7) {
            // Process Loan Payment
            string accNum;
            double amount;
            cout << "Enter Loan Account Number: ";
            cin >> accNum;
            cout << "Enter Repayment Amount: ";
            cin >> amount;

            auto account = bank.findAccount(accNum);
            if(account) {
                try {
                    account->deposit(amount);
                    cout << "Loan repayment of $" << fixed << setprecision(2) << amount << " processed successfully.\n";
                }
                catch(const exception& e) {
                    cerr << "Loan repayment failed: " << e.what() << "\n";
                }
            }
            else {
                cerr << "Loan Account not found.\n";
            }
        }
        else if(choice == 8) {
            cout << "Thank you for using SafeTransact Banking. Goodbye!\n";
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }

    } while(choice != 8);

    return 0;
}
