#include "header.h"

void displayMainMenu(void) {
    printHeader("MAIN MENU");
    printf(COLOR_BOLD "Available Operations:\n" COLOR_RESET);
    printf("  1. Create a new account\n");
    printf("  2. Update account information\n");
    printf("  3. Check account details\n");
    printf("  4. Check list of all owned accounts\n");
    printf("  5. Make transaction\n");
    printf("  6. Remove existing account\n");
    printf("  7. Transfer ownership\n");
    printf("  8. Check account balance\n");
    printf("  9. Exit\n");
    printf("\n");
}

ErrorCode getAccountNumber(char *accountNum, const char *prompt) {
    ErrorCode result = getStringInput(accountNum, MAX_ACCOUNT_NUMBER_LENGTH, prompt, false);
    if (result != SUCCESS) {
        return ERROR_INVALID_INPUT;
    }
    
    if (strlen(accountNum) < 1 || strlen(accountNum) > 18) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!is_valid_number(accountNum)) {
        return ERROR_INVALID_INPUT;
    }
    
    return SUCCESS;
}

ErrorCode handleAccountOperation(struct User u, const char *prompt, 
                                ErrorCode (*operation)(struct User, char*)) {
    char accountNum[MAX_ACCOUNT_NUMBER_LENGTH];
    
    ErrorCode result = getAccountNumber(accountNum, prompt);
    if (result != SUCCESS) {
        handleError(result, "Invalid account number format");
        return result;
    }
    
    if (!isAccountPresent(accountNum, u)) {
        handleError(ERROR_ACCOUNT_NOT_FOUND, "Account does not exist for this user");
        return ERROR_ACCOUNT_NOT_FOUND;
    }
    
    return operation(u, accountNum);
}

ErrorCode handleUpdateAccount(struct User u) {
    char accountNum[MAX_ACCOUNT_NUMBER_LENGTH];
    int choice;
    
    ErrorCode result = getAccountNumber(accountNum, "Enter account number to update");
    if (result != SUCCESS) {
        handleError(result, "Invalid account number");
        return result;
    }
    
    if (!isAccountPresent(accountNum, u)) {
        handleError(ERROR_ACCOUNT_NOT_FOUND, "Account not found");
        return ERROR_ACCOUNT_NOT_FOUND;
    }
    
    printf("\nWhat would you like to update?\n");
    printf("1. Phone number\n");
    printf("2. Country\n");
    
    result = getUserChoice(&choice, 1, 2);
    if (result != SUCCESS) {
        handleError(result, "Invalid choice");
        return result;
    }
    
    return updateAccountInfo(u, accountNum, choice);
}

ErrorCode handleTransaction(struct User u) {
    char accountNum[MAX_ACCOUNT_NUMBER_LENGTH];
    int choice;
    
    ErrorCode result = getAccountNumber(accountNum, "Enter account number for transaction");
    if (result != SUCCESS) {
        handleError(result, "Invalid account number");
        return result;
    }
    
    if (!isAccountPresent(accountNum, u)) {
        handleError(ERROR_ACCOUNT_NOT_FOUND, "Account not found");
        return ERROR_ACCOUNT_NOT_FOUND;
    }
    
    // Check if transactions are allowed for this account
    result = transactionAcc(u, accountNum);
    if (result != SUCCESS) {
        return result;
    }
    
    printf("\nTransaction Type:\n");
    printf("1. Withdraw\n");
    printf("2. Deposit\n");
    
    result = getUserChoice(&choice, 1, 2);
    if (result != SUCCESS) {
        handleError(result, "Invalid transaction type");
        return result;
    }
    
    return makeTransaction(u, accountNum, choice);
}

void mainMenu(struct User u) {
    int option;
    ErrorCode result;
    
    while (1) {
        displayMainMenu();
        
        result = getUserChoice(&option, 1, 9);
        if (result != SUCCESS) {
            handleError(result, "Please enter a valid option (1-9)");
            continue;
        }
        
        switch (option) {
            case 1:
                result = createNewAcc(u);
                stayOrReturn(result, NULL, u);
                break;
                
            case 2:
                result = handleUpdateAccount(u);
                stayOrReturn(result, NULL, u);
                break;
                
            case 3:
                result = handleAccountOperation(u, "Enter account number to check details", 
                                              checkDetailOfAccount);
                stayOrReturn(result, NULL, u);
                break;
                
            case 4:
                result = checkAllAccounts(u);
                stayOrReturn(result, NULL, u);
                break;
                
            case 5:
                result = handleTransaction(u);
                stayOrReturn(result, NULL, u);
                break;
                
            case 6:
                result = handleAccountOperation(u, "Enter account number to remove", 
                                              removeAccount);
                stayOrReturn(result, NULL, u);
                break;
                
            case 7:
                result = handleAccountOperation(u, "Enter account number to transfer", 
                                              transferAccount);
                stayOrReturn(result, NULL, u);
                break;
                
            case 8:
                result = handleAccountOperation(u, "Enter account number to check balance", 
                                              checkAccountBalance);
                stayOrReturn(result, NULL, u);
                break;
                
            case 9:
                printInfo("Thank you for using ATM Management System!");
                exit(0);
                break;
                
            default:
                printError("Invalid operation! Please choose from the listed options.");
                sleep(2);
                break;
        }
    }
}

void displayInitMenu(void) {
    printHeader("WELCOME");
    printf(COLOR_BOLD "Please choose an option:\n" COLOR_RESET);
    printf("  1. Login\n");
    printf("  2. Register\n");
    printf("  3. Exit\n");
    printf("\n");
}

ErrorCode initMenu(struct User *u) {
    int option;
    ErrorCode result;
    
    while (1) {
        displayInitMenu();
        
        result = getUserChoice(&option, 1, 3);
        if (result != SUCCESS) {
            handleError(result, "Please enter a valid option (1-3)");
            continue;
        }
        
        switch (option) {
            case 1:
                result = loginMenu(u->name, u->password);
                if (result == SUCCESS) {
                    if (strcmp(u->password, getPassword(u)) == 0) {
                        printSuccess("Login successful!");
                        sleep(1);
                        return SUCCESS;
                    } else {
                        handleError(ERROR_INVALID_CREDENTIALS, "Please check your credentials");
                        sleep(2);
                    }
                } else {
                    handleError(result, "Login failed");
                    sleep(2);
                }
                break;
                
            case 2:
                result = signUpMenu(u->name, u->password);
                if (result == SUCCESS) {
                    printSuccess("Registration successful! You can now login.");
                    sleep(2);
                } else {
                    handleError(result, "Registration failed");
                    sleep(2);
                }
                break;
                
            case 3:
                printInfo("Thank you for using ATM Management System!");
                exit(0);
                break;
        }
    }
}

int main(void) {
    struct User u;
    ErrorCode result;
    
    // Initialize user structure
    memset(&u, 0, sizeof(struct User));
    
    printInfo("Starting ATM Management System...");
    sleep(1);
    
    result = initMenu(&u);
    if (result == SUCCESS) {
        mainMenu(u);
    } else {
        handleError(result, "Failed to initialize system");
        return 1;
    }
    
    return 0;
}