#pragma once
#include <iostream>

// Transaction Node (Linked List)
class TransactionNode {
public:
	std::string type = "";
	double amount = 0.0;
	std::string date = "01/01/1980";

	TransactionNode* next = nullptr;

	TransactionNode() {}
	TransactionNode(std::string type, double amount, std::string date) {
		this->type = type;
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
	void addTransaction(std::string type, double amount, std::string date) {
		/* TODO: add check for type; should be only "withdrawal" or "deposit"
		 *       add a simple check for date too, maybe only one format
		 */
		TransactionNode* newNode = new TransactionNode(type, amount, date);
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
			std::cout << "No transactions." << std::endl;
			return;
		}

		while (current != nullptr) {
			std::cout << "Type: " << current->type << std::endl;
			std::cout << "Amount: " << current->amount << std::endl;
			std::cout << "Date: " << current->date << "\n\n";
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
	std::string accountName;
	double balance;

	TransactionNode* historyHead;
	AccountNode* left;
	AccountNode* right;
};

class 


class QueueNode {
private:
	int accountNumber;
	std::string type;
	double amount;
};

