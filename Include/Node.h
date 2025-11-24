using namespace std;
#include <iostream>

class history_Node{
    bool is_Deposit;
    double amount;
    string date;
    string time;
    history_Node* next_History;
};

class account_Node{   
 int account_Id;
 string account_Name;
 double balance;
 history_Node* transaction_History;
 account_Node* right_Child;   
 account_Node* left_Child;   

};
