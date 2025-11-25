using namespace std;
#include <iostream>
#include "Include/Node.h";

int main() {
    account_Node* root=new account_Node();
    root->SetAccountId(50);
    root->SetName("RootAccount");
    root->SetBalance(1000.0);
    
    addAccount(root, new account_Node());
    
    return 0;
}