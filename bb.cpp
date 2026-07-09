#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>
#include <limits>
#include <memory>

using namespace std;

// ==================== INPUT HELPERS ====================
// Reading numbers with cin >> x and no validation is a common source of
// infinite loops (a bad token puts cin into a fail state forever).
// These helpers clear/discard bad input so the menu can never hang.
template <typename T>
T readNumber(const string& prompt) {
    T value;
    cout << prompt;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

// ==================== TRANSACTION ====================
class Transaction {
private:
    string type;
    double amount;
    string description;
    time_t timestamp;

public:
    Transaction() : type(""), amount(0), description(""), timestamp(time(nullptr)) {}
    Transaction(string t, double a, string desc = "")
        : type(std::move(t)), amount(a), description(std::move(desc)), timestamp(time(nullptr)) {}

    void display() const {
        cout << "  Type: " << type << endl;
        cout << "  Amount: $" << fixed << setprecision(2) << amount << endl;
        if (!description.empty()) cout << "  Note: " << description << endl;
        cout << "  ------------------------" << endl;
    }
};

// ==================== ACCOUNT (base) ====================
class Account {
protected:
    int accountNumber;
    string holderName;
    double balance;
    string accountType;
    vector<Transaction> transactions;   // was a fixed-size array of 50; silently dropped
                                         // transactions past that limit with no warning.
    bool isActive;
    static int lastAccountNumber;

public:
    Account() : accountNumber(0), holderName(""), balance(0),
                accountType(""), isActive(true) {}

    Account(string name, double initialBalance, string type)
        : accountNumber(++lastAccountNumber), holderName(std::move(name)),
          balance(initialBalance), accountType(std::move(type)), isActive(true) {}

    virtual ~Account() = default;

    int getAccountNumber() const { return accountNumber; }
    string getHolderName() const { return holderName; }
    double getBalance() const { return balance; }
    string getAccountType() const { return accountType; }
    bool getStatus() const { return isActive; }
    void setHolderName(const string& name) { holderName = name; }
    void setStatus(bool status) { isActive = status; }

    virtual void deposit(double amount) {
        if (!isActive) {
            cout << "[ERROR] Account is inactive!" << endl;
            return;
        }
        if (amount <= 0) {
            cout << "[ERROR] Invalid deposit amount!" << endl;
            return;
        }
        balance += amount;
        transactions.emplace_back("DEPOSIT", amount);
        cout << "[OK] Deposited $" << fixed << setprecision(2) << amount << " successfully!" << endl;
    }

    virtual bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "[ERROR] Invalid withdrawal amount!" << endl;
            return false;
        }
        if (!isActive) {
            cout << "[ERROR] Account is inactive!" << endl;
            return false;
        }
        if (amount > balance) {
            cout << "[ERROR] Insufficient balance!" << endl;
            return false;
        }
        balance -= amount;
        transactions.emplace_back("WITHDRAWAL", amount);
        cout << "[OK] Withdrawn $" << fixed << setprecision(2) << amount << " successfully!" << endl;
        return true;
    }

    // Used by applyInterest(); does NOT modify balance itself, it only
    // reports what the interest for this period would be.
    virtual double calculateInterest() = 0;

    virtual void displayInfo() const {
        cout << "\n========================================" << endl;
        cout << "ACCOUNT DETAILS" << endl;
        cout << "========================================" << endl;
        cout << "Account No: " << accountNumber << endl;
        cout << "Holder Name: " << holderName << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        cout << "========================================\n" << endl;
    }

    void showTransactions() const {
        if (transactions.empty()) {
            cout << "[INFO] No transactions found!" << endl;
            return;
        }
        cout << "\nTRANSACTION HISTORY" << endl;
        cout << "========================================" << endl;
        for (const auto& t : transactions) {
            t.display();
        }
        cout << "========================================\n" << endl;
    }

    static int generateAccountNumber() { return ++lastAccountNumber; }
};

int Account::lastAccountNumber = 1000;

// ==================== SAVINGS ACCOUNT ====================
class SavingsAccount : public Account {
private:
    double interestRate;
    double minimumBalance;

public:
    SavingsAccount(string name, double initialBalance, double rate = 0.04, double minBal = 500)
        : Account(std::move(name), initialBalance, "Savings"),
          interestRate(rate), minimumBalance(minBal) {}

    double calculateInterest() override {
        return balance * interestRate;
    }

    bool withdraw(double amount) override {
        if (amount <= 0) {
            cout << "[ERROR] Invalid withdrawal amount!" << endl;
            return false;
        }
        if (balance - amount < minimumBalance) {
            cout << "[ERROR] Minimum balance of $" << fixed << setprecision(2)
                 << minimumBalance << " required!" << endl;
            return false;
        }
        return Account::withdraw(amount);
    }

    void displayInfo() const override {
        Account::displayInfo();
        cout << "Interest Rate: " << (interestRate * 100) << "%" << endl;
        cout << "Minimum Balance: $" << fixed << setprecision(2) << minimumBalance << endl;
    }
};

// ==================== CURRENT ACCOUNT ====================
class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(string name, double initialBalance, double overdraft = 5000)
        : Account(std::move(name), initialBalance, "Current"), overdraftLimit(overdraft) {}

    double calculateInterest() override { return 0; }

    bool withdraw(double amount) override {
        if (amount <= 0) {
            cout << "[ERROR] Invalid withdrawal amount!" << endl;
            return false;
        }
        if (!isActive) {
            cout << "[ERROR] Account is inactive!" << endl;
            return false;
        }
        if (amount > balance + overdraftLimit) {
            cout << "[ERROR] Exceeds overdraft limit of $" << fixed << setprecision(2)
                 << overdraftLimit << endl;
            return false;
        }
        balance -= amount;
        transactions.emplace_back("WITHDRAWAL", amount);
        cout << "[OK] Withdrawn $" << fixed << setprecision(2) << amount << " successfully!" << endl;
        return true;
    }

    void displayInfo() const override {
        Account::displayInfo();
        cout << "Overdraft Limit: $" << fixed << setprecision(2) << overdraftLimit << endl;
    }
};

// ==================== CUSTOMER ====================
class Customer {
private:
    int customerId;
    string name;
    string address;
    string phone;
    string email;
    vector<int> accountNumbers;   // was a fixed-size array of 10; silently dropped
                                   // account links beyond that with no warning.
    bool isActive;
    static int lastCustomerId;

public:
    Customer() : customerId(0), isActive(true) {}

    Customer(string n, string addr, string ph, string em)
        : customerId(++lastCustomerId), name(std::move(n)), address(std::move(addr)),
          phone(std::move(ph)), email(std::move(em)), isActive(true) {}

    int getId() const { return customerId; }
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    bool getStatus() const { return isActive; }

    void setName(const string& n) { name = n; }
    void setAddress(const string& a) { address = a; }
    void setPhone(const string& p) { phone = p; }
    void setEmail(const string& e) { email = e; }
    void setStatus(bool status) { isActive = status; }

    void addAccount(int accNo) {
        accountNumbers.push_back(accNo);
    }

    void displayInfo() const {
        cout << "\n========================================" << endl;
        cout << "CUSTOMER DETAILS" << endl;
        cout << "========================================" << endl;
        cout << "Customer ID: " << customerId << endl;
        cout << "Name: " << name << endl;
        cout << "Address: " << address << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
        cout << "Accounts: ";
        if (accountNumbers.empty()) {
            cout << "None";
        } else {
            for (int accNo : accountNumbers) {
                cout << accNo << " ";
            }
        }
        cout << endl;
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        cout << "========================================\n" << endl;
    }

    static int generateCustomerId() { return ++lastCustomerId; }
};

int Customer::lastCustomerId = 100;

// ==================== BANK ====================
class Bank {
private:
    vector<unique_ptr<Account>> accounts;    // was a raw fixed array of 50 with manual
                                              // delete in the destructor and a leak path
                                              // in createAccount() when the array was full.
    vector<unique_ptr<Customer>> customers;  // was a raw fixed array of 20 with the same issue.
    string bankName;

public:
    explicit Bank(string name = "MyBank") : bankName(std::move(name)) {}

    string getBankName() const { return bankName; }

    // ===== Customer Management =====
    void addCustomer(unique_ptr<Customer> customer) {
        if (!customer) return;
        cout << "[OK] Customer added successfully! ID: " << customer->getId() << endl;
        customers.push_back(std::move(customer));
    }

    Customer* getCustomer(int id) {
        for (auto& c : customers) {
            if (c->getId() == id) return c.get();
        }
        return nullptr;
    }

    void displayCustomer(int id) const {
        for (const auto& c : customers) {
            if (c->getId() == id) {
                c->displayInfo();
                return;
            }
        }
        cout << "[ERROR] Customer not found!" << endl;
    }

    void displayAllCustomers() const {
        if (customers.empty()) {
            cout << "[INFO] No customers in the bank!" << endl;
            return;
        }
        cout << "\n=== All Customers ===" << endl;
        for (const auto& c : customers) {
            c->displayInfo();
        }
    }

    // ===== Account Management =====
    void createAccount(unique_ptr<Account> account, int customerId) {
        if (!account) return;

        Customer* customer = getCustomer(customerId);
        if (!customer) {
            cout << "[ERROR] Customer not found!" << endl;
            return;   // account (unique_ptr) is freed automatically here - no leak
        }

        cout << "[OK] Account created successfully!" << endl;
        cout << "Account Number: " << account->getAccountNumber() << endl;
        customer->addAccount(account->getAccountNumber());
        accounts.push_back(std::move(account));
    }

    Account* getAccount(int accountNumber) {
        for (auto& a : accounts) {
            if (a->getAccountNumber() == accountNumber) return a.get();
        }
        return nullptr;
    }

    void displayAccount(int accountNumber) const {
        for (const auto& a : accounts) {
            if (a->getAccountNumber() == accountNumber) {
                a->displayInfo();
                return;
            }
        }
        cout << "[ERROR] Account not found!" << endl;
    }

    void displayAllAccounts() const {
        if (accounts.empty()) {
            cout << "[INFO] No accounts in the bank!" << endl;
            return;
        }
        cout << "\n=== All Accounts ===" << endl;
        for (const auto& a : accounts) {
            a->displayInfo();
        }
    }

    // ===== Banking Operations =====
    void deposit(int accountNumber, double amount) {
        Account* acc = getAccount(accountNumber);
        if (acc) acc->deposit(amount);
        else cout << "[ERROR] Account not found!" << endl;
    }

    void withdraw(int accountNumber, double amount) {
        Account* acc = getAccount(accountNumber);
        if (acc) acc->withdraw(amount);
        else cout << "[ERROR] Account not found!" << endl;
    }

    void transfer(int fromAccount, int toAccount, double amount) {
        if (fromAccount == toAccount) {
            cout << "[ERROR] Source and destination accounts must be different!" << endl;
            return;
        }
        Account* from = getAccount(fromAccount);
        Account* to = getAccount(toAccount);

        if (!from || !to) {
            cout << "[ERROR] One or both accounts not found!" << endl;
            return;
        }

        if (from->withdraw(amount)) {
            to->deposit(amount);
            cout << "[OK] Transfer of $" << fixed << setprecision(2) << amount << " completed!" << endl;
        }
    }

    // This was called from the menu (case 6) but never existed on Bank -
    // the program would not even compile. It now actually credits the
    // computed interest to the account's balance and logs it.
    void applyInterest(int accountNumber) {
        Account* acc = getAccount(accountNumber);
        if (!acc) {
            cout << "[ERROR] Account not found!" << endl;
            return;
        }
        double interest = acc->calculateInterest();
        if (interest <= 0) {
            cout << "[INFO] No interest applicable for this account." << endl;
            return;
        }
        acc->deposit(interest);
        cout << "[OK] Interest of $" << fixed << setprecision(2) << interest
             << " applied to account " << accountNumber << "!" << endl;
    }

    void showTransactionHistory(int accountNumber) const {
        for (const auto& a : accounts) {
            if (a->getAccountNumber() == accountNumber) {
                a->showTransactions();
                return;
            }
        }
        cout << "[ERROR] Account not found!" << endl;
    }

    void toggleAccountStatus(int accountNumber) {
        Account* acc = getAccount(accountNumber);
        if (!acc) {
            cout << "[ERROR] Account not found!" << endl;
            return;
        }
        bool currentStatus = acc->getStatus();
        acc->setStatus(!currentStatus);
        cout << "[OK] Account " << (currentStatus ? "deactivated" : "activated") << "!" << endl;
    }

    // ===== Special Features =====
    void showSchemes() const {
        cout << "\nSHOPPING SCHEMES" << endl;
        cout << "========================================" << endl;
        cout << "1. Khaadi - 15% Discount on all products" << endl;
        cout << "2. Sana Safinaz - 10% Discount + Free Shipping" << endl;
        cout << "3. Gul Ahmed - Buy 2 Get 1 Free" << endl;
        cout << "4. Sapphire - 20% Discount on Winter Collection" << endl;
        cout << "5. Bonanza - 25% Discount on Ready-to-Wear" << endl;
        cout << "6. Junaid Jamshed - 30% Discount on Unstitched" << endl;
        cout << "========================================\n" << endl;
    }

    void processDonation(int accountNumber) {
        Account* acc = getAccount(accountNumber);
        if (!acc) {
            cout << "[ERROR] Account not found!" << endl;
            return;
        }

        cout << "\nZAKAT & CHARITY OPTIONS" << endl;
        cout << "1. Zakat (2.5% of balance)" << endl;
        cout << "2. Sadqa (Fixed amount)" << endl;
        cout << "3. Fitrana (Per person)" << endl;
        int choice = readNumber<int>("Enter your choice: ");

        double amount = 0;
        switch (choice) {
            case 1:
                amount = acc->getBalance() * 0.025;
                cout << "Zakat calculated: $" << fixed << setprecision(2) << amount << endl;
                break;
            case 2:
                amount = readNumber<double>("Enter Sadqa amount: $");
                break;
            case 3: {
                int people = readNumber<int>("Enter number of people: ");
                double perPerson = readNumber<double>("Enter Fitrana per person: $");
                if (people < 0) {
                    cout << "[ERROR] Number of people cannot be negative!" << endl;
                    return;
                }
                amount = people * perPerson;
                break;
            }
            default:
                cout << "[ERROR] Invalid choice!" << endl;
                return;
        }

        if (amount <= 0) {
            cout << "[ERROR] Donation amount must be positive!" << endl;
            return;
        }

        if (acc->withdraw(amount)) {
            cout << "[OK] Donation of $" << fixed << setprecision(2) << amount << " processed successfully!" << endl;
            cout << "May your charity be accepted." << endl;
        }
    }

    void processShopping(int accountNumber) {
        Account* acc = getAccount(accountNumber);
        if (!acc) {
            cout << "[ERROR] Account not found!" << endl;
            return;
        }

        showSchemes();
        int brand = readNumber<int>("Select a brand (1-6): ");
        double amount = readNumber<double>("Enter shopping amount: $");

        if (amount <= 0) {
            cout << "[ERROR] Shopping amount must be positive!" << endl;
            return;
        }

        double discount = 0;
        switch (brand) {
            case 1: discount = 0.15; break; // Khaadi
            case 2: discount = 0.10; break; // Sana Safinaz
            case 3: discount = 0.0;
                    cout << "Buy 2 Get 1 Free offer applied!" << endl;
                    break;
            case 4: discount = 0.20; break; // Sapphire
            case 5: discount = 0.25; break; // Bonanza
            case 6: discount = 0.30; break; // Junaid Jamshed
            default:
                cout << "[ERROR] Invalid brand!" << endl;
                return;
        }

        double discountedAmount = amount * (1 - discount);
        cout << "Original Amount: $" << fixed << setprecision(2) << amount << endl;
        cout << "Discount: " << (discount * 100) << "%" << endl;
        cout << "Amount to Pay: $" << fixed << setprecision(2) << discountedAmount << endl;

        if (acc->withdraw(discountedAmount)) {
            cout << "[OK] Shopping done successfully! Enjoy your purchase!" << endl;
        }
    }

    void generateBankReport() const {
        cout << "\nBANK REPORT - " << bankName << endl;
        cout << "========================================" << endl;
        cout << "Total Customers: " << customers.size() << endl;
        cout << "Total Accounts: " << accounts.size() << endl;

        double totalBalance = 0;
        for (const auto& a : accounts) {
            totalBalance += a->getBalance();
        }
        cout << "Total Bank Balance: $" << fixed << setprecision(2) << totalBalance << endl;
        cout << "========================================\n" << endl;
    }
};

// ==================== LOGIN SYSTEM ====================
bool login() {
    const string validUsername = "admin";
    const string validPassword = "admin123";
    int attempts = 0;

    cout << "\nLOGIN SYSTEM" << endl;
    cout << "========================================" << endl;

    while (attempts < 3) {
        string username = readLine("Username: ");
        string password = readLine("Password: ");

        if (username == validUsername && password == validPassword) {
            cout << "[OK] Login successful!" << endl;
            return true;
        }

        attempts++;
        cout << "[ERROR] Invalid credentials! Attempts remaining: " << (3 - attempts) << endl;
    }

    cout << "[ERROR] Too many failed attempts! Exiting..." << endl;
    return false;
}

// ==================== MAIN FUNCTION ====================
int main() {
    if (!login()) {
        return 0;
    }

    Bank bank("MyBank");
    int choice;

    cout << "\n----------------------------------------" << endl;
    cout << "     WELCOME TO " << bank.getBankName() << endl;
    cout << "----------------------------------------" << endl;

    do {
        cout << "\n----------------------------------------" << endl;
        cout << "         BANK MANAGEMENT SYSTEM" << endl;
        cout << "----------------------------------------" << endl;
        cout << "  1.  Add New Customer" << endl;
        cout << "  2.  Create Account" << endl;
        cout << "  3.  Deposit Money" << endl;
        cout << "  4.  Withdraw Money" << endl;
        cout << "  5.  Transfer Money" << endl;
        cout << "  6.  Apply Interest" << endl;
        cout << "  7.  View Account Details" << endl;
        cout << "  8.  View Customer Details" << endl;
        cout << "  9.  Transaction History" << endl;
        cout << "  10. View All Accounts" << endl;
        cout << "  11. View All Customers" << endl;
        cout << "  12. Donation (Zakat/Sadqa)" << endl;
        cout << "  13. Shopping with Discount" << endl;
        cout << "  14. View Shopping Schemes" << endl;
        cout << "  15. Toggle Account Status" << endl;
        cout << "  16. Generate Bank Report" << endl;
        cout << "  0.  Exit" << endl;
        cout << "----------------------------------------" << endl;
        choice = readNumber<int>("Enter your choice: ");

        switch (choice) {
            case 0:
                cout << "\nThank you for using the Bank Management System!" << endl;
                cout << "Have a great day!" << endl;
                break;

            case 1: {
                string name = readLine("Enter Customer Name: ");
                string address = readLine("Enter Address: ");
                string phone = readLine("Enter Phone: ");
                string email = readLine("Enter Email: ");

                bank.addCustomer(unique_ptr<Customer>(new Customer(name, address, phone, email)));
                break;
            }

            case 2: {
                int customerId = readNumber<int>("Enter Customer ID: ");

                Customer* customer = bank.getCustomer(customerId);
                if (!customer) {
                    cout << "[ERROR] Customer not found!" << endl;
                    break;
                }

                cout << "Select Account Type:" << endl;
                cout << "1. Savings Account" << endl;
                cout << "2. Current Account" << endl;
                int type = readNumber<int>("Enter choice: ");

                double initialBalance = readNumber<double>("Enter Initial Balance: $");
                if (initialBalance < 0) {
                    cout << "[ERROR] Initial balance cannot be negative!" << endl;
                    break;
                }

                unique_ptr<Account> account;

                switch (type) {
                    case 1: {
                        double rate = readNumber<double>("Enter Interest Rate (e.g., 0.04 for 4%): ");
                        double minBal = readNumber<double>("Enter Minimum Balance: $");
                        if (rate < 0 || minBal < 0) {
                            cout << "[ERROR] Interest rate and minimum balance cannot be negative!" << endl;
                            break;
                        }
                        account = unique_ptr<Account>(new SavingsAccount(customer->getName(), initialBalance, rate, minBal));
                        break;
                    }
                    case 2: {
                        double overdraft = readNumber<double>("Enter Overdraft Limit: $");
                        if (overdraft < 0) {
                            cout << "[ERROR] Overdraft limit cannot be negative!" << endl;
                            break;
                        }
                        account = unique_ptr<Account>(new CurrentAccount(customer->getName(), initialBalance, overdraft));
                        break;
                    }
                    default:
                        cout << "[ERROR] Invalid account type!" << endl;
                        break;
                }

                if (account) {
                    bank.createAccount(std::move(account), customerId);
                }
                break;
            }

            case 3: {
                int accNum = readNumber<int>("Enter Account Number: ");
                double amount = readNumber<double>("Enter Amount to Deposit: $");
                bank.deposit(accNum, amount);
                break;
            }

            case 4: {
                int accNum = readNumber<int>("Enter Account Number: ");
                double amount = readNumber<double>("Enter Amount to Withdraw: $");
                bank.withdraw(accNum, amount);
                break;
            }

            case 5: {
                int fromAcc = readNumber<int>("Enter Source Account: ");
                int toAcc = readNumber<int>("Enter Destination Account: ");
                double amount = readNumber<double>("Enter Amount: $");
                bank.transfer(fromAcc, toAcc, amount);
                break;
            }

            case 6: {
                int accNum = readNumber<int>("Enter Account Number: ");
                bank.applyInterest(accNum);
                break;
            }

            case 7: {
                int accNum = readNumber<int>("Enter Account Number: ");
                bank.displayAccount(accNum);
                break;
            }

            case 8: {
                int custId = readNumber<int>("Enter Customer ID: ");
                bank.displayCustomer(custId);
                break;
            }

            case 9: {
                int accNum = readNumber<int>("Enter Account Number: ");
                bank.showTransactionHistory(accNum);
                break;
            }

            case 10:
                bank.displayAllAccounts();
                break;

            case 11:
                bank.displayAllCustomers();
                break;

            case 12: {
                int accNum = readNumber<int>("Enter Account Number: ");
                bank.processDonation(accNum);
                break;
            }

            case 13: {
                int accNum = readNumber<int>("Enter Account Number: ");
                bank.processShopping(accNum);
                break;
            }

            case 14:
                bank.showSchemes();
                break;

            case 15: {
                int accNum = readNumber<int>("Enter Account Number: ");
                bank.toggleAccountStatus(accNum);
                break;
            }

            case 16:
                bank.generateBankReport();
                break;

            default:
                cout << "[ERROR] Invalid choice! Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}

