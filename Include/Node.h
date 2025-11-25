using namespace std;
#include <iostream>

class history_Node{
    bool is_Deposit;
    double amount;
    string date;
    string time;
    history_Node* next_History;

public:
    void SetIsDeposit(bool v){ is_Deposit = v; }
    void SetAmount(double a){ amount = a; }
    void SetDate(const string& d){ date = d; }
    void SetTime(const string& t){ time = t; }
    void SetNext(history_Node* nxt){ next_History = nxt; }

    bool IsDeposit() const { return is_Deposit; }
    double GetAmount() const { return amount; }
    string GetDate() const { return date; }
    string GetTime() const { return time; }
    history_Node* GetNext() const { return next_History; }
};

class account_Node{
    int account_Id;
    string account_Name;
    double balance;
    history_Node* transaction_History = nullptr;
    account_Node* right_Child = nullptr;   
    account_Node* left_Child = nullptr;   

public:
    void SetAccountId(int Id){ account_Id = Id; }
    void SetName(const string& name){ account_Name = name; }
    void SetBalance(double Balance){ balance = Balance; }
    void SetHistory(history_Node* History){ transaction_History = History; }
    void SetRight(account_Node* Right){ right_Child = Right; }
    void SetLeft(account_Node* Left){ left_Child = Left; }

    int GetAccountId() const { return account_Id; }
    string GetName() const { return account_Name; }
    double GetBalance() const { return balance; }
    history_Node* GetHistory() const { return transaction_History; }
    account_Node* GetRight() const { return right_Child; }
    account_Node* GetLeft() const { return left_Child; }
};

void addEnd(account_Node* root,account_Node* newAccount){
    if(root->GetAccountId()<newAccount->GetAccountId()){
        if(root->GetRight()==nullptr){
            root->SetRight(newAccount);
        }else{
            addEnd(root->GetRight(),newAccount);
        }
    }else{
        if(root->GetLeft()==nullptr){
            root->SetLeft(newAccount);
        }else{
            addEnd(root->GetLeft(),newAccount);
        }
    }
}

void addAccount(account_Node root){
    account_Node* newAccount = new account_Node();
    history_Node* history = new history_Node();
    int id = 0;
    string name = "NewAccount";
    double balance = 0.0;

    
    newAccount->SetAccountId(id);
    newAccount->SetName(name);
    newAccount->SetBalance(balance);
    newAccount->SetHistory(history);
    addEnd(&root,newAccount);
};

