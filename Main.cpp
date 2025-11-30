// main.cpp
#include <iostream>
using namespace std;

#include "Include/Node.h" 


void printTree(AccountNode* node, int depth = 0) {
    if (!node) return;
    printTree(node->getRight(), depth + 1);
    for (int i = 0; i < depth; ++i) cout << "    ";
    cout << node->getAccountId() << " (Bal: " << node->getBalance() << ")\n";
    printTree(node->getLeft(), depth + 1);
}

// In-order traversal -> print account IDs and check sorted order
// lastSeen should be initialized to a very small value by caller (e.g., -2147483648)
// ok is a reference boolean that stays true while sorted, becomes false if order breaks
void inorderCheckAndPrint(AccountNode* node, int &lastSeen, bool &ok) {
    if (!node) return;
    inorderCheckAndPrint(node->getLeft(), lastSeen, ok);

    // visit node
    cout << node->getAccountId() << " ";
    if (lastSeen != INT_MIN) {
        if (node->getAccountId() < lastSeen) ok = false;
    }
    lastSeen = node->getAccountId();

    inorderCheckAndPrint(node->getRight(), lastSeen, ok);
}

// Helper to safely print account info given pointer (handles nullptr)
void printAccount(AccountNode* acc) {
    if (!acc) {
        cout << "Account: <null>\n";
        return;
    }
    cout << "Account ID: " << acc->getAccountId()
         << " | Name: " << acc->getName()
         << " | Balance: " << acc->getBalance()
         << " | #Tx: ";
    if (acc->getHistory())
        cout << acc->getHistory()->countTransactions();
    else
        cout << 0;
    cout << '\n';
}

int main() {
    cout << "=== BST + Queue + Transaction History test harness ===\n\n";

    // create a shared transaction history object `a`
    TransactionHistory a(new TransactionNode(true, 2000, "10/30/2025"));
    a.addTransaction(true, 20, "11/01/2025");
    a.addTransaction(false, 40, "11/05/2025");
    a.addTransaction(true, 50, "11/12/2025");
    a.addTransaction(false, 100, "11/17/2025");
    a.addTransaction(true, 999, "11/26/2025");

    cout << "Initial shared history entries (a):\n";
    a.printHistory();
    cout << "----\n\n";

    // ROOT account and tree
    AccountNode* rootAccount = new AccountNode(1001, "Root", 5000.0, &a);
    BSTaccount* tree = new BSTaccount(rootAccount);

    // Create additional accounts (mixed ids)
    AccountNode* acc1 = new AccountNode(1500, "A1500", 100, &a);  // right
    AccountNode* acc2 = new AccountNode( 800, "A800", 100, &a);   // left
    AccountNode* acc3 = new AccountNode(2000, "A2000", 100, &a);  // right-right
    AccountNode* acc4 = new AccountNode( 500, "A500", 100, &a);   // left-left
    AccountNode* acc5 = new AccountNode(1200, "A1200", 100, &a);  // right-left
    AccountNode* acc6 = new AccountNode( 900, "A900", 100, &a);   // left-right
    AccountNode* acc7 = new AccountNode(1800, "A1800", 100, &a);  // right-right-left
    AccountNode* acc8 = new AccountNode(2200, "A2200", 100, &a);  // right-right-right
    AccountNode* acc9 = new AccountNode(750, "A750", 67, &a);     // left-left-right
    AccountNode* acc10 = new AccountNode(1300, "A1300", 100, &a); // right-left-right

    // Insert into the BST
    cout << "Inserting accounts...\n";
    tree->addEnd(acc1);
    tree->addEnd(acc2);
    tree->addEnd(acc3);
    tree->addEnd(acc4);
    tree->addEnd(acc5);
    tree->addEnd(acc6);
    tree->addEnd(acc7);
    tree->addEnd(acc8);
    tree->addEnd(acc9);
    tree->addEnd(acc10);
    cout << "Insertions done.\n\n";

    // 1) Visual check - print rotated tree
    cout << "Rotated BST (right higher, left lower):\n";
    printTree(tree->getRoot());
    cout << "\n----\n\n";

    // 2) In-order traversal check (printed, and checks sorted order)
    cout << "In-order traversal (should be sorted ascending):\n";
    int lastSeen = INT_MIN; // sentinel for "no last value yet"
    bool ok = true;
    inorderCheckAndPrint(tree->getRoot(), lastSeen, ok);
    cout << "\nIn-order sorted? " << (ok ? "YES" : "NO") << "\n";
    cout << "\n----\n";

    // 3) Test searching existing and non-existing accounts safely
    cout << "Search tests:\n";
    int tests[] = {1001, 750, 1300, 9999};
    for (int i = 0; i < 4; ++i) {
        int t = tests[i];
        AccountNode* res = tree->searchAccount(t);
        cout << "Searching " << t << ": ";
        if (res) {
            cout << "FOUND -> ";
            printAccount(res);
        } else {
            cout << "NOT FOUND\n";
        }
    }
    cout << "\n----\n";

    // 4) Test updateBalance & transaction history for a specific account (750)
    cout << "Balance & history test for account 750:\n";
    AccountNode* a750 = tree->searchAccount(750);
    if (a750) {
        cout << "Before: "; printAccount(a750);
        // add a deposit
        a750->updateBalance(true, 2.0, "11/26/2025"); // deposit 2
        cout << "After deposit 2.0: "; printAccount(a750);
        cout << "History for account 750:\n";
        if (a750->getHistory()) a750->getHistory()->printHistory();
    } else {
        cout << "Account 750 not found (unexpected)\n";
    }
    cout << "\n----\n";

    // 5) Queue tests - create a TransactionQueue and enqueue a few actions
    TransactionQueue tq;
    cout << "Enqueueing transactions:\n";
    tq.addQueue(1001, true, 500, "11/01/2025");   // deposit to root
    tq.addQueue(1500, false, 200, "11/02/2025");  // withdrawal from 1500
    tq.addQueue(800, true, 300, "11/03/2025");    // deposit to 800
    tq.addQueue(9999, true, 50, "11/04/2025");    // non-existing account (should be handled)
    cout << "Queued 4 transactions.\n\n";

    // 6) Process queue: dequeue, locate account by BST search, apply updateBalance, append history
    cout << "Processing queue:\n";
    while (true) {
        QueueNode* job = tq.removeQueue();
        if (!job) break; // queue empty
        int id = job->getAccountNumber();
        bool isDep = job->getIsDeposit();
        double amt = job->getAmount();
        cout << "Processing job -> Account: " << id << " | " << (isDep ? "Deposit" : "Withdraw") << " | " << amt << "\n";

        AccountNode* acct = tree->searchAccount(id);
        if (!acct) {
            cout << "  -> Account " << id << " not found. Skipping.\n";
        } else {
            cout << "  -> Before: "; printAccount(acct);
            acct->updateBalance(isDep, amt, "queue-processed");
            cout << "  -> After:  "; printAccount(acct);
        }

        // caller must delete returned QueueNode (your queue doesn't delete it)
        delete job;
    }
    cout << "Queue processing finished.\n\n";

    // 7) Final tree print to show end balances
    cout << "Final tree (balances updated):\n";
    printTree(tree->getRoot());
    cout << "\n----\n";

    // 8) Print history for an account that was queued (1001)
    cout << "Transaction history for account 1001 (root):\n";
    AccountNode* rootAcc = tree->searchAccount(1001);
    if (rootAcc && rootAcc->getHistory()) rootAcc->getHistory()->printHistory();
    else cout << "No history for 1001.\n";

    cout << "\nAll tests executed.\n";
    return 0;
}
