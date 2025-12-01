// main.cpp
#include <iostream>
using namespace std;

#include "Include/Node.h"

// print basic info (no countTransactions)
void printAccountSummary(AccountNode* acc) {
    if (!acc) {
        cout << "Account: <null>\n";
        return;
    }
    cout << "Account ID: " << acc->getAccountId()
         << " | Name: " << acc->getName()
         << " | Balance: " << acc->getBalance()
         << '\n';
}

void printAccountFull(AccountNode* acc) {
    printAccountSummary(acc);
    if (acc && acc->getHistory()) {
        cout << "Transactions:\n";
        acc->getHistory()->printHistory();
    }
}

// apply interest (inorder traversal) once per node
void applyInterestToAll(AccountNode* node, double ratePercent, const string& date) {
    if (!node) return;
    applyInterestToAll(node->getLeft(), ratePercent, date);
    double interest = node->getBalance() * (ratePercent / 100.0);
    node->updateBalance(true, interest, date);
    applyInterestToAll(node->getRight(), ratePercent, date);
}

// Account actions menu for a specific ID (create/delete/enqueue deposit/withdraw)
void accountActionsMenu(BSTaccount* tree, TransactionQueue& tq, int targetID, AccountNode* found) {
    while (true) {
        cout << "\nOptions for account " << targetID << ":\n";
        cout << "1) Create account (if not exists)\n";
        cout << "2) Delete account\n";
        cout << "3) Enqueue deposit to this account\n";
        cout << "4) Enqueue withdrawal from this account\n";
        cout << "5) Check transaction history\n";   // << NEW OPTION
        cout << "6) Back\n";
        cout << "Enter choice: ";

        int ch; 
        cin >> ch;

        if (ch == 1) { 
            if (found) {
                cout << "Account already exists.\n";
            } else {
                string name; 
                double bal;
                cout << "Enter name: "; cin >> name;
                cout << "Enter initial balance: "; cin >> bal;
                TransactionHistory* h = new TransactionHistory(nullptr);
                AccountNode* newAcc = new AccountNode(targetID, name, bal, h);
                tree->addEnd(newAcc, tree->getRoot());
                cout << "Account " << targetID << " created.\n";
                found = tree->searchAccount(targetID);
            }
        }

        else if (ch == 2) {
            if (!found) cout << "Account not found.\n";
            else {
                tree->deleteAccount(targetID);
                cout << "Account " << targetID << " deleted.\n";
                found = nullptr;
            }
        }

        else if (ch == 3 || ch == 4) {
            bool isDep = (ch == 3);
            double amt; 
            string date;
            cout << "Amount: "; cin >> amt;
            cout << "Date (e.g. 12/01/2025): "; cin >> date;
            tq.addQueue(targetID, isDep, amt, date);
            cout << (isDep ? "Deposit" : "Withdrawal") << " enqueued for " << targetID << ".\n";
        }

        else if (ch == 5) {  // << NEW: PRINT HISTORY
            if (!found) {
                cout << "This account does not exist.\n";
            } else {
                cout << "\n--- Transaction History for Account " << targetID << " ---\n";
                printAccountFull(found);
            }
        }

        else if (ch == 6) {
            return; // back
        }

        else {
            cout << "Invalid choice.\n";
        }
    }
}


// top-level account management: ask ID then actions
void accountManagementMenu(BSTaccount* tree, TransactionQueue& tq) {
    while (true) {
        cout << "\n--- Account Management ---\n";
        cout << "Enter account ID to manage (or 0 to back): ";
        int id; cin >> id;
        if (id == 0) return;
        AccountNode* found = tree->searchAccount(id);
        if (found) {
            cout << "Found account:\n";
            printAccountFull(found);
        } else {
            cout << "Account " << id << " not found.\n";
        }
        accountActionsMenu(tree, tq, id, found);
    }
}

// Queue Management menu: see queue, clean queue, or process queue
void queueManagementMenu(BSTaccount* tree, TransactionQueue& tq) {
    while (true) {
        cout << "\n--- Queue Management ---\n";
        cout << "1) See queue\n";
        cout << "2) Clean queue (remove all)\n";
        cout << "3) Process queue (apply transactions)\n";
        cout << "4) Back\n";
        cout << "Enter choice: ";
        int ch; cin >> ch;
        if (ch == 1) {
            tq.printQueue();
        } else if (ch == 2) {
            tq.clearQueue();
            cout << "Queue cleared.\n";
        } else if (ch == 3) {
            // process queue with withdrawal-safety checks
            cout << "Processing queue now...\n";
            while (true) {
                QueueNode* job = tq.removeQueue();
                if (!job) break;
                int id = job->getAccountNumber();
                bool isDep = job->getIsDeposit();
                double amt = job->getAmount();
                string date = job->getDate();
                AccountNode* acct = tree->searchAccount(id);
                if (!acct) {
                    cout << "Account " << id << " not found. Skipping transaction.\n";
                } else {
                    if (!isDep && acct->getBalance() < amt) {
                        cout << "Insufficient funds for account " << id << " (balance: " << acct->getBalance() << ", requested: " << amt << "). Skipping withdrawal.\n";
                    } else {
                        acct->updateBalance(isDep, amt, date);
                        cout << "Applied " << (isDep ? "deposit" : "withdrawal") << " of " << amt << " to account " << id << ". New balance: " << acct->getBalance() << "\n";
                    }
                }
                delete job;
            }
            cout << "Queue processing finished.\n";
        } else if (ch == 4) {
            return;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

int main() {
    // Create tree and sample accounts — each gets its own TransactionHistory
    AccountNode* rootAccount = new AccountNode(1001, "Root", 5000.0, new TransactionHistory(nullptr));
    BSTaccount* tree = new BSTaccount(rootAccount);

    tree->addEnd(new AccountNode(1500, "A1500", 100, new TransactionHistory(nullptr)));
    tree->addEnd(new AccountNode(800,  "A800",  100, new TransactionHistory(nullptr)));
    tree->addEnd(new AccountNode(2000, "A2000", 100, new TransactionHistory(nullptr)));
    tree->addEnd(new AccountNode(500,  "A500",  100, new TransactionHistory(nullptr)));
    tree->addEnd(new AccountNode(1200, "A1200", 100, new TransactionHistory(nullptr)));
    tree->addEnd(new AccountNode(900,  "A900",  100, new TransactionHistory(nullptr)));
    tree->addEnd(new AccountNode(1800, "A1800", 100, new TransactionHistory(nullptr)));
    tree->addEnd(new AccountNode(2200, "A2200", 100, new TransactionHistory(nullptr)));
    tree->addEnd(new AccountNode(750,  "A750",   67, new TransactionHistory(nullptr)));
    tree->addEnd(new AccountNode(1300, "A1300", 100, new TransactionHistory(nullptr)));

    TransactionQueue tq;

    while (true) {
        cout << "\n=== Data Bank Application ===\n";
        cout << "1) Account Management\n";
        cout << "2) Queue Management\n";
        cout << "3) Interest Rate Application\n";
        cout << "4) Exit\n";
        cout << "Enter choice: ";
        int mainChoice; cin >> mainChoice;
        if (mainChoice == 1) accountManagementMenu(tree, tq);
        else if (mainChoice == 2) queueManagementMenu(tree, tq);
        else if (mainChoice == 3) {
            cout << "Enter interest rate (percent, e.g. 1.5 for 1.5%): ";
            double rate; cin >> rate;
            cout << "Enter date string for interest transactions (e.g. 12/01/2025): ";
            string date; cin >> date;
            applyInterestToAll(tree->getRoot(), rate, date);
            cout << "Interest applied to all accounts.\n";
        }
        else if (mainChoice == 4) {
            cout << "Exiting. Goodbye.\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
