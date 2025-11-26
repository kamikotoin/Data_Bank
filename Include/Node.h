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

	TransactionNode* historyHead;
	AccountNode* left;
	AccountNode* right;
};

class QueueNode {
private:
	int accountNumber;
	string type;
	double amount;
};

