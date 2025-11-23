// Simple Banking System in C
// Features:
// - Create account
// - Login
// - Deposit / Withdraw
// - Check balance
// - Simple transaction history
// - Uses simple file storage

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACC_FILE "accounts.dat"
#define TXN_FILE "transactions.dat"

// Account structure
typedef struct {
    int id;
    char name[50];
    int pin;
    float balance;
} Account;

// Transaction structure
typedef struct {
    int accId;
    char type[10];
    float amount;
} Transaction;

// Function declarations
void createAccount();
void login();
void deposit(Account *acc);
void withdraw(Account *acc);
void saveAccount(Account acc);
void saveTransaction(int id, const char *type, float amt);
void viewTransactions(int id);

// MAIN FUNCTION
int main() {
    int choice;

    while (1) {
        printf("\n=== Simple Banking System ===\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: createAccount(); break;
            case 2: login(); break;
            case 3: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}

// CREATE ACCOUNT
void createAccount() {
    Account acc;
    FILE *fp = fopen(ACC_FILE, "ab");

    printf("Enter name: ");
    scanf("%s", acc.name);

    printf("Set PIN (4-digit): ");
    scanf("%d", &acc.pin);

    acc.balance = 0;

    // Generate account ID
    acc.id = rand() % 90000 + 10000;

    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);

    printf("Account created! Your Account ID is: %d\n", acc.id);
}

// LOGIN
void login() {
    int id, pin;
    Account acc;
    FILE *fp = fopen(ACC_FILE, "rb");

    printf("Enter Account ID: "); scanf("%d", &id);
    printf("Enter PIN: "); scanf("%d", &pin);

    int found = 0;

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.id == id && acc.pin == pin) {
            found = 1; break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Invalid ID or PIN!\n");
        return;
    }

    int choice;
    while (1) {
        printf("\nWelcome, %s!\n", acc.name);
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. View Transactions\n");
        printf("5. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: printf("Current Balance: %.2f\n", acc.balance); break;
            case 2: deposit(&acc); break;
            case 3: withdraw(&acc); break;
            case 4: viewTransactions(acc.id); break;
            case 5: saveAccount(acc); return;
            default: printf("Invalid choice!\n");
        }
    }
}

// SAVE ACCOUNT BACK TO FILE
void saveAccount(Account acc) {
    FILE *fp = fopen(ACC_FILE, "rb+");
    Account temp;

    while (fread(&temp, sizeof(Account), 1, fp)) {
        if (temp.id == acc.id) {
            fseek(fp, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, fp);
            break;
        }
    }
    fclose(fp);
}

// DEPOSIT
void deposit(Account *acc) {
    float amt;
    printf("Enter amount to deposit: ");
    scanf("%f", &amt);

    acc->balance += amt;
    saveTransaction(acc->id, "DEPOSIT", amt);

    printf("Amount deposited successfully!\n");
}

// WITHDRAW
void withdraw(Account *acc) {
    float amt;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amt);

    if (amt > acc->balance) {
        printf("Insufficient balance!\n");
        return;
    }

    acc->balance -= amt;
    saveTransaction(acc->id, "WITHDRAW", amt);

    printf("Amount withdrawn successfully!\n");
}

// SAVE TRANSACTION ENTRY
void saveTransaction(int id, const char *type, float amt) {
    Transaction t;
    t.accId = id;
    strcpy(t.type, type);
    t.amount = amt;

    FILE *fp = fopen(TXN_FILE, "ab");
    fwrite(&t, sizeof(Transaction), 1, fp);
    fclose(fp);
}

// VIEW TRANSACTIONS
void viewTransactions(int id) {
    Transaction t;
    FILE *fp = fopen(TXN_FILE, "rb");

    printf("\n--- Transaction History ---\n");

    while (fread(&t, sizeof(Transaction), 1, fp)) {
        if (t.accId == id) {
            printf("%s : %.2f\n", t.type, t.amount);
        }
    }
    fclose(fp);
}