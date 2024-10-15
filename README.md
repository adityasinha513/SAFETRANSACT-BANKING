# SafeTransact Banking System

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Usage](#usage)

  ## Introduction

**SafeTransact Banking System** is a C++ console-based application designed to simulate a secure and reliable banking environment using Object-Oriented Programming (OOP) principles. This project demonstrates the implementation of key OOP concepts such as encapsulation, inheritance, polymorphism, and robust error handling to ensure data protection, modularity, and system stability.

## Features

- **Data Protection**: Secure and controlled access to banking data through encapsulation.
- **Modular Design**: Scalable architecture using inheritance and polymorphism.
- **Customer Management**: Manage customer details and associate multiple accounts.
- **Account Management**: Handle various account types including Savings, Checking, and Loan accounts.
- **Transaction Processing**: Perform deposits, withdrawals, and transfers with robust error handling.
- **Transaction History**: Maintain detailed records of all transactions for each account.
- **User Authentication**: Secure login mechanism using usernames and passwords.
- **Loan Processing**: Manage loan accounts with interest calculations and repayment handling.
- **Account Statements**: Generate and view account statements with transaction history.

## Technologies Used

- **Programming Language**: C++
- **Libraries**: Standard C++ Libraries (`iostream`, `string`, `vector`, `stdexcept`, `memory`, `ctime`, `iomanip`)
- **Development Environment**: Any C++ compatible compiler (e.g., `g++`)
## Usage

Upon running the program, you will be greeted with a list of existing customers and their accounts. The program includes two predefined customers for demonstration:

### Predefined Customers

1. **Alice Smith**
    - **Username**: `alice`
    - **Password**: `password123`
    - **Accounts**:
        - Savings Account (`SA1001`) with a balance of $5,000 and an interest rate of 3%.
        - Checking Account (`CA1001`) with a balance of $2,000 and an overdraft limit of $500.

2. **Bob Johnson**
    - **Username**: `bob`
    - **Password**: `securepwd`
    - **Accounts**:
        - Savings Account (`SA2001`) with a balance of $3,000 and an interest rate of 2%.
        - Loan Account (`LA2001`) with a loan amount of $10,000 and an interest rate of 5%.

### Logging In

1. **Enter Username and Password**

    When prompted, enter the username and password of one of the predefined customers.

    ```plaintext
    Login to your account
    Username: alice
    Password: password123
    ```

2. **Access the Menu**

    After successful authentication, you will be presented with a menu to perform various operations:

    ```plaintext
    --- Menu ---
    1. View Accounts
    2. Deposit Funds
    3. Withdraw Funds
    4. Transfer Funds
    5. View Transaction History
    6. Apply Interest (Savings Accounts)
    7. Process Loan Payment (Loan Accounts)
    8. Exit
    Enter your choice:
    ```

3. **Perform Operations**

    - **View Accounts**: Displays all accounts associated with the logged-in customer.
    - **Deposit Funds**: Deposit money into a specified account.
    - **Withdraw Funds**: Withdraw money from a specified account.
    - **Transfer Funds**: Transfer money between two accounts within the bank.
    - **View Transaction History**: View all transactions performed on a specified account.
    - **Apply Interest**: Apply interest to all Savings Accounts associated with the customer.
    - **Process Loan Payment**: Make repayments towards a Loan Account.
    - **Exit**: Exit the banking system.

### Example Operations

- **Depositing Funds**

    ```plaintext
    Enter Account Number to Deposit Into: SA1001
    Enter Amount to Deposit: 500
    Deposited $500.00 successfully.
    ```

- **Withdrawing Funds**

    ```plaintext
    Enter Account Number to Withdraw From: CA1001
    Enter Amount to Withdraw: 1000
    Withdrew $1000.00 successfully.
    ```

- **Transferring Funds**

    ```plaintext
    Enter Source Account Number: SA1001
    Enter Destination Account Number: CA1001
    Enter Amount to Transfer: 200
    Transferred $200.00 from SA1001 to CA1001 successfully.
    ```

- **Viewing Transaction History**

    ```plaintext
    Enter Account Number to View History: SA1001
    Transaction History for Savings Account SA1001:
    2024-10-15 - Deposit of $500.00 - Deposit
    2024-10-15 - Withdrawal of $100.00 - Withdrawal
    2024-10-15 - Transfer of $200.00 - Transfer
    ```
