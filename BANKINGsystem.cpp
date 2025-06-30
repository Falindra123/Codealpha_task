#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

// Transaction class
class Transaction {
public:
    string type;
    double amount;
    string timestamp;

    Transaction(string t, double a) {
        type = t;
        amount = a;
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back(); // remove newline
    }

    void display() const {
        cout << "[" << timestamp << "] " << type << ": $" << amount << endl;
    }
};

// Account class
class Account {
private:
    static int nextAccountNumber;
    int accountNumber;
    double balance;
    vector<Transaction> transactions;

public:
    Account() {
        accountNumber = ++nextAccountNumber;
        balance = 0;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
        transactions.emplace_back("Deposit", amount);
        cout << "Deposited $" << amount << " successfully.\n";
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient funds.\n";
            return false;
        }
        balance -= amount;
        transactions.emplace_back("Withdrawal", amount);
        cout << "Withdrew $" << amount << " successfully.\n";
        return true;
    }

    bool transfer(Account &toAccount, double amount) {
        if (withdraw(amount)) {
            toAccount.deposit(amount);
            transactions.emplace_back("Transfer to Acc #" + to_string(toAccount.getAccountNumber()), amount);
            return true;
        }
        return false;
    }

    void showTransactions() const {
        if (transactions.empty()) {
            cout << "No transactions yet.\n";
        } else {
            for (const auto &t : transactions)
                t.display();
        }
    }

    void displayInfo() const {
        cout << "Account Number: " << accountNumber << "\n";
        cout << "Current Balance: $" << balance << "\n";
        cout << "Transaction History:\n";
        showTransactions();
    }
};
int Account::nextAccountNumber = 1000;

// Customer class
class Customer {
public:
    string name;
    int customerID;
    Account account;

    Customer(string n, int id) : name(n), customerID(id) {}

    void displayCustomerInfo() const {
        cout << "Customer ID: " << customerID << "\n";
        cout << "Name: " << name << "\n";
        account.displayInfo();
    }
};

// Bank system
class BankSystem {
private:
    vector<Customer> customers;
    int nextCustomerID = 1;

    Customer* findCustomerByID(int id) {
        for (auto &c : customers) {
            if (c.customerID == id) return &c;
        }
        return nullptr;
    }

public:
    void createCustomer() {
        string name;
        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, name);
        Customer newCustomer(name, nextCustomerID++);
        customers.push_back(newCustomer);
        cout << "Customer created with ID: " << newCustomer.customerID << endl;
    }

    void depositToAccount() {
        int id;
        double amount;
        cout << "Enter Customer ID: ";
        cin >> id;
        Customer *cust = findCustomerByID(id);
        if (cust) {
            cout << "Enter deposit amount: $";
            cin >> amount;
            cust->account.deposit(amount);
        } else {
            cout << "Customer not found.\n";
        }
    }

    void withdrawFromAccount() {
        int id;
        double amount;
        cout << "Enter Customer ID: ";
        cin >> id;
        Customer *cust = findCustomerByID(id);
        if (cust) {
            cout << "Enter withdrawal amount: $";
            cin >> amount;
            cust->account.withdraw(amount);
        } else {
            cout << "Customer not found.\n";
        }
    }

    void transferFunds() {
        int fromID, toID;
        double amount;
        cout << "Enter sender Customer ID: ";
        cin >> fromID;
        cout << "Enter receiver Customer ID: ";
        cin >> toID;
        cout << "Enter transfer amount: $";
        cin >> amount;
        Customer *fromCust = findCustomerByID(fromID);
        Customer *toCust = findCustomerByID(toID);
        if (fromCust && toCust) {
            if (fromCust->account.transfer(toCust->account, amount))
                cout << "Transfer successful.\n";
        } else {
            cout << "One or both customers not found.\n";
        }
    }

    void viewAccountInfo() {
        int id;
        cout << "Enter Customer ID: ";
        cin >> id;
        Customer *cust = findCustomerByID(id);
        if (cust) {
            cust->displayCustomerInfo();
        } else {
            cout << "Customer not found.\n";
        }
    }
};

// Main driver
int main() {
    BankSystem bank;
    int choice;
    do {
        cout << "\n--- Banking System Menu ---\n";
        cout << "1. Create Customer\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Transfer Funds\n";
        cout << "5. View Account Info\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: bank.createCustomer(); break;
            case 2: bank.depositToAccount(); break;
            case 3: bank.withdrawFromAccount(); break;
            case 4: bank.transferFunds(); break;
            case 5: bank.viewAccountInfo(); break;
            case 6: cout << "Thank you!\n"; break;
            default: cout << "Invalid option.\n";
        }
    } while (choice != 6);
    return 0;
}
