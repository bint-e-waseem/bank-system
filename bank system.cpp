#include<iostream>
#include<string>
#include<ctime>
#include<sstream>
#include<iomanip>
#include <vector>
using namespace std;
class Transaction {
private:
    string type;
    double amount;
    string description;
    time_t timestamp;
    
public:
    Transaction() : type(""), amount(0), description(""), timestamp(time(nullptr)) {}
    Transaction(string t, double a, string desc = "") 
        : type(t), amount(a), description(desc), timestamp(time(nullptr)) {}
    void display() const {
        cout << "  Type: " << type << endl;
        cout << "  Amount: $" << fixed << setprecision(2) << amount << endl;
        if(!description.empty()) cout << "  Note: " << description << endl;
        cout << "  ------------------------" << endl;
    }
};
class Account {
protected:
    int accountNumber;
    string holderName;
    double balance;
    string accountType;
    Transaction transactions[50]; 
    int transactionCount;
    bool isActive;
    static int lastAccountNumber;
    
public:
    Account() : accountNumber(0), holderName(""), balance(0), 
                accountType(""), transactionCount(0), isActive(true) {}
    
    Account(string name, double initialBalance, string type) 
        : accountNumber(++lastAccountNumber), holderName(name), 
          balance(initialBalance), accountType(type), transactionCount(0), isActive(true) {}
    
    virtual ~Account() {}
    int getAccountNumber() const { return accountNumber; }
    string getHolderName() const { return holderName; }
    double getBalance() const { return balance; }
    string getAccountType() const { return accountType; }
    bool getStatus() const { return isActive; }
    void setHolderName(const string& name) { holderName = name; }
    void setStatus(bool status) { isActive = status; }
    virtual void deposit(double amount) {
        if(amount <= 0) {
            cout << "? Invalid deposit amount!" << endl;
            return;
        }
        balance += amount;
        if(transactionCount < 50) {
            transactions[transactionCount++] = Transaction("DEPOSIT", amount);
        }
        cout << "? Deposited $" << amount << " successfully!" << endl;
    }
    
    virtual bool withdraw(double amount) {
        if(amount <= 0) {
            cout << "? Invalid withdrawal amount!" << endl;
            return false;
        }
        if(amount > balance) {
            cout << "? Insufficient balance!" << endl;
            return false;
        }
        if(!isActive) {
            cout << "? Account is inactive!" << endl;
            return false;
        }
        balance -= amount;
        if(transactionCount < 50) {
            transactions[transactionCount++] = Transaction("WITHDRAWAL", amount);
        }
        cout << "? Withdrawn $" << amount << " successfully!" << endl;
        return true;
    }
    virtual double calculateInterest() = 0;
    virtual void displayInfo() const {
        cout << "\n========================================" << endl;
        cout << "?? ACCOUNT DETAILS" << endl;
        cout << "========================================" << endl;
        cout << "Account No: " << accountNumber << endl;
        cout << "Holder Name: " << holderName << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "Status: " << (isActive ? "?? Active" : "?? Inactive") << endl;
        cout << "========================================\n" << endl;
    }
    void showTransactions() const {
        if(transactionCount == 0) {
            cout << "?? No transactions found!" << endl;
            return;
        }
        cout << "\n?? TRANSACTION HISTORY" << endl;
        cout << "========================================" << endl;
        for(int i = 0; i < transactionCount; i++) {
            transactions[i].display();
        }
        cout << "========================================\n" << endl;
    }
    
    static int generateAccountNumber() { return ++lastAccountNumber; }
};

int Account::lastAccountNumber = 1000;

class SavingsAccount : public Account {
private:
    double interestRate;
    double minimumBalance;
    
public:
    SavingsAccount(string name, double initialBalance, double rate = 0.04, double minBal = 500) 
        : Account(name, initialBalance, "Savings"), interestRate(rate), minimumBalance(minBal) {}
    
    double calculateInterest() override {
        return balance * interestRate;
    }
    
    bool withdraw(double amount) override {
        if(balance - amount < minimumBalance) {
            cout << "? Minimum balance of $" << minimumBalance << " required!" << endl;
            return false;
        }
        return Account::withdraw(amount);
    }
    
    void displayInfo() const override {
        Account::displayInfo();
        cout << "Interest Rate: " << (interestRate * 100) << "%" << endl;
        cout << "Minimum Balance: $" << minimumBalance << endl;
    }
};
class CurrentAccount : public Account {
private:
    double overdraftLimit;
    
public:
    CurrentAccount(string name, double initialBalance, double overdraft = 5000) 
        : Account(name, initialBalance, "Current"), overdraftLimit(overdraft) {}
    
    double calculateInterest() override { return 0; }
    
    bool withdraw(double amount) override {
        if(amount > balance + overdraftLimit) {
            cout << "? Exceeds overdraft limit of $" << overdraftLimit << endl;
            return false;
        }
        return Account::withdraw(amount);
    }
    
    void displayInfo() const override {
        Account::displayInfo();
        cout << "Overdraft Limit: $" << overdraftLimit << endl;
    }
};
class Customer {
private:
    int customerId;
    string name;
    string address;
    string phone;
    string email;
    int accountNumbers[10];  
    int accountCount;
    bool isActive;
    static int lastCustomerId;
    
public:
    Customer() : customerId(0), name(""), address(""), phone(""), 
                 email(""), accountCount(0), isActive(true) {}
    
    Customer(string n, string addr, string ph, string em) 
        : customerId(++lastCustomerId), name(n), address(addr), 
          phone(ph), email(em), accountCount(0), isActive(true) {}

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
        if(accountCount < 10) {
            accountNumbers[accountCount++] = accNo;
        }
    }
    
    void displayInfo() const {
        cout << "\n========================================" << endl;
        cout << "?? CUSTOMER DETAILS" << endl;
        cout << "========================================" << endl;
        cout << "Customer ID: " << customerId << endl;
        cout << "Name: " << name << endl;
        cout << "Address: " << address << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
        cout << "Accounts: ";
        if(accountCount == 0) cout << "None";
        else {
            for(int i = 0; i < accountCount; i++) {
                cout << accountNumbers[i] << " ";
            }
        }
        cout << endl;
        cout << "Status: " << (isActive ? "?? Active" : "?? Inactive") << endl;
        cout << "========================================\n" << endl;
    }
    
    static int generateCustomerId() { return ++lastCustomerId; }
};

int Customer::lastCustomerId = 100;

// ==================== BANK CLASS ====================
class Bank {
private:
    Account* accounts[50];  
    Customer* customers[20]; // Array of customers
    int accountCount;
    int customerCount;
    string bankName;
    
public:
    Bank(const string& name = "MyBank") : bankName(name), accountCount(0), customerCount(0) {
        // Initialize arrays with null
        for(int i = 0; i < 50; i++) accounts[i] = nullptr;
        for(int i = 0; i < 20; i++) customers[i] = nullptr;
    }
    
    ~Bank() {
        // Clean up memory
        for(int i = 0; i < accountCount; i++) {
            delete accounts[i];
        }
        for(int i = 0; i < customerCount; i++) {
            delete customers[i];
        }
    }
    
    string getBankName() const { return bankName; }
    
    // ===== Customer Management =====
    void addCustomer(Customer* customer) {
        if(!customer || customerCount >= 20) return;
        customers[customerCount++] = customer;
        cout << "? Customer added successfully! ID: " << customer->getId() << endl;
    }
    
    Customer* getCustomer(int id) {
        for(int i = 0; i < customerCount; i++) {
            if(customers[i]->getId() == id) {
                return customers[i];
            }
        }
        return nullptr;
    }
    
    void displayCustomer(int id) const {
        for(int i = 0; i < customerCount; i++) {
            if(customers[i]->getId() == id) {
                customers[i]->displayInfo();
                return;
            }
        }
        cout << "? Customer not found!" << endl;
    }
    
    void displayAllCustomers() const {
        if(customerCount == 0) {
            cout << "?? No customers in the bank!" << endl;
            return;
        }
        cout << "\n=== All Customers ===" << endl;
        for(int i = 0; i < customerCount; i++) {
            customers[i]->displayInfo();
        }
    }
    
    // ===== Account Management =====
    void createAccount(Account* account, int customerId) {
        if(!account || accountCount >= 50) return;
        
        Customer* customer = getCustomer(customerId);
        if(!customer) {
            cout << "? Customer not found!" << endl;
            delete account;
            return;
        }
        
        accounts[accountCount++] = account;
        customer->addAccount(account->getAccountNumber());
        cout << "? Account created successfully!" << endl;
        cout << "?? Account Number: " << account->getAccountNumber() << endl;
    }
    
    Account* getAccount(int accountNumber) {
        for(int i = 0; i < accountCount; i++) {
            if(accounts[i]->getAccountNumber() == accountNumber) {
                return accounts[i];
            }
        }
        return nullptr;
    }
    
    void displayAccount(int accountNumber) const {
        for(int i = 0; i < accountCount; i++) {
            if(accounts[i]->getAccountNumber() == accountNumber) {
                accounts[i]->displayInfo();
                return;
            }
        }
        cout << "? Account not found!" << endl;
    }
    
    void displayAllAccounts() const {
        if(accountCount == 0) {
            cout << "?? No accounts in the bank!" << endl;
            return;
        }
        cout << "\n=== All Accounts ===" << endl;
        for(int i = 0; i < accountCount; i++) {
            accounts[i]->displayInfo();
        }
    }
    
    // ===== Banking Operations =====
    void deposit(int accountNumber, double amount) {
        Account* acc = getAccount(accountNumber);
        if(acc) {
            acc->deposit(amount);
        } else {
            cout << "? Account not found!" << endl;
        }
    }
    
    void withdraw(int accountNumber, double amount) {
        Account* acc = getAccount(accountNumber);
        if(acc) {
            acc->withdraw(amount);
        } else {
            cout << "? Account not found!" << endl;
        }
    }
    
    void transfer(int fromAccount, int toAccount, double amount) {
        Account* from = getAccount(fromAccount);
        Account* to = getAccount(toAccount);
        
        if(!from || !to) {
            cout << "? One or both accounts not found!" << endl;
            return;
        }
        
        if(from->withdraw(amount)) {
            to->deposit(amount);
            cout << "? Transfer of $" << amount << " completed!" << endl;
        }
    }
    
    void showTransactionHistory(int accountNumber) const {
        for(int i = 0; i < accountCount; i++) {
            if(accounts[i]->getAccountNumber() == accountNumber) {
                accounts[i]->showTransactions();
                return;
            }
        }
        cout << "? Account not found!" << endl;
    }
    
    void toggleAccountStatus(int accountNumber) {
        Account* acc = getAccount(accountNumber);
        if(!acc) {
            cout << "? Account not found!" << endl;
            return;
        }
        bool currentStatus = acc->getStatus();
        acc->setStatus(!currentStatus);
        cout << "? Account " << (currentStatus ? "deactivated" : "activated") << "!" << endl;
    }
    
    // ===== Special Features =====
    void showSchemes() {
        cout << "\n?? EXCLUSIVE SHOPPING SCHEMES ??" << endl;
        cout << "========================================" << endl;
        cout << "1. Khaadi - 15% Discount on all products" << endl;
        cout << "2. Sana Safinaz - 10% Discount + Free Shipping" << endl;
        cout << "3. Gul Ahmed - Buy 2 Get 1 Free" << endl;
        cout << "4. Sapphire - 20% Discount on Winter Collection" << endl;
        cout << "5. Bonanza - 25% Discount on Ready-to-Wear" << endl;
        cout << "6. Junaid Jamshed - 30% Discount on Unstitched" << endl;
        cout << "========================================\n" << endl;
    }
    
    void showTransactionOptions() {
        cout << "\n?? TRANSACTION OPTIONS ??" << endl;
        cout << "========================================" << endl;
        cout << "1. Donation (Zakat/Charity)" << endl;
        cout << "2. Shopping" << endl;
        cout << "3. Utility Bill Payment" << endl;
        cout << "4. Transfer to Another Account" << endl;
        cout << "5. Cash Withdrawal" << endl;
        cout << "========================================" << endl;
    }
    
    void processDonation(int accountNumber) {
        Account* acc = getAccount(accountNumber);
        if(!acc) {
            cout << "? Account not found!" << endl;
            return;
        }
        
        cout << "\n?? ZAKAT & CHARITY OPTIONS ??" << endl;
        cout << "1. Zakat (2.5% of balance)" << endl;
        cout << "2. Sadqa (Fixed amount)" << endl;
        cout << "3. Fitrana (Per person)" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        
        double amount = 0;
        switch(choice) {
            case 1:
                amount = acc->getBalance() * 0.025;
                cout << "Zakat calculated: $" << amount << endl;
                break;
            case 2:
                cout << "Enter Sadqa amount: $";
                cin >> amount;
                break;
            case 3:
                int people;
                cout << "Enter number of people: ";
                cin >> people;
                cout << "Enter Fitrana per person: $";
                double perPerson;
                cin >> perPerson;
                amount = people * perPerson;
                break;
            default:
                cout << "Invalid choice!" << endl;
                return;
        }
        
        if(acc->withdraw(amount)) {
            cout << "? Donation of $" << amount << " processed successfully!" << endl;
            cout << "May Allah accept your charity! ??" << endl;
        }
    }
    
    void processShopping(int accountNumber) {
        Account* acc = getAccount(accountNumber);
        if(!acc) {
            cout << "? Account not found!" << endl;
            return;
        }
        
        showSchemes();
        cout << "Select a brand (1-6): ";
        int brand;
        cin >> brand;
        
        cout << "Enter shopping amount: $";
        double amount;
        cin >> amount;
        
        double discount = 0;
        switch(brand) {
            case 1: discount = 0.15; break; // Khaadi
            case 2: discount = 0.10; break; // Sana Safinaz
            case 3: discount = 0.0; 
                    cout << "Buy 2 Get 1 Free offer applied!" << endl; 
                    break;
            case 4: discount = 0.20; break; // Sapphire
            case 5: discount = 0.25; break; // Bonanza
            case 6: discount = 0.30; break; // Junaid Jamshed
            default: 
                cout << "Invalid brand!" << endl;
                return;
        }
        
        double discountedAmount = amount * (1 - discount);
        cout << "Original Amount: $" << amount << endl;
        cout << "Discount: " << (discount * 100) << "%" << endl;
        cout << "Amount to Pay: $" << discountedAmount << endl;
        
        if(acc->withdraw(discountedAmount)) {
            cout << "? Shopping done successfully!" << endl;
            cout << "?? Enjoy your purchase!" << endl;
        }
    }
    
    void generateBankReport() const {
        cout << "\n?? BANK REPORT - " << bankName << endl;
        cout << "========================================" << endl;
        cout << "Total Customers: " << customerCount << endl;
        cout << "Total Accounts: " << accountCount << endl;
        
        double totalBalance = 0;
        for(int i = 0; i < accountCount; i++) {
            totalBalance += accounts[i]->getBalance();
        }
        cout << "Total Bank Balance: $" << fixed << setprecision(2) << totalBalance << endl;
        cout << "========================================\n" << endl;
    }
};

// ==================== LOGIN SYSTEM ====================
bool login() {
    string username, password;
    int attempts = 0;
    
    cout << "\n?? LOGIN SYSTEM" << endl;
    cout << "========================================" << endl;
    
    while(attempts < 3) {
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        
        if(username == "admin" && password == "admin123") {
            cout << "? Login successful!" << endl;
            return true;
        }
        
        attempts++;
        cout << "? Invalid credentials! Attempts remaining: " << (3 - attempts) << endl;
    }
    
    cout << "? Too many failed attempts! Exiting..." << endl;
    return false;
}

// ==================== MAIN FUNCTION ====================
int main() {
    // Login first
    if(!login()) {
        return 0;
    }
    
    Bank bank("MyBank");
    int choice;
    
    cout << "\n+----------------------------------------+" << endl;
    cout << "¦     WELCOME TO " << bank.getBankName() << "       ¦" << endl;
    cout << "+----------------------------------------+" << endl;
    
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦         BANK MANAGEMENT SYSTEM       ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦  1.  Add New Customer                 ¦" << endl;
        cout << "¦  2.  Create Account                   ¦" << endl;
        cout << "¦  3.  Deposit Money                    ¦" << endl;
        cout << "¦  4.  Withdraw Money                   ¦" << endl;
        cout << "¦  5.  Transfer Money                   ¦" << endl;
        cout << "¦  6.  Apply Interest                   ¦" << endl;
        cout << "¦  7.  View Account Details             ¦" << endl;
        cout << "¦  8.  View Customer Details            ¦" << endl;
        cout << "¦  9.  Transaction History              ¦" << endl;
        cout << "¦  10. View All Accounts                ¦" << endl;
        cout << "¦  11. View All Customers               ¦" << endl;
        cout << "¦  12. DONATION (Zakat/Sadqa)          ¦" << endl;
        cout << "¦  13. SHOPPING with Discount          ¦" << endl;
        cout << "¦  14. View Shopping Schemes           ¦" << endl;
        cout << "¦  15. Toggle Account Status           ¦" << endl;
        cout << "¦  16. Generate Bank Report            ¦" << endl;
        cout << "¦  0.  Exit                            ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 0:
                cout << "\n?? Thank you for using the Bank Management System!" << endl;
                cout << "Have a great day! ??" << endl;
                break;
                
            case 1: {
                string name, address, phone, email;
                cin.ignore();
                cout << "Enter Customer Name: ";
                getline(cin, name);
                cout << "Enter Address: ";
                getline(cin, address);
                cout << "Enter Phone: ";
                getline(cin, phone);
                cout << "Enter Email: ";
                getline(cin, email);
                
                Customer* customer = new Customer(name, address, phone, email);
                bank.addCustomer(customer);
                break;
            }
            
            case 2: {
                int customerId;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                
                Customer* customer = bank.getCustomer(customerId);
                if(!customer) {
                    cout << "? Customer not found!" << endl;
                    break;
                }
                
                cout << "Select Account Type:" << endl;
                cout << "1. Savings Account" << endl;
                cout << "2. Current Account" << endl;
                cout << "Enter choice: ";
                int type;
                cin >> type;
                
                double initialBalance;
                cout << "Enter Initial Balance: $";
                cin >> initialBalance;
                
                Account* account = nullptr;
                
                switch(type) {
                    case 1: {
                        double rate, minBal;
                        cout << "Enter Interest Rate (e.g., 0.04 for 4%): ";
                        cin >> rate;
                        cout << "Enter Minimum Balance: $";
                        cin >> minBal;
                        account = new SavingsAccount(customer->getName(), initialBalance, rate, minBal);
                        break;
                    }
                    case 2: {
                        double overdraft;
                        cout << "Enter Overdraft Limit: $";
                        cin >> overdraft;
                        account = new CurrentAccount(customer->getName(), initialBalance, overdraft);
                        break;
                    }
                    default:
                        cout << "? Invalid account type!" << endl;
                        continue;
                }
                
                bank.createAccount(account, customerId);
                break;
            }
            
            case 3: {
                int accNum;
                double amount;
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter Amount to Deposit: $";
                cin >> amount;
                bank.deposit(accNum, amount);
                break;
            }
            
            case 4: {
                int accNum;
                double amount;
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter Amount to Withdraw: $";
                cin >> amount;
                bank.withdraw(accNum, amount);
                break;
            }
            
            case 5: {
                int fromAcc, toAcc;
                double amount;
                cout << "Enter Source Account: ";
                cin >> fromAcc;
                cout << "Enter Destination Account: ";
                cin >> toAcc;
                cout << "Enter Amount: $";
                cin >> amount;
                bank.transfer(fromAcc, toAcc, amount);
                break;
            }
            
            case 6: {
                int accNum;
                cout << "Enter Account Number: ";
                cin >> accNum;
                bank.applyInterest(accNum); 
                break;
            }
            
            case 7: {
                int accNum;
                cout << "Enter Account Number: ";
                cin >> accNum;
                bank.displayAccount(accNum);
                break;
            }
            
            case 8: {
                int custId;
                cout << "Enter Customer ID: ";
                cin >> custId;
                bank.displayCustomer(custId);
                break;
            }
            
            case 9: {
                int accNum;
                cout << "Enter Account Number: ";
                cin >> accNum;
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
                int accNum;
                cout << "Enter Account Number: ";
                cin >> accNum;
                bank.processDonation(accNum);
                break;
            }
            
            case 13: {
                int accNum;
                cout << "Enter Account Number: ";
                cin >> accNum;
                bank.processShopping(accNum);
                break;
            }
            
            case 14:
                bank.showSchemes();
                break;
                
            case 15: {
                int accNum;
                cout << "Enter Account Number: ";
                cin >> accNum;
                bank.toggleAccountStatus(accNum);
                break;
            }
            
            case 16:
                bank.generateBankReport();
                break;
                
            default:
                cout << "? Invalid choice! Please try again." << endl;
        }
    } while(choice != 0);
    
    return 0;
}
