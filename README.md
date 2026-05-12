
---

# Bank Account Management System

## 📌 Overview
This program is a **random-access file management system** written in C. It simulates a simple bank account database where each account record is stored in a binary file (`credit.dat`). The program allows users to:
- Create new accounts
- Update existing accounts
- Delete accounts
- Display all accounts
- Export accounts to a text file
- Log transactions to a separate file (`transactions.txt`)
- View transaction history

It demonstrates **random file access** using `fseek`, `fread`, and `fwrite`.

---

## ⚙️ Features
- **Create new account**: Add a customer record with account number, name, and balance.
- **Update account**: Apply charges or payments to an existing account.
- **Delete account**: Remove a customer record by overwriting it with a blank record.
- **Display all accounts**: Show all active accounts directly in the console.
- **Export accounts**: Save all accounts into a formatted text file (`accounts.txt`) for printing.
- **Transaction logging**: Every update, creation, or deletion is logged in `transactions.txt`.
- **View transaction history**: Display all logged transactions.

---

## 🗂️ File Structure
- **`credit.dat`** → Binary file storing account records (`struct clientData`).
- **`accounts.txt`** → Text file containing formatted account data for printing.
- **`transactions.txt`** → Text file logging all account transactions.

---

## 📖 Data Structure
Each account is represented by the following structure:
```c
struct clientData {
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance
};
```

---

## ▶️ How to Run
1. **Compile the program**:
   ```bash
   gcc bank.c -o bank
   ```
2. **Run the program**:
   ```bash
   ./bank
   ```
3. **Menu Options**:
   ```
   1 - Export accounts to accounts.txt
   2 - Update an account
   3 - Add a new account
   4 - Delete an account
   5 - Exit program
   6 - Display all accounts
   7 - View transaction history
   ```

---

## 📝 Example Usage
- **Add new account** → Enter account number, last name, first name, and balance.
- **Update account** → Enter account number, then a transaction amount (positive for charges, negative for payments).
- **Delete account** → Enter account number to remove.
- **View transactions** → Shows all actions performed with timestamps.

---

## 🚀 Notes
- The program supports up to **100 accounts** (account numbers 1–100).
- Deleted accounts are replaced with a blank record (`acctNum = 0`).
- Transactions are logged with details: account number, action type, amount, and new balance.

---

## 📌 Learning Highlights
This project demonstrates:
- Random file access with `fseek`
- Binary file I/O (`fread`, `fwrite`)
- Structured data storage
- Logging and reporting
- Menu-driven console applications in C

---
