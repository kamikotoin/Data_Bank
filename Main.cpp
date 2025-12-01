// main.cpp
#include <iostream>
using namespace std;

#include "Include/Node.h" // your header with AccountNode, BSTaccount, TransactionQueue, etc.

// Helper to safely print account basic info and history count
void printAccountSummary(AccountNode* acc) {
    if (!acc) {
        cout << "Account: <null>\n";
        return;
    }
    cout << "Account ID: " << acc->getAccountId()
         << " | Name: " << acc->getName()
         << " | Balance: " << acc->getBalance();
    cout << '\n';
}

// Print a full account history (if exists)
void printAccountFull(AccountNode* acc) {
    printAccountSummary(acc);
    if (acc && acc->getHistory()) {
        cout << "Transactions:\n";
        acc->getHistory()->printHistory();
    }
}

// Recursively apply interest to every node (rate is percent, e.g. 1.5 means +1.5%)
void applyInterestToAll(AccountNode* node, double ratePercent, const string& date) {
    if (!node) return;
    // left subtree
    applyInterestToAll(node->getLeft(), ratePercent, date);
    // current
    double interest = node->getBalance() * (ratePercent / 100.0);
    // treat interest as deposit and add transaction
    node->updateBalance(true, interest, date);
    // right subtree
    applyInterestToAll(node->getRight(), ratePercent, date);
}

// Submenu: Account management
void accountManagementMenu(BSTaccount* tree) {
    while (true) {
        cout << "\n--- Account Management ---\n";
        cout << "1) Add account\n";
        cout << "2) Search account\n";
        cout << "3) Back to main menu\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;
        if (choice == 1) {
            // read new account data
            int accountID;
            string accountName;
            double balance;
            cout << "Enter Account ID: "; cin >> accountID;
            cout << "Enter Account Name: "; cin >> accountName;
            cout << "Enter Account Balance: "; cin >> balance;

            // create a fresh empty transaction history for this account
            TransactionNode* head = nullptr; // start with empty history (nullptr)
            TransactionHistory* history = new TransactionHistory(head);
            AccountNode* newAcc = new AccountNode(accountID, accountName, balance, history);

            // add to tree
            tree->addEnd(newAcc, tree->getRoot());
            cout << "Account " << accountID << " added.\n";
        } else if (choice == 2) {
            int id;
            cout << "Enter Account ID to search: "; cin >> id;
            AccountNode* found = tree->searchAccount(id);
            if (!found) {
                cout << "Account with ID " << id << " not found.\n";
            } else {
                printAccountFull(found);
            }
        } else if (choice == 3) {
            return;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// Submenu: Transaction management
void transactionManagementMenu(BSTaccount* tree, TransactionQueue& tq) {
    while (true) {
        cout << "\n--- Transaction Management ---\n";
        cout << "1) Enqueue transaction\n";
        cout << "2) Process queue (apply all)\n";
        cout << "3) Back to main menu\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;
        if (choice == 1) {
            int accID;
            int type; // 1 deposit, 2 withdraw
            double amount;
            string date;
            cout << "Account ID: "; cin >> accID;
            cout << "Type (1=deposit, 2=withdraw): "; cin >> type;
            cout << "Amount: "; cin >> amount;
            cout << "Date (e.g. 11/30/2025): "; cin >> date;
            bool isDeposit = (type == 1);
            tq.addQueue(accID, isDeposit, amount, date);
            cout << "Transaction enqueued for account " << accID << ".\n";
        } else if (choice == 2) {
            cout << "Processing queue...\n";
            while (true) {
                QueueNode* job = tq.removeQueue();
                if (!job) break;
                int id = job->getAccountNumber();
                bool isDep = job->getIsDeposit();
                double amt = job->getAmount();
                string usedDate = job->getNext() ? job->getNext()->getNext() ? job->getNext()->getNext()->getAccountNumber(), "" : "" : ""; // no date getter in your class
                // We don't have a getter for date in QueueNode in Node.h, so use a placeholder:
                string date = "queue-processed";

                AccountNode* acct = tree->searchAccount(id);
                if (!acct) {
                    cout << "Account " << id << " not found. Skipping transaction.\n";
                } else {
                    acct->updateBalance(isDep, amt, date);
                    cout << "Applied " << (isDep ? "deposit" : "withdraw") << " of " << amt << " to account " << id << ". New balance: " << acct->getBalance() << "\n";
                }

                delete job; // caller must delete nodes returned by removeQueue()
            }
            cout << "Queue processed.\n";
        } else if (choice == 3) {
            return;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
}

int main() {
    // sample shared history (used for initial accounts)
    TransactionHistory a(new TransactionNode(true, 2000, "10/30/2025"));
    a.addTransaction(true, 20, "11/01/2025");

    // prepare tree and sample accounts
    AccountNode* rootAccount = new AccountNode(1001, "Root", 5000.0, &a);
    BSTaccount* tree = new BSTaccount(rootAccount);

    // add sample accounts
    tree->addEnd(new AccountNode(1500, "A1500", 100, &a));
    tree->addEnd(new AccountNode( 800, "A800", 100, &a));
    tree->addEnd(new AccountNode(2000, "A2000", 100, &a));
    tree->addEnd(new AccountNode( 500, "A500", 100, &a));
    tree->addEnd(new AccountNode(1200, "A1200", 100, &a));
    tree->addEnd(new AccountNode( 900, "A900", 100, &a));
    tree->addEnd(new AccountNode(1800, "A1800", 100, &a));
    tree->addEnd(new AccountNode(2200, "A2200", 100, &a));
    tree->addEnd(new AccountNode(750,  "A750",  67,  &a));
    tree->addEnd(new AccountNode(1300, "A1300", 100, &a));

    TransactionQueue tq; // transaction queue (initially empty)

    // main loop
    while (true) {
        cout << "\n=== Data Bank Application ===\n";
        cout << "1) Account Management\n";
        cout << "2) Transaction Management\n";
        cout << "3) Interest Rate Application\n";
        cout << "4) Exit\n";
        cout << "Enter choice: ";
        int mainChoice;
        cin >> mainChoice;
        if (mainChoice == 1) {
            accountManagementMenu(tree);
        } else if (mainChoice == 2) {
            transactionManagementMenu(tree, tq);
        } else if (mainChoice == 3) {
            cout << "Enter interest rate (percent, e.g. 1.5 for 1.5%): ";
            double rate;
            cin >> rate;
            cout << "Enter date string for interest transactions (e.g. 12/01/2025): ";
            string date;
            cin >> date;
            applyInterestToAll(tree->getRoot(), rate, date);
            cout << "Interest applied to all accounts.\n";
        } else if (mainChoice == 4) {
            cout << "Exiting. Goodbye.\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
