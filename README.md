# 🏦 Bank Management System

A comprehensive C++ banking system with Islamic banking features, shopping discounts, and donation management. Built with Object-Oriented Programming principles.


## 🎯 Overview

The **Bank Management System** is a console-based application designed to manage banking operations with a unique focus on Islamic banking principles and local shopping integrations. It provides a complete solution for customer management, account operations, and special features like Zakat calculation and brand discounts.

### Key Highlights
- ✅ **Complete Banking Operations** - Deposit, Withdraw, Transfer
- ✅ **Islamic Banking** - Zakat, Sadqa, Fitrana calculations
- ✅ **Shopping Integration** - Discounts from popular Pakistani brands
- ✅ **Account Management** - Savings and Current accounts with different features
- ✅ **Transaction History** - Track all account activities
- ✅ **Customer Management** - Complete CRUD operations

---

## ✨ Features

### 🏦 Core Banking Features
| Feature | Description |
|---------|-------------|
| Customer Management | Add, view, and manage customer details |
| Account Creation | Savings & Current accounts with custom parameters |
| Deposits | Secure deposit system with validation |
| Withdrawals | With overdraft support for Current accounts |
| Transfers | Transfer funds between accounts |
| Interest Calculation | Automatic interest for Savings accounts |
| Transaction History | View detailed transaction logs |
| Account Status | Activate/Deactivate accounts |
| Bank Reports | Generate comprehensive bank summaries |

### 🕌 Islamic Banking Features
- **Zakat** - Automatic 2.5% calculation based on balance
- **Sadqa** - Fixed amount charity donations
- **Fitrana** - Per person donation calculation
- **Special Messages** - Islamic blessings on transactions

### 🛍️ Shopping Discounts
Exclusive discounts from top Pakistani brands:

| Brand | Discount | Special Offer |
|-------|----------|---------------|
| Khaadi | 15% | All products |
| Sana Safinaz | 10% | Free shipping |
| Gul Ahmed | Varies | Buy 2 Get 1 Free |
| Sapphire | 20% | Winter collection |
| Bonanza | 25% | Ready-to-wear |
| Junaid Jamshed | 30% | Unstitched fabric |

---

## 🚀 Getting Started

### Prerequisites
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Terminal/Command Prompt
- No external dependencies required

### Installation

1. **Clone the repository**
```bash
git clone https://github.com/yourusername/bank-management-system.git
cd bank-management-system
```

2. **Compile the program**
```bash
g++ -std=c++17 bank_system.cpp -o bank_system
```

3. **Run the program**
```bash
./bank_system      # On Linux/Mac
bank_system.exe    # On Windows
```

### Default Login Credentials
| Field | Value |
|-------|-------|
| Username | `admin` |
| Password | `admin123` |

---

## 🏗️ System Architecture

### Class Diagram Overview

```
┌─────────────────────────────────────────────────────────────┐
│                         Bank                              │
│  - accounts: vector<Account*>                            │
│  - customers: vector<Customer*>                         │
│  + deposit(), withdraw(), transfer()                    │
│  + processDonation(), processShopping()                 │
└─────────────────────────────────────────────────────────────┘
                           │
                           │ manages
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                      Account (Abstract)                    │
│  - accountNumber: int                                     │
│  - holderName: string                                     │
│  - balance: double                                        │
│  + deposit(), withdraw()                                  │
│  + calculateInterest() = 0 (Pure Virtual)                │
└─────────────────────────────────────────────────────────────┘
            ▲                              ▲
            │                              │
            │                              │
┌───────────────────────┐   ┌────────────────────────────┐
│   SavingsAccount      │   │    CurrentAccount          │
│  - interestRate       │   │   - overdraftLimit         │
│  - minimumBalance     │   │   + withdraw() overridden  │
│  + calculateInterest()│   │   + calculateInterest()=0  │
└───────────────────────┘   └────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                      Customer                             │
│  - customerId: int                                       │
│  - name, address, phone, email                           │
│  - accountNumbers: int[]                                 │
│  + displayInfo(), addAccount()                           │
└─────────────────────────────────────────────────────────────┘
```

### Inheritance Hierarchy
- **Account** (Base Abstract Class)
  - **SavingsAccount** - Interest rate, minimum balance
  - **CurrentAccount** - Overdraft limit support

---

## 📖 Usage Guide

### Main Menu Options

```
+----------------------------------------+
¦         BANK MANAGEMENT SYSTEM          ¦
¦----------------------------------------¦
¦  1.  Add New Customer                   ¦
¦  2.  Create Account                     ¦
¦  3.  Deposit Money                      ¦
¦  4.  Withdraw Money                     ¦
¦  5.  Transfer Money                     ¦
¦  6.  Apply Interest                     ¦
¦  7.  View Account Details               ¦
¦  8.  View Customer Details              ¦
¦  9.  Transaction History                ¦
¦  10. View All Accounts                  ¦
¦  11. View All Customers                 ¦
¦  12. DONATION (Zakat/Sadqa)            ¦
¦  13. SHOPPING with Discount            ¦
¦  14. View Shopping Schemes             ¦
¦  15. Toggle Account Status             ¦
¦  16. Generate Bank Report              ¦
¦  0.  Exit                              ¦
+----------------------------------------+
```

### Example Workflow

1. **Add a Customer**
```bash
Enter Customer Name: Ahmad Khan
Enter Address: Islamabad, Pakistan
Enter Phone: 0333-1234567
Enter Email: ahmad@email.com
✓ Customer added successfully! ID: 100
```

2. **Create Account**
```bash
Enter Customer ID: 100
Select Account Type:
1. Savings Account
2. Current Account
Enter choice: 1
Enter Initial Balance: $5000
Enter Interest Rate: 0.05
Enter Minimum Balance: $1000
✓ Account created successfully!
🎯 Account Number: 1001
```

3. **Make Donation**
```bash
Enter Account Number: 1001
🔹 ZAKAT & CHARITY OPTIONS 🔹
1. Zakat (2.5% of balance)
2. Sadqa (Fixed amount)
3. Fitrana (Per person)
Enter your choice: 1
Zakat calculated: $125.00
✓ Donation of $125.00 processed successfully!
May Allah accept your charity! 🤲
```

4. **Shopping with Discount**
```bash
Enter Account Number: 1001

🎯 EXCLUSIVE SHOPPING SCHEMES 🎯
1. Khaadi - 15% Discount
2. Sana Safinaz - 10% Discount + Free Shipping
...

Select a brand: 1
Enter shopping amount: $1000
Original Amount: $1000.00
Discount: 15%
Amount to Pay: $850.00
✓ Shopping done successfully!
🎉 Enjoy your purchase!
```

---

## 🕌 Islamic Banking Features

### Zakat Calculation
- **Rate**: 2.5% (0.025) of account balance
- **Trigger**: Threshold of gold/silver value
- **Purpose**: Obligatory charity in Islam

### Sadqa
- **Type**: Voluntary charity
- **Amount**: User-defined fixed amount
- **Purpose**: Optional good deeds

### Fitrana
- **Rate**: Per person donation
- **Calculation**: People × Per-person amount
- **Purpose**: Charity before Eid prayer

---

## 🛍️ Shopping Discounts

### Discount Calculation
```cpp
double discountedAmount = amount * (1 - discount);
```

| Brand | Discount | Type |
|-------|----------|------|
| Khaadi | 15% | Percentage |
| Sana Safinaz | 10% | Percentage |
| Gul Ahmed | Varies | BOGO Offer |
| Sapphire | 20% | Percentage |
| Bonanza | 25% | Percentage |
| Junaid Jamshed | 30% | Percentage |

---

## 📁 Code Structure

### File: bank_system.cpp

```cpp
// ==================== HEADER IMPORTS ====================
#include <iostream>    // I/O operations
#include <string>      // String handling
#include <ctime>       // Timestamp for transactions
#include <iomanip>     // Output formatting
#include <vector>      // Dynamic arrays (used for future)

// ==================== CLASS DEFINITIONS ====================
class Transaction { /* ... */ };      // Transaction records
class Account { /* ... */ };          // Base account class
class SavingsAccount : public Account; // Savings account
class CurrentAccount : public Account; // Current account
class Customer { /* ... */ };         // Customer management
class Bank { /* ... */ };             // Main banking system

// ==================== MAIN FUNCTION ====================
int main() { /* ... */ }
```

---

## 🔮 Future Improvements

### Planned Features

#### 🚀 Critical Enhancements
- [ ] Replace fixed arrays with `std::vector` for dynamic sizing
- [ ] Implement database persistence (SQLite/PostgreSQL)
- [ ] Add proper error handling with try-catch blocks
- [ ] Implement input validation class
- [ ] Fix interest application functionality

#### 💡 Feature Additions
- [ ] Online banking portal integration
- [ ] Mobile app compatibility
- [ ] Multiple currency support
- [ ] Loan management system
- [ ] Credit card integration
- [ ] Cheque management
- [ ] Email notifications
- [ ] PDF report generation

#### 🔒 Security Improvements
- [ ] Password hashing (bcrypt/Argon2)
- [ ] Session management
- [ ] Role-based access control
- [ ] Audit logging
- [ ] Encryption for sensitive data
- [ ] Two-factor authentication

#### ⚡ Performance Optimizations
- [ ] Use `unordered_map` for O(1) lookups
- [ ] Implement caching
- [ ] Smart pointers for memory management
- [ ] Multi-threading support
- [ ] Database connection pooling

---

## 🤝 Contributing

### How to Contribute

1. **Fork the repository**
```bash
git clone https://github.com/yourusername/bank-management-system.git
cd bank-management-system
```

2. **Create a feature branch**
```bash
git checkout -b feature/amazing-feature
```

3. **Make your changes**
- Follow C++17 standards
- Maintain OOP principles
- Add comments for complex logic

4. **Commit your changes**
```bash
git commit -m "Add some amazing feature"
git push origin feature/amazing-feature
```

5. **Open a Pull Request**
- Describe your changes clearly
- Reference any related issues
- Include test cases if applicable

---

## 📊 Project Statistics

| Metric | Value |
|--------|-------|
| **Lines of Code** | ~800 |
| **Classes** | 5 |
| **Features** | 18 |
| **Supported Brands** | 6 |
| **Islamic Features** | 3 |
| **Complexity** | O(n) - Linear |

---

## 📞 Support & Contact

- **Project Maintainer**: yashfa waseem
- **Email**: yashfawaseem2006@gmail.com

## 🙏 Acknowledgments

- Islamic principles for charity calculations
- Pakistani brands for discount schemes
- Open-source community for inspiration
- All contributors and testers


**Built with ❤️ and C++17**
