#pragma once
#include <iostream>

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

// Transaction Node (Linked List) Manager
class TransactionHistory {
private:
	TransactionNode* head;
public:
	TransactionHistory(TransactionNode* head) {
		this->head = head;
	}

	// append for linked list basically
	void addTransaction( bool isDeposit, double amount, string date) {
		/* TODO: True=Deposit, False=Withdrawal
		 */
		TransactionNode* newNode = new TransactionNode(isDeposit, amount, date);
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
			cout << "Type: " << current->isDeposit << endl;
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
	int accountID;
	string accountName;
	double balance;

	TransactionHistory* transactions;
	AccountNode* left=nullptr;
	AccountNode* right=nullptr;

	public:
	AccountNode(int accountID, string accountName, double balance, TransactionHistory* transactions) {
		this->accountID = accountID;
		this->accountName = accountName;
		this->balance = balance;
		this->transactions = transactions;
	}
	void updateBalance(bool isDeposit, double amount, string date) {
		if(isDeposit){
			balance += amount;
		}else{
			balance -= amount;
		}
		transactions->addTransaction(isDeposit, amount, date);
	}

	void setRight(AccountNode* right){ this->right = right; }
    void setLeft(AccountNode* left){ this->left = left; }

	int getAccountId() const { return accountID; }
    string getName() const { return accountName; }
    double getBalance() const { return balance; }
    TransactionHistory* getHistory() const { return transactions; }
    AccountNode* getRight() const { return right; }
    AccountNode* getLeft() const { return left; }
};

class BSTaccount {
	AccountNode* root=nullptr;;
public:
	AccountNode* getRoot(){ return root; }

	BSTaccount(AccountNode* root) {
		this->root = root;
	}
	// Recursive add function for inserting a new account into the BST
	void addEnd(AccountNode* newAccount, AccountNode* current = nullptr){
		if (current == nullptr){
        current = root;
		}
		if(newAccount->getAccountId()==current->getAccountId()){
			cout<<"Same ID cannot exist, please create another account, or change ID"<<endl;
			return;
		}
		if(current->getAccountId()<newAccount->getAccountId()){
			// if (newAccount->getAccountId()==1300)
			// {
				// }
				// cout<<"current id:"<<current->getAccountId()<<endl;
				
				if(current->getRight()==nullptr){
					// cout<<newAccount->getAccountId()<<"set to right"<<endl;
					current->setRight(newAccount);
				}else{
					// cout<<newAccount->getAccountId()<<" went to right"<<endl;
					addEnd(newAccount, current->getRight());
				}
			}else{
			// cout<<"current id:"<<current->getAccountId()<<endl;
			if(current->getLeft()==nullptr){
				// cout<<newAccount->getAccountId()<<"set to Left"<<endl;
				current->setLeft(newAccount);
			}else{
				// cout<<newAccount->getAccountId()<<" went to Left"<<endl;
				addEnd(newAccount, current->getLeft());
			}
		}
	}
	// Recursive search function for finding an account by account number
	AccountNode* searchAccount(int searchAccountNumber, AccountNode* current=nullptr){
				if (current == nullptr){
        current = root;
		}
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
    int accountID = 0;
    string accountName = "NewAccount";
    double balance = 0.0;
	cout<<"Enter Account ID";
	cin >> accountID;
	cout<<"Enter Account Name";
	cin >> accountName;
	cout<<"Enter Account Balance";
	cin >> balance;

    TransactionNode* head = new TransactionNode();
    TransactionHistory* history = new TransactionHistory(head);
    AccountNode* newAccount = new AccountNode(accountID,accountName,balance,history);

    root->addEnd(newAccount, root->getRoot());
};
// Queue Node for Transaction Queue
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
	QueueNode* getNext() const { return next; }

	void setNext(QueueNode* next) { this->next = next; }
};

class TransactionQueue {
	private:
	QueueNode* front=nullptr;
	QueueNode* rear=nullptr;
public:

// Add a new transaction to the end of the queue
	void addQueue(int accountID, bool isDeposit, double amount, string date) {
		QueueNode* newNode = new QueueNode(accountID, isDeposit, amount, date);
		if (rear == nullptr) {
			front = rear = newNode;
			return;
		}else{
			rear->setNext(newNode);
			rear = newNode;
		}
	}
	// Remove and return the transaction at the front of the queue
	QueueNode* removeQueue(){
		if(front==nullptr){
			cout<<"Queue is empty!"<<endl;
			return nullptr;
		}else if(front==rear){
			QueueNode* temp = front;
			front = rear = nullptr;
			return temp;
		}else{
			QueueNode* temp = front;
			front = front->getNext();
			return temp;
		}
	}
};
