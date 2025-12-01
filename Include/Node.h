#pragma once
#include <iostream>
#include <string>
using namespace std;

// Transaction Node (Linked List)
class TransactionNode {
public:
    bool isDeposit;
    double amount = 0.0;
    string date = "01/01/1980";

    TransactionNode* next = nullptr;

    TransactionNode() {}
    TransactionNode(bool isDeposit, double amount, string date) {
        this->isDeposit = isDeposit;
        this->amount = amount;
        this->date = date;
    }
};

// TransactionHistory: minimal manager (no countTransactions here)
class TransactionHistory {
private:
    TransactionNode* head;
public:
    TransactionHistory(TransactionNode* head) {
        this->head = head;
    }

    void addTransaction(bool isDeposit, double amount, string date) {
        TransactionNode* newNode = new TransactionNode(isDeposit, amount, date);
        if (head == nullptr) {
            head = newNode;
        } else {
            TransactionNode* last = head;
            while (last->next != nullptr) last = last->next;
            last->next = newNode;
        }
    }

    void printHistory() {
        TransactionNode* current = head;
        if (!current) {
            cout << "No transactions." << endl;
            return;
        }
        while (current != nullptr) {
            cout << "Type: " << (current->isDeposit ? "Deposit" : "Withdraw") << endl;
            cout << "Amount: " << current->amount << endl;
            cout << "Date: " << current->date << "\n\n";
            current = current->next;
        }
    }

    // Expose head if you ever need to iterate from outside (optional)
    TransactionNode* getHead() const { return head; }
};

class AccountNode {
private:
    int accountID;
    string accountName;
    double balance;

    TransactionHistory* transactions;
    AccountNode* left = nullptr;
    AccountNode* right = nullptr;

public:
    AccountNode(int accountID, string accountName, double balance, TransactionHistory* transactions) {
        this->accountID = accountID;
        this->accountName = accountName;
        this->balance = balance;
        this->transactions = transactions;
    }

    void updateBalance(bool isDeposit, double amount, string date) {
        if (isDeposit) balance += amount;
        else balance -= amount;
        if (transactions) transactions->addTransaction(isDeposit, amount, date);
    }

    // Used by delete (two-children case): copy data fields
    void replaceDataFrom(const AccountNode* other) {
        this->accountID = other->accountID;
        this->accountName = other->accountName;
        this->balance = other->balance;
        this->transactions = other->transactions;
    }

    void setRight(AccountNode* right) { this->right = right; }
    void setLeft(AccountNode* left) { this->left = left; }

    int getAccountId() const { return accountID; }
    string getName() const { return accountName; }
    double getBalance() const { return balance; }
    TransactionHistory* getHistory() const { return transactions; }
    AccountNode* getRight() const { return right; }
    AccountNode* getLeft() const { return left; }
};

class BSTaccount {
    AccountNode* root = nullptr;
public:
    AccountNode* getRoot() { return root; }

    BSTaccount(AccountNode* root) {
        this->root = root;
    }

    // Insert (recursive). If tree empty, set root.
    void addEnd(AccountNode* newAccount, AccountNode* current = nullptr) {
        if (current == nullptr) current = root;
        if (current == nullptr) { // empty tree
            root = newAccount;
            return;
        }
        if (newAccount->getAccountId() == current->getAccountId()) {
            cout << "Same ID cannot exist, please create another account, or change ID" << endl;
            return;
        }
        if (current->getAccountId() < newAccount->getAccountId()) {
            if (current->getRight() == nullptr) current->setRight(newAccount);
            else addEnd(newAccount, current->getRight());
        } else {
            if (current->getLeft() == nullptr) current->setLeft(newAccount);
            else addEnd(newAccount, current->getLeft());
        }
    }

    AccountNode* searchAccount(int searchAccountNumber, AccountNode* current = nullptr) {
        if (current == nullptr) current = root;
        if (current == nullptr) return nullptr;
        if (searchAccountNumber == current->getAccountId()) return current;
        else if (searchAccountNumber > current->getAccountId() && current->getRight() != nullptr)
            return searchAccount(searchAccountNumber, current->getRight());
        else if (searchAccountNumber < current->getAccountId() && current->getLeft() != nullptr)
            return searchAccount(searchAccountNumber, current->getLeft());
        else return nullptr;
    }

    AccountNode* leftMostChild(AccountNode* current) {
        if (current == nullptr) return nullptr;
        while (current->getLeft() != nullptr) current = current->getLeft();
        return current;
    }

    // removeNode returns new subtree root after deletion
    AccountNode* removeNode(AccountNode* node, int accountID) {
        if (node == nullptr) return nullptr;

        if (accountID < node->getAccountId()) {
            AccountNode* newLeft = removeNode(node->getLeft(), accountID);
            node->setLeft(newLeft);
            return node;
        } else if (accountID > node->getAccountId()) {
            AccountNode* newRight = removeNode(node->getRight(), accountID);
            node->setRight(newRight);
            return node;
        } else {
            // found node
            AccountNode* left = node->getLeft();
            AccountNode* right = node->getRight();

            // case 1: no children
            if (left == nullptr && right == nullptr) {
                delete node; // DO NOT delete transactions pointer here
                return nullptr;
            }
            // case 2: one child
            if (left == nullptr) {
                AccountNode* tmp = right;
                delete node;
                return tmp;
            }
            if (right == nullptr) {
                AccountNode* tmp = left;
                delete node;
                return tmp;
            }
            // case 3: two children
            AccountNode* succ = leftMostChild(right);
            if (succ != nullptr) {
                node->replaceDataFrom(succ);
                AccountNode* newRight = removeNode(right, succ->getAccountId());
                node->setRight(newRight);
            }
            return node;
        }
    }

    void deleteAccount(int deleteAccountID) {
        root = removeNode(root, deleteAccountID);
    }
};

// helper addAccount (uses new empty history)
void addAccount(BSTaccount* root) {
    int accountID = 0;
    string accountName = "NewAccount";
    double balance = 0.0;
    cout << "Enter Account ID: ";
    cin >> accountID;
    cout << "Enter Account Name: ";
    cin >> accountName;
    cout << "Enter Account Balance: ";
    cin >> balance;

    TransactionNode* head = nullptr;
    TransactionHistory* history = new TransactionHistory(head);
    AccountNode* newAccount = new AccountNode(accountID, accountName, balance, history);

    root->addEnd(newAccount, root->getRoot());
}

// Queue Node and TransactionQueue
class QueueNode {
private:
    int accountID;
    bool isDeposit;
    double amount;
    string date;
    QueueNode* next;
public:
    QueueNode(int accountID, bool isDeposit, double amount, string date, QueueNode* next = nullptr) {
        this->accountID = accountID;
        this->isDeposit = isDeposit;
        this->amount = amount;
        this->date = date;
        this->next = next;
    }
    int getAccountNumber() const { return accountID; }
    bool getIsDeposit() const { return isDeposit; }
    double getAmount() const { return amount; }
    string getDate() const { return date; }
    QueueNode* getNext() const { return next; }
    void setNext(QueueNode* next) { this->next = next; }
};

class TransactionQueue {
private:
    QueueNode* front = nullptr;
    QueueNode* rear = nullptr;
public:
    void addQueue(int accountID, bool isDeposit, double amount, string date) {
        QueueNode* newNode = new QueueNode(accountID, isDeposit, amount, date);
        if (rear == nullptr) {
            front = rear = newNode;
            return;
        } else {
            rear->setNext(newNode);
            rear = newNode;
        }
    }
    QueueNode* removeQueue() {
        if (front == nullptr) return nullptr;
        if (front == rear) {
            QueueNode* tmp = front;
            front = rear = nullptr;
            return tmp;
        } else {
            QueueNode* tmp = front;
            front = front->getNext();
            return tmp;
        }
    }
    void printQueue() {
        if (front == nullptr) {
            cout << "Queue is empty.\n";
            return;
        }
        QueueNode* cur = front;
        cout << "Queue contents (front -> ... -> rear):\n";
        while (cur != nullptr) {
            cout << "  Account: " << cur->getAccountNumber()
                 << " | " << (cur->getIsDeposit() ? "Deposit" : "Withdraw")
                 << " | " << cur->getAmount()
                 << " | Date: " << cur->getDate()
                 << "\n";
            cur = cur->getNext();
        }
    }
    void clearQueue() {
        while (front != nullptr) {
            QueueNode* tmp = removeQueue();
            delete tmp;
        }
    }
};
