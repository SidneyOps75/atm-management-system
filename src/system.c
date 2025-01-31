#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECORDS_FILE "data/records.txt"
#define USERS_FILE "data/users.txt"

int getAccountFromFile(FILE *ptr, char name[50],  Record *r)
{
    int result = fscanf(ptr, "%d %d %49s %d %d/%d/%d %49s %d %lf %49s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType);
    if (result == EOF) {
        return 0; // End of file
    } else if (result != 11) {
        // Skip invalid lines
        while (fgetc(ptr) != '\n' && !feof(ptr));
        return 0;
    }

    return 1;
        
}

void saveAccountToFile(FILE *ptr, User u, Record r)
{
    printf("Debug: Saving account with ID %d to file.\n", r.id);
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            
	        r.id,
            u.id,
	        u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);

            fflush(ptr);
            printf("Debug: Account saved successfully.\n");
}

int getNextRecordId() {
    FILE *fp = fopen("data/records.txt", "r");
    if (!fp){
        printf("Debug: records.txt does not exist. Returning 0.\n");
        perror("Error opening file");
        return 0;
    }  // No records exist yet

    Record record;
    int maxId = -1;
    printf("Debug: Starting to read records from file.\n");
    while (getAccountFromFile(fp, NULL, &record)) {
        printf("Debug: Read record with ID: %d\n", record.id);
        if (record.id > maxId) {
            maxId = record.id;
            printf("Debug: Updated maxId to: %d\n", maxId);
        }
    }

    fclose(fp);
    printf("Debug: Finished reading records. Next Record ID: %d\n", maxId + 1);
    return maxId + 1;
}

void stayOrReturn(int notGood, void (*f)(User), User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(User u)
{
     Record r = {0};
    char userName[50];
    FILE *pf = fopen(RECORDS_FILE, "a");
    if(!pf) {
        perror("Error opening file");
        return;
    }
    printf("Debug : File opened successfully\n");
    r.userId = u.id;
noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(dd/mm/yyyy):");
    scanf("%d/%d/%d", &r.deposit.day, &r.deposit.month, &r.deposit.year);
    printf("Debug: Date entered: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);

    clearInputBuffer();
    printf("Debug: Input buffer cleared.\n");
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);
    clearInputBuffer();
    printf("Debug: Account number entered: %d\n", r.accountNbr);

    rewind(pf);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%99s", r.country);
    clearInputBuffer();
    printf("Debug: Country entered: %s\n", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("Debug: Phone number entered: %d\n", r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("Debug: Amount entered: %.2f\n", r.amount);
    
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    // Clear the input buffer explicitly
clearInputBuffer();
    char temp[50];
printf("\nEnter your choice: ");
if (scanf("%49s", temp) != 1) { // Read into a temporary buffer
    printf("\n✖ Error reading account type! Please try again.\n");
    goto noAccount; // Restart account creation
}
strcpy(r.accountType, temp);
    printf("Debug: Account type entered: %s\n", r.accountType);
    fflush(stdout);
    printf("Debug: Attempting to get next record ID.\n");
    fflush(stdout);
    r.id = getNextRecordId();
    printf("Debug: Assigned account ID: %d\n", r.id);
    fflush(stdout);
    r.userId = u.id;
    printf("Debug: Assigned account ID: %d\n", r.userId);
    fflush(stdout);
    printf("Debug: Attempting to save account to file.\n");
    fflush(stdout);
    saveAccountToFile(pf, u, r);
    printf("Debug: Account saved successfully.\n");
    fflush(stdout);

    fclose(pf);
    printf("Debug: File closed successfully.\n");
    fflush(stdout);

    printf("\n✔ Account created successfully!\n");
    success(u);
}

void checkAllAccounts(User u)
{
    char userName[100];
    Record r;

    FILE *pf = fopen(RECORDS_FILE, "r");
    if (!pf)
    {
        perror("Error opening file");
        return;
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void checkAccountDetails(User u, int accountId) {
    FILE *fp = fopen(RECORDS_FILE, "r");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    Record r;
    char userName[50];
    int found = 0;

    printf("\n\t\t===== Account Details =====\n");
    while (getAccountFromFile(fp, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountId) {
            found = 1;
            printf("\nAccount Number: %d\n", r.accountNbr);
            printf("Deposit Date: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("Country: %s\n", r.country);
            printf("Phone Number: %d\n", r.phone);
            printf("Amount Deposited: $%.2lf\n", r.amount);
            printf("Account Type: %s\n", r.accountType);
            break;
        }
    }

    if (!found) {
        printf("\n✖ No account found with ID %d.\n", accountId);
    }

    fclose(fp);
}

void makeTransaction(int userId, int accountId, int transactionType) {
    FILE *fp = fopen("data/records.txt", "r");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    FILE *tempFile = fopen("data/temp.txt", "w");
    if (!tempFile) {
        perror("Error creating temporary file");
        fclose(fp);
        return;
    }

    Record r;
    char userName[100];
    int found = 0;

    while (getAccountFromFile(fp, userName, &r)) {
        if (r.userId == userId && r.accountNbr == accountId) {
            found = 1;

            double amount;
            if (transactionType == 1) { // Deposit
                printf("\nEnter the amount to deposit: $");
                scanf("%lf", &amount);
                r.amount += amount;
            } else if (transactionType == 2) { // Withdraw
                printf("\nEnter the amount to withdraw: $");
                scanf("%lf", &amount);
                if (amount > r.amount) {
                    printf("\n✖ Insufficient balance!\n");
                    fclose(fp);
                    fclose(tempFile);
                    remove("data/temp.txt");
                    return;
                }
                r.amount -= amount;
            }

            printf("\n✔ Transaction successful! New balance: $%.2lf\n", r.amount);
        }

        // Write the record to the temporary file
        fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                r.id,
                r.userId,
                userName,
                r.accountNbr,
                r.deposit.month,
                r.deposit.day,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
    }

    fclose(fp);
    fclose(tempFile);

    if (!found) {
        printf("\n✖ No account found with ID %d.\n", accountId);
        remove("data/temp.txt"); // Delete the temporary file
    } else {
        remove("data/records.txt");                  // Remove the original file
        rename("data/temp.txt", "data/records.txt"); // Rename the temp file to the original file
    }
}