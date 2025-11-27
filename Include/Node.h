#pragma once
#include <iostream>

// Transaction Node (Linked List)
class TransactionNode {
public:
	bool iswithdrawal;
	double amount = 0.0;
	string date = "01/01/1980";

	TransactionNode* next = nullptr;

	TransactionNode() {}
	TransactionNode(bool iswithdrawal, double amount, string date) {
		this->iswithdrawal = iswithdrawal;
		this->amount = amount;
		this->date = date;
	}
};

// Transaction Node (Linked List) Manager
class TransactionHistory {
private:
	TransactionNode* head;
public:
	TransactionHistory(TransactionNode* head) {
		this->head = head;
	}

	// append for linked list basically
	void addTransaction( bool iswithdrawal, double amount, string date) {
		/* TODO: add check for type; should be only "withdrawal" or "deposit"
		 *       add a simple check for date too, maybe only one format
		 */
		TransactionNode* newNode = new TransactionNode(iswithdrawal, amount, date);
		if (head == nullptr)
			head = newNode;
		else {
			TransactionNode* last = head;

			while (last->next != nullptr) {
				last = last->next;
			}
			last->next = newNode;
		}
	}
	// goes through every element of the linked list and prints contents of nodes
	void printHistory() {
		TransactionNode* current = head;
		if (!current) {
			cout << "No transactions." << endl;
			return;
		}

		while (current != nullptr) {
			cout << "Type: " << current->iswithdrawal << endl;
			cout << "Amount: " << current->amount << endl;
			cout << "Date: " << current->date << "\n\n";
			current = current->next;
		}
	}
	// goes through every element of the linked list and counts nodes
	int countTransactions() {
		int counter = 0;
		TransactionNode* current = head;

		while (current->next != nullptr) {
			counter++;
			current = current->next;
		};
		return counter;
	}
};

class AccountNode {
private:
	int accountNumber;
	string accountName;
	double balance;

	TransactionHistory* transactions;
	AccountNode* left=nullptr;
	AccountNode* right=nullptr;

	public:
	AccountNode(int accountNumber, string accountName, double balance, TransactionHistory* transactions) {
		this->accountNumber = accountNumber;
		this->accountName = accountName;
		this->balance = balance;
		this->transactions = transactions;
	}
	void setRight(AccountNode* right){ this->right = right; }
    void setLeft(AccountNode* left){ this->left = left; }

	int getAccountId() const { return accountNumber; }
    string getName() const { return accountName; }
    double getBalance() const { return balance; }
    TransactionHistory* getHistory() const { return transactions; }
    AccountNode* getRight() const { return right; }
    AccountNode* getLeft() const { return left; }
};

class BSTaccount {
	AccountNode* root;
public:
	AccountNode* getRoot(){ return root; }

	BSTaccount(AccountNode* root) {
		this->root = root;
	}
	// Recursive add function for inserting a new account into the BST
	void addEnd(AccountNode* newAccount, AccountNode* current = nullptr){
		
		if(root->getAccountId()<newAccount->getAccountId()){
			if(root->getRight()==nullptr){
				root->setRight(newAccount);
			}else{
				addEnd(newAccount, root->getRight());
			}
		}else{
			if(root->getLeft()==nullptr){
				root->setLeft(newAccount);
			}else{
				addEnd(newAccount, root->getRight());
			}
		}
	}
	// Recursive search function for finding an account by account number
	AccountNode* searchAccount(int searchAccountNumber, AccountNode* current){
		if(searchAccountNumber==current->getAccountId()){
			return current;
	}else if(current->getRight()!=nullptr&&searchAccountNumber>current->getAccountId()){
			return searchAccount(searchAccountNumber,current->getRight());
	}else if(current->getLeft()!=nullptr&&searchAccountNumber<current->getAccountId()){
			return searchAccount(searchAccountNumber,current->getLeft());
	}else {
		return nullptr;
	}
	}
};


// Function to create and add a new account to the BST
void addAccount(BSTaccount* root){
    int accountNumber = 0;
    string accountName = "NewAccount";
    double balance = 0.0;
	cout<<"Enter Account ID";
	cin >> accountNumber;
	cout<<"Enter Account Name";
	cin >> accountName;
	cout<<"Enter Account Balance";
	cin >> balance;

    TransactionNode* head = new TransactionNode();
    TransactionHistory* history = new TransactionHistory(head);
    AccountNode* newAccount = new AccountNode(accountNumber,accountName,balance,history);

    root->addEnd(newAccount, root->getRoot());
};

class QueueNode {
private:
	int accountNumber;
	string type;
	double amount;
};

