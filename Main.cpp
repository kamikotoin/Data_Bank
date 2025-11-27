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
	AccountNode* rootAccount = new AccountNode(1001, "MainAccount", 5000.0, &a);
	BSTaccount* root= new BSTaccount(rootAccount);
	AccountNode* acc1 = new AccountNode(1003, "right", 5000.0, &a);
	AccountNode* acc2 = new AccountNode(999, "left", 5000.0, &a);
	
	root->addEnd(acc1);
	root->addEnd(acc2);

	addAccount(root);
}
