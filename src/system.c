#include "header.h"

int getAccountFromFile(FILE *ptr, char name[MAX_USERNAME_LENGTH], struct Record *r) {
    return fscanf(ptr, "%d %d %s %s %d/%d/%d %s %s %lf %s",
                  &r->id, &r->userId, name, r->accountNbr,
                  &r->deposit.month, &r->deposit.day, &r->deposit.year,
                  r->country, r->phone, &r->amount, r->accountType) != EOF;
}

int getAccountFromUser(FILE *ptr, char name[MAX_USERNAME_LENGTH], struct User *r) {
    return fscanf(ptr, "%d %s", &r->id, name);
}

ErrorCode saveAccountToFile(FILE *ptr, struct User u, struct Record r) {
    if (fprintf(ptr, "%d %d %s %s %d/%d/%d %s %s %.2lf %s\n\n",
                r.id, u.id, u.name, r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType) < 0) {
        return ERROR_FILE_WRITE;
    }
    return SUCCESS;
}

ErrorCode saveRecordToFile(FILE *ptr, struct Record r) {
    if (fprintf(ptr, "%d %d %s %s %d/%d/%d %s %s %.2lf %s\n\n",
                r.id, r.userId, r.name, r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType) < 0) {
        return ERROR_FILE_WRITE;
    }
    return SUCCESS;
}

ErrorCode validateAccountType(const char *accountType) {
    if (strcmp(accountType, "current") != 0 &&
        strcmp(accountType, "savings") != 0 &&
        strcmp(accountType, "fixed01") != 0 &&
        strcmp(accountType, "fixed02") != 0 &&
        strcmp(accountType, "fixed03") != 0) {
        return ERROR_INVALID_INPUT;
    }
    return SUCCESS;
}

ErrorCode createNewAcc(struct User u) {
    struct Record r, cr;
    char userName[MAX_USERNAME_LENGTH];
    FILE *pf;
    int ind = 0;
    ErrorCode result;
    
    printHeader("CREATE NEW ACCOUNT");
    
    pf = fopen(RECORDS_FILE, "a+");
    if (pf == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Get today's date
    while (1) {
        result = getDateInput(&r.deposit, "Enter today's date");
        if (result == SUCCESS) break;
        handleError(result, "Please use format MM/DD/YYYY (e.g., 8/8/2024)");
    }
    
    // Get account number
    while (1) {
        result = getStringInput(r.accountNbr, MAX_ACCOUNT_NUMBER_LENGTH, 
                               "Enter account number", false);
        if (result != SUCCESS) {
            handleError(result, "Account number must be numeric");
            continue;
        }
        
        if (strlen(r.accountNbr) < 1 || strlen(r.accountNbr) > 18) {
            handleError(ERROR_INVALID_INPUT, "Account number must be 1-18 digits");
            continue;
        }
        
        if (!is_valid_number(r.accountNbr)) {
            handleError(ERROR_INVALID_INPUT, "Account number must contain only digits");
            continue;
        }
        
        // Check if account already exists
        rewind(pf);
        bool accountExists = false;
        while (getAccountFromFile(pf, userName, &cr)) {
            if (strcmp(userName, u.name) == 0 && strcmp(cr.accountNbr, r.accountNbr) == 0) {
                handleError(ERROR_DUPLICATE_ACCOUNT, "You already have an account with this number");
                accountExists = true;
                break;
            } else if (strcmp(cr.accountNbr, r.accountNbr) == 0) {
                handleError(ERROR_DUPLICATE_ACCOUNT, "This account number is taken by another user");
                accountExists = true;
                break;
            }
            ind++;
        }
        
        if (!accountExists) break;
        ind = 0;
    }
    
    r.id = ind;
    u.id = getUserId(u.name);
    
    // Get country
    while (1) {
        result = getStringInput(r.country, MAX_COUNTRY_LENGTH, "Enter country", true);
        if (result != SUCCESS) {
            handleError(result, "Country name must contain only letters and spaces");
            continue;
        }
        
        if (strlen(r.country) < 2 || strlen(r.country) > 65) {
            handleError(ERROR_INVALID_COUNTRY, "Country name must be 2-65 characters");
            continue;
        }
        
        sanitize(r.country);
        break;
    }
    
    // Get phone number
    while (1) {
        result = getStringInput(r.phone, MAX_PHONE_LENGTH, "Enter phone number", false);
        if (result != SUCCESS) {
            handleError(result, "Phone number must contain only digits");
            continue;
        }
        
        if (strlen(r.phone) < 5 || strlen(r.phone) > 14) {
            handleError(ERROR_INVALID_PHONE, "Phone number must be 5-14 digits");
            continue;
        }
        
        break;
    }
    
    // Get initial deposit amount
    while (1) {
        result = getDoubleInput(&r.amount, "Enter initial deposit amount", 
                               MIN_TRANSACTION_AMOUNT, MAX_TRANSACTION_AMOUNT);
        if (result == SUCCESS) break;
        handleError(result, "Please enter a valid amount");
    }
    
    // Get account type
    while (1) {
        printf("\nAccount Types:\n");
        printf("  savings    - 7%% annual interest (monthly payout)\n");
        printf("  current    - No interest\n");
        printf("  fixed01    - 4%% annual interest (1 year term)\n");
        printf("  fixed02    - 5%% annual interest (2 year term)\n");
        printf("  fixed03    - 8%% annual interest (3 year term)\n");
        
        result = getStringInput(r.accountType, MAX_ACCOUNT_TYPE_LENGTH, 
                               "Enter account type", false);
        if (result != SUCCESS) {
            handleError(result, "Please enter a valid account type");
            continue;
        }
        
        result = validateAccountType(r.accountType);
        if (result == SUCCESS) break;
        handleError(result, "Please choose from the listed account types");
    }
    
    result = saveAccountToFile(pf, u, r);
    fclose(pf);
    
    if (result == SUCCESS) {
        printSuccess("Account created successfully!");
        printf("\nAccount Details:\n");
        printf("Account Number: %s\n", r.accountNbr);
        printf("Account Type: %s\n", r.accountType);
        printf("Initial Deposit: $%.2f\n", r.amount);
    }
    
    return result;
}

ErrorCode checkAllAccounts(struct User u) {
    char userName[MAX_USERNAME_LENGTH];
    struct Record r;
    FILE *pf;
    int accountCount = 0;
    
    printHeader("ALL YOUR ACCOUNTS");
    
    pf = fopen(RECORDS_FILE, "r");
    if (pf == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0) {
            if (accountCount == 0) {
                printSeparator();
            }
            
            printf(COLOR_BOLD "Account #%d\n" COLOR_RESET, ++accountCount);
            printf("Owner: %s\n", userName);
            printf("Account Number: %s\n", r.accountNbr);
            printf("Date Created: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("Country: %s\n", r.country);
            printf("Phone: %s\n", r.phone);
            printf("Balance: " COLOR_GREEN "$%.2f" COLOR_RESET "\n", r.amount);
            printf("Account Type: %s\n", r.accountType);
            printSeparator();
        }
    }
    
    fclose(pf);
    
    if (accountCount == 0) {
        printInfo("You don't have any accounts yet.");
        return ERROR_ACCOUNT_NOT_FOUND;
    }
    
    printf(COLOR_BOLD "Total Accounts: %d\n" COLOR_RESET, accountCount);
    return SUCCESS;
}

void calculateInterest(struct Record r) {
    double rate = 0;
    double interest = 0;
    
    if (strcmp(r.accountType, "savings") == 0) {
        rate = 0.07;
        interest = r.amount * (rate / 12);
        printf(COLOR_GREEN "→ Monthly Interest: $%.2f (paid on day %d of each month)\n" COLOR_RESET,
               interest, r.deposit.day);
    } else if (strcmp(r.accountType, "fixed01") == 0) {
        rate = 0.04;
        interest = r.amount * rate;
        printf(COLOR_GREEN "→ Annual Interest: $%.2f (paid on %d/%d/%d)\n" COLOR_RESET,
               interest, r.deposit.month, r.deposit.day, r.deposit.year + 1);
    } else if (strcmp(r.accountType, "fixed02") == 0) {
        rate = 0.05;
        interest = r.amount * rate * 2;
        printf(COLOR_GREEN "→ Total Interest: $%.2f (paid on %d/%d/%d)\n" COLOR_RESET,
               interest, r.deposit.month, r.deposit.day, r.deposit.year + 2);
    } else if (strcmp(r.accountType, "fixed03") == 0) {
        rate = 0.08;
        interest = r.amount * rate * 3;
        printf(COLOR_GREEN "→ Total Interest: $%.2f (paid on %d/%d/%d)\n" COLOR_RESET,
               interest, r.deposit.month, r.deposit.day, r.deposit.year + 3);
    } else {
        printf(COLOR_YELLOW "→ No interest (current account)\n" COLOR_RESET);
    }
}

ErrorCode checkDetailOfAccount(struct User u, char *accountNum) {
    char userName[MAX_USERNAME_LENGTH];
    struct Record r;
    FILE *pf;
    
    printHeader("ACCOUNT DETAILS");
    
    pf = fopen(RECORDS_FILE, "r");
    if (pf == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0) {
            printf(COLOR_BOLD "Account Information:\n" COLOR_RESET);
            printSeparator();
            printf("Owner: %s\n", userName);
            printf("Account Number: %s\n", r.accountNbr);
            printf("Date Created: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("Country: %s\n", r.country);
            printf("Phone: %s\n", r.phone);
            printf("Current Balance: " COLOR_GREEN "$%.2f" COLOR_RESET "\n", r.amount);
            printf("Account Type: %s\n", r.accountType);
            printSeparator();
            
            printf(COLOR_BOLD "Interest Information:\n" COLOR_RESET);
            calculateInterest(r);
            
            fclose(pf);
            return SUCCESS;
        }
    }
    
    fclose(pf);
    return ERROR_ACCOUNT_NOT_FOUND;
}

ErrorCode checkAccountBalance(struct User u, char *accountNum) {
    char userName[MAX_USERNAME_LENGTH];
    struct Record r;
    FILE *pf;
    
    printHeader("ACCOUNT BALANCE");
    
    pf = fopen(RECORDS_FILE, "r");
    if (pf == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0) {
            printf(COLOR_BOLD "Balance Information:\n" COLOR_RESET);
            printSeparator();
            printf("Account Owner: %s\n", userName);
            printf("Account Number: %s\n", r.accountNbr);
            printf("Current Balance: " COLOR_GREEN COLOR_BOLD "$%.2f" COLOR_RESET "\n", r.amount);
            printSeparator();
            
            fclose(pf);
            return SUCCESS;
        }
    }
    
    fclose(pf);
    return ERROR_ACCOUNT_NOT_FOUND;
}

int getUserId(char *name) {
    FILE *pf = fopen(USERS_FILE, "r");
    if (pf == NULL) {
        return -1;
    }
    
    char id[5];
    struct User temp;
    while (fscanf(pf, "%s %s %s", id, temp.name, temp.password) != EOF) {
        if (strcmp(temp.name, name) == 0) {
            fclose(pf);
            return atoi(id);
        }
    }
    
    fclose(pf);
    return -1;
}

ErrorCode updateAccountInfo(struct User u, char *accountNum, int choice) {
    char userName[MAX_USERNAME_LENGTH];
    struct Record r;
    struct Record arr[100];
    FILE *pf;
    int index = 0;
    ErrorCode result;
    
    printHeader("UPDATE ACCOUNT INFORMATION");
    
    pf = fopen(RECORDS_FILE, "a+");
    if (pf == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    while (getAccountFromFile(pf, userName, &r)) {
        strcpy(r.name, userName);
        
        if (strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0) {
            if (choice == 1) {
                // Update phone number
                while (1) {
                    result = getStringInput(r.phone, MAX_PHONE_LENGTH, 
                                           "Enter new phone number", false);
                    if (result != SUCCESS) {
                        handleError(result, "Phone number must contain only digits");
                        continue;
                    }
                    
                    if (strlen(r.phone) < 5 || strlen(r.phone) > 14) {
                        handleError(ERROR_INVALID_PHONE, "Phone number must be 5-14 digits");
                        continue;
                    }
                    
                    break;
                }
            } else if (choice == 2) {
                // Update country
                while (1) {
                    result = getStringInput(r.country, MAX_COUNTRY_LENGTH, 
                                           "Enter new country name", true);
                    if (result != SUCCESS) {
                        handleError(result, "Country name must contain only letters and spaces");
                        continue;
                    }
                    
                    if (strlen(r.country) < 2 || strlen(r.country) > 65) {
                        handleError(ERROR_INVALID_COUNTRY, "Country name must be 2-65 characters");
                        continue;
                    }
                    
                    sanitize(r.country);
                    break;
                }
            }
        }
        
        arr[index] = r;
        index++;
    }
    
    fclose(pf);
    
    // Rewrite file with updated data
    pf = fopen(RECORDS_FILE, "w");
    if (pf == NULL) {
        return ERROR_FILE_WRITE;
    }
    
    for (int i = 0; i < index; i++) {
        result = saveRecordToFile(pf, arr[i]);
        if (result != SUCCESS) {
            fclose(pf);
            return result;
        }
    }
    
    fclose(pf);
    return SUCCESS;
}

ErrorCode transactionAcc(struct User u, char *accountNum) {
    char userName[MAX_USERNAME_LENGTH];
    struct Record r;
    FILE *pf;
    
    pf = fopen(RECORDS_FILE, "r");
    if (pf == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0) {
            if (strcmp(r.accountType, "fixed01") == 0 ||
                strcmp(r.accountType, "fixed02") == 0 ||
                strcmp(r.accountType, "fixed03") == 0) {
                fclose(pf);
                return ERROR_TRANSACTION_NOT_ALLOWED;
            }
        }
    }
    
    fclose(pf);
    return SUCCESS;
}

ErrorCode makeTransaction(struct User u, char *accountNum, int choice) {
    char userName[MAX_USERNAME_LENGTH];
    struct Record r;
    struct Record arr[100];
    FILE *pf;
    int index = 0;
    double amount = 0;
    ErrorCode result;
    
    printHeader(choice == 1 ? "WITHDRAW MONEY" : "DEPOSIT MONEY");
    
    pf = fopen(RECORDS_FILE, "a+");
    if (pf == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    while (getAccountFromFile(pf, userName, &r)) {
        strcpy(r.name, userName);
        
        if (strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0) {
            printf("Current Balance: " COLOR_GREEN "$%.2f" COLOR_RESET "\n\n", r.amount);
            
            if (choice == 1) {
                // Withdraw
                while (1) {
                    result = getDoubleInput(&amount, "Enter withdrawal amount", 
                                           MIN_TRANSACTION_AMOUNT, r.amount);
                    if (result == SUCCESS) break;
                    if (result == ERROR_INVALID_AMOUNT) {
                        handleError(ERROR_INSUFFICIENT_FUNDS, 
                                   "Insufficient funds for this withdrawal");
                    } else {
                        handleError(result, "Please enter a valid amount");
                    }
                }
                r.amount -= amount;
            } else {
                // Deposit
                while (1) {
                    result = getDoubleInput(&amount, "Enter deposit amount", 
                                           MIN_TRANSACTION_AMOUNT, MAX_TRANSACTION_AMOUNT);
                    if (result == SUCCESS) break;
                    handleError(result, "Please enter a valid amount");
                }
                r.amount += amount;
            }
        }
        
        arr[index] = r;
        index++;
    }
    
    fclose(pf);
    
    // Rewrite file with updated data
    pf = fopen(RECORDS_FILE, "w");
    if (pf == NULL) {
        return ERROR_FILE_WRITE;
    }
    
    for (int i = 0; i < index; i++) {
        result = saveRecordToFile(pf, arr[i]);
        if (result != SUCCESS) {
            fclose(pf);
            return result;
        }
    }
    
    fclose(pf);
    
    printf("\nTransaction completed successfully!\n");
    printf("Amount %s: " COLOR_GREEN "$%.2f" COLOR_RESET "\n", 
           choice == 1 ? "withdrawn" : "deposited", amount);
    
    return SUCCESS;
}

ErrorCode removeAccount(struct User u, char *accountNum) {
    char userName[MAX_USERNAME_LENGTH];
    struct Record r;
    struct Record deleted;
    struct Record arr[100];
    FILE *pf;
    int index = 0;
    bool found = false;
    ErrorCode result;
    
    printHeader("REMOVE ACCOUNT");
    
    pf = fopen(RECORDS_FILE, "a+");
    if (pf == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    while (getAccountFromFile(pf, userName, &r)) {
        strcpy(r.name, userName);
        
        if (strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0) {
            deleted = r;
            found = true;
        } else {
            arr[index] = r;
            index++;
        }
    }
    
    fclose(pf);
    
    if (!found) {
        return ERROR_ACCOUNT_NOT_FOUND;
    }
    
    // Rewrite file without the deleted account
    pf = fopen(RECORDS_FILE, "w");
    if (pf == NULL) {
        return ERROR_FILE_WRITE;
    }
    
    for (int i = 0; i < index; i++) {
        result = saveRecordToFile(pf, arr[i]);
        if (result != SUCCESS) {
            fclose(pf);
            return result;
        }
    }
    
    fclose(pf);
    
    printf(COLOR_BOLD "Deleted Account Details:\n" COLOR_RESET);
    printSeparator();
    printf("Owner: %s\n", deleted.name);
    printf("Account Number: %s\n", deleted.accountNbr);
    printf("Final Balance: " COLOR_RED "$%.2f" COLOR_RESET "\n", deleted.amount);
    printf("Account Type: %s\n", deleted.accountType);
    printSeparator();
    
    return SUCCESS;
}

ErrorCode transferAccount(struct User u, char *accountNum) {
    char userName[MAX_USERNAME_LENGTH];
    char newOwnerName[MAX_USERNAME_LENGTH];
    struct Record r;
    struct Record arr[100];
    FILE *pf;
    int index = 0;
    int newOwnerId;
    ErrorCode result;
    
    printHeader("TRANSFER ACCOUNT OWNERSHIP");
    
    pf = fopen(RECORDS_FILE, "a+");
    if (pf == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    while (getAccountFromFile(pf, userName, &r)) {
        strcpy(r.name, userName);
        
        if (strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0) {
            printf(COLOR_BOLD "Account to Transfer:\n" COLOR_RESET);
            printSeparator();
            printf("Account Number: %s\n", r.accountNbr);
            printf("Balance: " COLOR_GREEN "$%.2f" COLOR_RESET "\n", r.amount);
            printf("Account Type: %s\n", r.accountType);
            printSeparator();
        }
        
        arr[index] = r;
        index++;
    }
    
    fclose(pf);
    
    // Get new owner name
    while (1) {
        result = getStringInput(newOwnerName, MAX_USERNAME_LENGTH, 
                               "Enter username of new owner", false);
        if (result != SUCCESS) {
            handleError(result, "Please enter a valid username");
            continue;
        }
        
        sanitize(newOwnerName);
        
        if (!isNamePresent(newOwnerName)) {
            handleError(ERROR_USER_NOT_FOUND, "User does not exist");
            continue;
        }
        
        break;
    }
    
    newOwnerId = getUserId(newOwnerName);
    if (newOwnerId == -1) {
        return ERROR_USER_NOT_FOUND;
    }
    
    // Update ownership
    for (int i = 0; i < index; i++) {
        if (strcmp(arr[i].name, u.name) == 0 && strcmp(arr[i].accountNbr, accountNum) == 0) {
            strcpy(arr[i].name, newOwnerName);
            arr[i].userId = newOwnerId;
            break;
        }
    }
    
    // Rewrite file with updated ownership
    pf = fopen(RECORDS_FILE, "w");
    if (pf == NULL) {
        return ERROR_FILE_WRITE;
    }
    
    for (int i = 0; i < index; i++) {
        result = saveRecordToFile(pf, arr[i]);
        if (result != SUCCESS) {
            fclose(pf);
            return result;
        }
    }
    
    fclose(pf);
    
    printf(COLOR_BOLD "Transfer completed successfully!\n" COLOR_RESET);
    printf("Account %s has been transferred to %s\n", accountNum, newOwnerName);
    
    return SUCCESS;
}