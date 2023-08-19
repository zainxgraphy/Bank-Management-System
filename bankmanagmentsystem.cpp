#include <iostream>
#include <fstream>
using namespace std;
const int MAX_ACCOUNTS=100;
//Using Trees
class BankingManagementSystem {
private:
    struct Account {
        int accountNumber;
        string accountHolder;
        double balance;
        Account* left;
        Account* right;
    };

    Account* root;

public:
    BankingManagementSystem() : root(NULL) {}

    void run() {
        loadFromFile();

        int choice;
        do {
            printMenu();
            cin >> choice;

            switch (choice) {
                case 1:
                    addAccount();
                    break;
                case 2:
                    displayAccounts(root);
                    break;
                case 3: {
                    int accountNumber;
                    double amount;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    cout << "Enter the amount to deposit: ";
                    cin >> amount;
                    depositFunds(accountNumber, amount);
                    break;
                }
                case 4: {
                    int accountNumber;
                    double amount;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    cout << "Enter the amount to withdraw: ";
                    cin >> amount;
                    withdrawFunds(accountNumber, amount);
                    break;
                }
                case 5: {
                    int accountNumber;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    Account* account = searchAccount(accountNumber);
                    if (account !=NULL) {
                        cout << "Account Found!" << endl;
                        cout << "Account Number: " << account->accountNumber << endl;
                        cout << "Account Holder: " << account->accountHolder << endl;
                        cout << "Balance: " << account->balance << endl;
                    } else {
                        cout << "Account Not Found!" << endl;
                    }
                    break;
                }
                case 6:
                    saveToFile();
                    cout << "Accounts saved to file." << endl;
                    break;
                case 7: {
                    int accountNumber;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    deleteAccount(accountNumber);
                    break;
                }
                case 0:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
            }

            cout << endl;
        } while (choice != 0);
    }

private:
    void printMenu() {
        cout << "----- Banking Management System -----" << endl;
        cout << "1. Add Account" << endl;
        cout << "2. Display All Accounts" << endl;
        cout << "3. Deposit Funds" << endl;
        cout << "4. Withdraw Funds" << endl;
        cout << "5. Search Account" << endl;
        cout << "6. Save Accounts to File" << endl;
        cout << "7. Delete Account" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
    }

    void addAccount() {
        Account* newAccount = new Account;
        cout << "Enter the account number: ";
        cin >> newAccount->accountNumber;
        cout << "Enter the account holder name: ";
        cin.ignore();
        getline(cin, newAccount->accountHolder);
        cout << "Enter the initial balance: ";
        cin >> newAccount->balance;
        newAccount->left = NULL;
        newAccount->right = NULL;

        if (root ==NULL) {
            root = newAccount;
            cout << "Account added successfully!" << endl;
            return;
        }

        Account* current = root;
        while (true) {
            if (newAccount->accountNumber < current->accountNumber) {
                if (current->left == NULL) {
                    current->left = newAccount;
                    break;
                }
                current = current->left;
            } else if (newAccount->accountNumber > current->accountNumber) {
                if (current->right == NULL) {
                    current->right = newAccount;
                    break;
                }
                current = current->right;
            } else {
                delete newAccount;
                cout << "Account with the same account number already exists!" << endl;
                return;
            }
        }

        cout << "Account added successfully!" << endl;
    }

    void displayAccounts(const Account* root) const {
        if (root == NULL) {
            cout << "No accounts to display." << endl;
            return;
        }

        cout << "----- Account Details -----" << endl;
        // In-order traversal
        if (root->left != NULL) {
            displayAccounts(root->left);
        }
        cout << "Account Number: " << root->accountNumber << endl;
        cout << "Account Holder: " << root->accountHolder << endl;
        cout << "Balance: " << root->balance << endl;
        cout << endl;
        if (root->right != NULL) {
            displayAccounts(root->right);
        }
    }

    void depositFunds(int accountNumber, double amount) {
        Account* account = searchAccount(accountNumber);
        if (account != NULL) {
            account->balance += amount;
            cout << "Amount deposited successfully!" << endl;
        } else {
            cout << "Account not found. Deposit failed!" << endl;
        }
    }

    void withdrawFunds(int accountNumber, double amount) {
        Account* account = searchAccount(accountNumber);
        if (account != NULL) {
            if (account->balance >= amount) {
                account->balance -= amount;
                cout << "Amount withdrawn successfully!" << endl;
            } else {
                cout << "Insufficient balance. Withdrawal failed!" << endl;
            }
        } else {
            cout << "Account not found. Withdrawal failed!" << endl;
        }
    }

    Account* searchAccount(int accountNumber) const {
        Account* current = root;
        while (current != NULL) {
            if (accountNumber == current->accountNumber) {
                return current;
            } else if (accountNumber < current->accountNumber) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return NULL;
    }

    void saveToFile() const {
        ofstream outFile("accounts1.txt");
        if (!outFile) {
            cout << "Error opening file!" << endl;
            return;
        }

        saveToFile(root, outFile);

        outFile.close();
    }

    void saveToFile(const Account* root, ofstream& outFile) const {
        if (root != NULL) {
            saveToFile(root->left, outFile);
            outFile << root->accountNumber << endl;
            outFile << root->accountHolder << endl;
            outFile << root->balance << endl;
            saveToFile(root->right, outFile);
        }
    }

    void loadFromFile() {
        ifstream inFile("accounts1.txt");
        if (!inFile) {
            cout << "File not found. Starting with an empty account list." << endl;
            return;
        }

        int accountNumber;
        string accountHolder;
        double balance;
        while (inFile >> accountNumber >> ws && getline(inFile, accountHolder) && inFile >> balance) {
            addAccountFromFile(accountNumber, accountHolder, balance);
        }

        inFile.close();
    }

    void addAccountFromFile(int accountNumber, const string& accountHolder, double balance) {
        Account* newAccount = new Account;
        newAccount->accountNumber = accountNumber;
        newAccount->accountHolder = accountHolder;
        newAccount->balance = balance;
        newAccount->left = NULL;
        newAccount->right = NULL;

        if (root == NULL) {
            root = newAccount;
            cout << "Account added successfully!" << endl;
            return;
        }

        Account* current = root;
        while (true) {
            if (newAccount->accountNumber < current->accountNumber) {
                if (current->left == NULL) {
                    current->left = newAccount;
                    break;
                }
                current = current->left;
            } else if (newAccount->accountNumber > current->accountNumber) {
                if (current->right == NULL) {
                    current->right = newAccount;
                    break;
                }
                current = current->right;
            } else {
                delete newAccount;
                cout << "Account with the same account number already exists!" << endl;
                return;
            }
        }

        cout << "Account added successfully!" << endl;
    }

    Account* findMinAccount(Account* root) const {
        while (root->left != NULL) {
            root = root->left;
        }
        return root;
    }

    Account* deleteNode(Account* root, int accountNumber) {
        if (root == NULL) {
            return NULL;
        }

        if (accountNumber < root->accountNumber) {
            root->left = deleteNode(root->left, accountNumber);
        } else if (accountNumber > root->accountNumber) {
            root->right = deleteNode(root->right, accountNumber);
        } else {
            // Node to be deleted found
            if (root->left == NULL && root->right == NULL) {
                // Case 1: Node is a leaf node
                delete root;
                root = NULL;
            } else if (root->left == NULL) {
                // Case 2: Node has only a right child
                Account* temp = root;
                root = root->right;
                delete temp;
            } else if (root->right == NULL) {
                // Case 2: Node has only a left child
                Account* temp = root;
                root = root->left;
                delete temp;
            } else {
                // Case 3: Node has both left and right children
                Account* temp = findMinAccount(root->right);
                root->accountNumber = temp->accountNumber;
                root->accountHolder = temp->accountHolder;
                root->balance = temp->balance;
                root->right = deleteNode(root->right, temp->accountNumber);
            }
        }

        return root;
    }

    void deleteAccount(int accountNumber) {
        if (root == NULL) {
            cout << "No accounts to delete." << endl;
            return;
        }

        root = deleteNode(root, accountNumber);

        if (root == NULL) {
            cout << "Account deleted successfully!" << endl;
        } else {
            cout << "Account not found. Deletion failed!" << endl;
        }
    }
};

void printMenu() {
    cout << "----- Banking Management System -----" << endl;
    cout << "1. Add Account" << endl;
    cout << "2. Display All Accounts" << endl;
    cout << "3. Deposit Funds" << endl;
    cout << "4. Withdraw Funds" << endl;
    cout << "5. Search Account" << endl;
    cout << "6. Save Accounts to File" << endl;
    cout << "7. Delete Account" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}
//Array
struct Account {
    int accountNumber;
    string accountHolder;
    double balance;
   
};

class BankingManagementSystem2 {
private:
    Account accounts[MAX_ACCOUNTS];
    int numAccounts;

public:
    BankingManagementSystem2() {
        numAccounts = 0;
    }

    void addAccount() {
        if (numAccounts >= MAX_ACCOUNTS) {
            cout << "Maximum number of accounts reached." << endl;
            return;
        }

        cout << "Enter the account number: ";
        cin >> accounts[numAccounts].accountNumber;
        cout << "Enter the account holder name: ";
        cin.ignore();
        getline(cin, accounts[numAccounts].accountHolder);
        cout << "Enter the initial balance: ";
        cin >> accounts[numAccounts].balance;

        numAccounts++;
        cout << "Account added successfully!" << endl;
    }

    void displayAccounts() {
        if (numAccounts == 0) {
            cout << "No accounts to display." << endl;
            return;
        }

        cout << "----- Account Details -----" << endl;
        for (int i = 0; i < numAccounts; i++) {
            cout << "Account Number: " << accounts[i].accountNumber << endl;
            cout << "Account Holder: " << accounts[i].accountHolder << endl;
            cout << "Balance: " << accounts[i].balance << endl;
            cout << endl;
        }
    }

    void depositFunds(int accountNumber, double amount) {
        Account* account = searchAccount(accountNumber);
        if (account != NULL) {
            account->balance += amount;
            cout << "Amount deposited successfully!" << endl;
        } else {
            cout << "Account not found. Deposit failed!" << endl;
        }
    }

    void withdrawFunds(int accountNumber, double amount) {
        Account* account = searchAccount(accountNumber);
        if (account != NULL) {
            if (account->balance >= amount) {
                account->balance -= amount;
                cout << "Amount withdrawn successfully!" << endl;
            } else {
                cout << "Insufficient balance. Withdrawal failed!" << endl;
            }
        } else {
            cout << "Account not found. Withdrawal failed!" << endl;
        }
    }

    void deleteAccount(int accountNumber) {
        int accountIndex = -1;
        for (int i = 0; i < numAccounts; i++) {
            if (accounts[i].accountNumber == accountNumber) {
                accountIndex = i;
                break;
            }
        }

        if (accountIndex != -1) {
            for (int i = accountIndex; i < numAccounts - 1; i++) {
                accounts[i] = accounts[i + 1];
            }
            numAccounts--;
            cout << "Account deleted successfully!" << endl;
        } else {
            cout << "Account not found. Deletion failed!" << endl;
        }
    }

    Account* searchAccount(int accountNumber) {
        for (int i = 0; i < numAccounts; i++) {
            if (accounts[i].accountNumber == accountNumber) {
                return &accounts[i];
            }
        }
        return NULL;
    }

    void saveToFile() {
        ofstream outFile("accounts2.txt");
        if (!outFile) {
            cout << "Error opening file for writing." << endl;
            return;
        }

        for (int i = 0; i < numAccounts; i++) {
            outFile << accounts[i].accountNumber << endl;
            outFile << accounts[i].accountHolder << endl;
            outFile << accounts[i].balance << endl;
        }

        outFile.close();
    }

    void loadFromFile() {
        ifstream inFile("accounts2.txt");
        if (!inFile) {
            cout << "File not found. Starting with an empty account list." << endl;
            return;
        }

        numAccounts = 0;
        while (inFile >> accounts[numAccounts].accountNumber >> ws && getline(inFile, accounts[numAccounts].accountHolder) && inFile >> accounts[numAccounts].balance) {
            numAccounts++;
        }

        inFile.close();
    }

    void run() {
        loadFromFile();

        int choice;
        do {
            cout << "----- Banking Management System -----" << endl;
            cout << "1. Add Account" << endl;
            cout << "2. Display All Accounts" << endl;
            cout << "3. Deposit Funds" << endl;
            cout << "4. Withdraw Funds" << endl;
            cout << "5. Search Account" << endl;
            cout << "6. Save Accounts to File" << endl;
            cout << "7. Delete Account" << endl;
            cout << "0. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addAccount();
                    break;
                case 2:
                    displayAccounts();
                    break;
                case 3: {
                    int accountNumber;
                    double amount;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    cout << "Enter the amount to deposit: ";
                    cin >> amount;
                    depositFunds(accountNumber, amount);
                    break;
                }
                case 4: {
                    int accountNumber;
                    double amount;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    cout << "Enter the amount to withdraw: ";
                    cin >> amount;
                    withdrawFunds(accountNumber, amount);
                    break;
                }
                case 5: {
                    int accountNumber;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    Account* account = searchAccount(accountNumber);
                    if (account != NULL) {
                        cout << "Account Found!" << endl;
                        cout << "Account Number: " << account->accountNumber << endl;
                        cout << "Account Holder: " << account->accountHolder << endl;
                        cout << "Balance: " << account->balance << endl;
                    } else {
                        cout << "Account Not Found!" << endl;
                    }
                    break;
                }
                case 6:
                    saveToFile();
                    cout << "Accounts saved to file." << endl;
                    break;
                case 7: {
                    int accountNumber;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    deleteAccount(accountNumber);
                    break;
                }
                case 0:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
            }

            cout << endl;
        } while (choice != 0);
    }
};
//LINKED LIST
struct Account2 {
    int accountNumber;
    string accountHolder;
    double balance;
    Account2* next;
};

class BankingManagementSystem3 {
private:
    Account2* head;

public:
    BankingManagementSystem3() {
        head = NULL;
        loadFromFile();
    }

    void addAccount() {
        Account2* newAccount = new Account2;
        cout << "Enter the account number: ";
        cin >> newAccount->accountNumber;
        cout << "Enter the account holder name: ";
        cin.ignore();
        getline(cin, newAccount->accountHolder);
        cout << "Enter the initial balance: ";
        cin >> newAccount->balance;
        newAccount->next = NULL;

        if (head == NULL) {
            head = newAccount;
        } else {
            Account2* current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newAccount;
        }

        cout << "Account added successfully!" << endl;
    }

    void displayAccounts() const {
        if (head == NULL) {
            cout << "No accounts to display." << endl;
            return;
        }

        cout << "----- Account Details -----" << endl;
        const Account2* current = head;
        while (current != NULL) {
            cout << "Account Number: " << current->accountNumber << endl;
            cout << "Account Holder: " << current->accountHolder << endl;
            cout << "Balance: " << current->balance << endl;
            cout << endl;
            current = current->next;
        }
    }

    void depositFunds(int accountNumber, double amount) {
        Account2* account = searchAccount(accountNumber);
        if (account != NULL) {
            account->balance += amount;
            cout << "Amount deposited successfully!" << endl;
        } else {
            cout << "Account not found. Deposit failed!" << endl;
        }
    }

    void withdrawFunds(int accountNumber, double amount) {
        Account2* account = searchAccount(accountNumber);
        if (account != NULL) {
            if (account->balance >= amount) {
                account->balance -= amount;
                cout << "Amount withdrawn successfully!" << endl;
            } else {
                cout << "Insufficient balance. Withdrawal failed!" << endl;
            }
        } else {
            cout << "Account not found. Withdrawal failed!" << endl;
        }
    }

    Account2* searchAccount(int accountNumber) const {
        Account2* current = head;
        while (current != NULL) {
            if (current->accountNumber == accountNumber) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    void saveToFile() const {
        ofstream outFile("accounts3.txt");
        const Account2* current = head;
        while (current != NULL) {
            outFile << current->accountNumber << endl;
            outFile << current->accountHolder << endl;
            outFile << current->balance << endl;
            current = current->next;
        }
        outFile.close();
    }

    void deleteAccount(int accountNumber) {
        if (head == NULL) {
            cout << "No accounts to delete." << endl;
            return;
        }

        // Check if the head node is the one to be deleted
        if (head->accountNumber == accountNumber) {
            Account2* temp = head;
            head = head->next;
            delete temp;
            cout << "Account deleted successfully!" << endl;
            return;
        }

        Account2* prev = NULL;
        Account2* current = head;

        while (current != NULL && current->accountNumber != accountNumber) {
            prev = current;
            current = current->next;
        }

        if (current == NULL) {
            cout << "Account not found. Deletion failed!" << endl;
            return;
        }

        prev->next = current->next;
        delete current;
        cout << "Account deleted successfully!" << endl;
    }

    void run() {
        int choice;
        do {
            cout << "----- Banking Management System -----" << endl;
            cout << "1. Add Account" << endl;
            cout << "2. Display All Accounts" << endl;
            cout << "3. Deposit Funds" << endl;
            cout << "4. Withdraw Funds" << endl;
            cout << "5. Search Account" << endl;
            cout << "6. Save Accounts to File" << endl;
            cout << "7. Delete Account" << endl;
            cout << "0. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addAccount();
                    break;
                case 2:
                    displayAccounts();
                    break;
                case 3: {
                    int accountNumber;
                    double amount;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    cout << "Enter the amount to deposit: ";
                    cin >> amount;
                    depositFunds(accountNumber, amount);
                    break;
                }
                case 4: {
                    int accountNumber;
                    double amount;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    cout << "Enter the amount to withdraw: ";
                    cin >> amount;
                    withdrawFunds(accountNumber, amount);
                    break;
                }
                case 5: {
                    int accountNumber;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    Account2* account = searchAccount(accountNumber);
                    if (account != NULL) {
                        cout << "Account Found!" << endl;
                        cout << "Account Number: " << account->accountNumber << endl;
                        cout << "Account Holder: " << account->accountHolder << endl;
                        cout << "Balance: " << account->balance << endl;
                    } else {
                        cout << "Account Not Found!" << endl;
                    }
                    break;
                }
                case 6:
                    saveToFile();
                    cout << "Accounts saved to file." << endl;
                    break;
                case 7: {
                    int accountNumber;
                    cout << "Enter the account number: ";
                    cin >> accountNumber;
                    deleteAccount(accountNumber);
                    break;
                }
                case 0:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
            }

            cout << endl;
        } while (choice != 0);
    }

    void loadFromFile() {
        ifstream inFile("accounts3.txt");
        if (!inFile) {
            cout << "File not found. Starting with an empty account list." << endl;
            return;
        }

        int accountNumber;
        string accountHolder;
        double balance;
        while (inFile >> accountNumber >> ws && getline(inFile, accountHolder) && inFile >> balance) {
            Account2* newAccount = new Account2;
            newAccount->accountNumber = accountNumber;
            newAccount->accountHolder = accountHolder;
            newAccount->balance = balance;
            newAccount->next = NULL;

            if (head == NULL) {
                head = newAccount;
            } else {
                Account2* current = head;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newAccount;
            }
        }

        inFile.close();
    }
};
void mainprogramall(){
	
	while(true){
	cout<<"Press 1 for Trees"<<endl;
	cout<<"Press 2 for arrays"<<endl;
	cout<<"Press 3 for linkedlist"<<endl;
	cout<<"Press 4 to exit"<<endl;
	cout<<"Enter your choice:";
	int choice;
	cin>>choice;
	if(choice==1){
		BankingManagementSystem bankingSystem;
    	bankingSystem.run();
	}
	else if(choice==2){
		BankingManagementSystem2 system2;
    		system2.run();
	}
	else if(choice==3){
		BankingManagementSystem3 system3;
    system3.run();
	}
	else if(choice==4){
		exit(0);
	}
	else{
		cout<<"Invalid choice! Try again..."<<endl;
	}
	
	}
	
}
int main() {
	cout<<"WELCOME TO BANK MANAGEMENT SYSTEM"<<endl;
	cout<<"Please Enter your 4 digit pin:";
	int pin;
	cin>>pin;
	if(pin==1234){
		cout<<"Login successfully"<<endl;
		mainprogramall();
	}
	else{
		cout<<"Your password is incorrect.Exiting the program...";
		exit(0);
	}
    return 0;
}