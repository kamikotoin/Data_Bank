#include <iostream>
using namespace std;
#include "Include/Node.h"


int main() {
	TransactionHistory a(
		new TransactionNode("Deposit", 2000, "10/30/2025")
	);

	a.addTransaction("Deposit", 20, "11/01/2025");
	a.addTransaction("Withdrawal", 40, "11/05/2025");
	a.addTransaction("Deposit", 50, "11/12/2025");
	a.addTransaction("Withdrawal", 100, "11/17/2025");
	a.addTransaction("Deposit", 999, "11/26/2025");

	// cout << "Number of Transactions: " << a.countTransactions() << endl;

	// a.printHistory();
// Root
AccountNode* rootAccount = new AccountNode(1001, "Root", 5000.0, &a);
BSTaccount* tree = new BSTaccount(rootAccount);

// 10 more accounts (IDs intentionally mixed)
AccountNode* acc1 = new AccountNode(1500, "A1500", 100, &a);  // right
AccountNode* acc2 = new AccountNode( 800, "A800", 100, &a);   // left
AccountNode* acc3 = new AccountNode(2000, "A2000", 100, &a);  // right-right
AccountNode* acc4 = new AccountNode( 500, "A500", 100, &a);   // left-left
AccountNode* acc5 = new AccountNode(1200, "A1200", 100, &a);  // right-left
AccountNode* acc6 = new AccountNode( 900, "A900", 100, &a);   // left-right
AccountNode* acc7 = new AccountNode(1800, "A1800", 100, &a);  // right-right-left
AccountNode* acc8 = new AccountNode(2200, "A2200", 100, &a);  // right-right-right
AccountNode* acc9 = new AccountNode( 750, "A750", 100212, &a);   // left-left-right
AccountNode* acc10 = new AccountNode(1300, "A1300", 100, &a); // right-left-right

// Insert into the BST
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

AccountNode* result = tree->searchAccount(228);

if (result == nullptr) {
		cout<<"such account does not exist! Try another number";
} else {
    cout << result->getBalance();
}

	// addAccount(root);
}
