// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.
#include <stdio.h>
#include <stdlib.h>

// clientData structure definition
struct clientData {
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance
};                        // end structure clientData

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayAllAccounts(FILE *fPtr);
void logTransaction(unsigned int acctNum, double amount, double newBalance, const char *transactionType);
void viewTransactions(void);

int main(int argc, char *argv[])
{
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        printf("%s: File could not be opened.\n", argv[0]);
        exit(-1);
    }

    while ((choice = enterChoice()) != 5) {
        switch (choice) {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            updateRecord(cfPtr);
            break;
        case 3:
            newRecord(cfPtr);
            break;
        case 4:
            deleteRecord(cfPtr);
            break;
        case 6:
            displayAllAccounts(cfPtr);
            break;
        case 7:
            viewTransactions();
            break;
        default:
            puts("Incorrect choice");
            break;
        }
    }

    fclose(cfPtr);
}

// create formatted text file for printing
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        puts("File could not be opened.");
    } else {
        rewind(readPtr);
        fprintf(writePtr, "%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

        while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
            if (client.acctNum != 0) {
                fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                        client.acctNum, client.lastName, client.firstName, client.balance);
            }
        }
        fclose(writePtr);
    }
}

// update balance in record
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;
    struct clientData client = {0, "", "", 0.0};

    printf("%s", "Enter account to update ( 1 - 100 ): ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account #%d has no information.\n", account);
    } else {
        printf("%-6d%-16s%-11s%10.2f\n\n",
               client.acctNum, client.lastName, client.firstName, client.balance);

        printf("%s", "Enter charge ( + ) or payment ( - ): ");
        scanf("%lf", &transaction);
        client.balance += transaction;

        printf("%-6d%-16s%-11s%10.2f\n",
               client.acctNum, client.lastName, client.firstName, client.balance);

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        logTransaction(client.acctNum, transaction, client.balance, "Update");
    }
}

// delete an existing record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0};
    unsigned int accountNum;

    printf("%s", "Enter account number to delete ( 1 - 100 ): ");
    scanf("%d", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account %d does not exist.\n", accountNum);
    } else {
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

        logTransaction(accountNum, 0.0, 0.0, "Delete");
    }
}

// create and insert record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;

    printf("%s", "Enter new account number ( 1 - 100 ): ");
    scanf("%d", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf("Account #%d already contains information.\n", client.acctNum);
    } else {
        printf("%s", "Enter lastname, firstname, balance\n? ");
        scanf("%14s%9s%lf", client.lastName, client.firstName, &client.balance);

        client.acctNum = accountNum;
        fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        logTransaction(client.acctNum, client.balance, client.balance, "New Account");
    }
}

// display all accounts directly on console
void displayAllAccounts(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    rewind(fPtr);
    printf("%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum != 0) {
            printf("%-6d%-16s%-11s%10.2f\n",
                   client.acctNum, client.lastName, client.firstName, client.balance);
        }
    }
}

// log transactions into a separate file
void logTransaction(unsigned int acctNum, double amount, double newBalance, const char *transactionType)
{
    FILE *logPtr;

    if ((logPtr = fopen("transactions.txt", "a")) == NULL) {
        puts("Transaction log file could not be opened.");
        return;
    }

    fprintf(logPtr, "Account: %u | Action: %s | Amount: %+8.2f | New Balance: %8.2f\n",
            acctNum, transactionType, amount, newBalance);

    fclose(logPtr);
}

// view transaction history
void viewTransactions(void)
{
    FILE *logPtr;
    char line[200];

    logPtr = fopen("transactions.txt", "r");
    if (logPtr == NULL) {
        puts("No transaction history found.");
        return;
    }

    puts("\n--- Transaction History ---");
    while (fgets(line, sizeof(line), logPtr)) {
        printf("%s", line);
    }
    puts("---------------------------\n");

    fclose(logPtr);
}

// enable user to input menu choice
unsigned int enterChoice(void)
{
    unsigned int menuChoice;
    printf("%s", "\nEnter your choice\n"
                 "1 - store a formatted text file of accounts called\n"
                 "    \"accounts.txt\" for printing\n"
                 "2 - update an account\n"
                 "3 - add a new account\n"
                 "4 - delete an account\n"
                 "5 - end program\n"
                 "6 - display all accounts\n"
                 "7 - view transaction history\n? ");
    scanf("%u", &menuChoice);
    return menuChoice;
}
