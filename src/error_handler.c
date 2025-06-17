#include "header.h"

const char *getErrorMessage(ErrorCode error) {
    switch (error) {
        case SUCCESS:
            return "Operation completed successfully";
        case ERROR_FILE_NOT_FOUND:
            return "Required file not found";
        case ERROR_INVALID_INPUT:
            return "Invalid input provided";
        case ERROR_INSUFFICIENT_FUNDS:
            return "Insufficient funds for this transaction";
        case ERROR_ACCOUNT_NOT_FOUND:
            return "Account not found";
        case ERROR_USER_NOT_FOUND:
            return "User not found";
        case ERROR_DUPLICATE_ACCOUNT:
            return "Account number already exists";
        case ERROR_DUPLICATE_USER:
            return "Username already exists";
        case ERROR_INVALID_CREDENTIALS:
            return "Invalid username or password";
        case ERROR_TRANSACTION_NOT_ALLOWED:
            return "Transaction not allowed for this account type";
        case ERROR_MEMORY_ALLOCATION:
            return "Memory allocation failed";
        case ERROR_FILE_WRITE:
            return "Failed to write to file";
        case ERROR_FILE_READ:
            return "Failed to read from file";
        case ERROR_INVALID_AMOUNT:
            return "Invalid amount specified";
        case ERROR_INVALID_DATE:
            return "Invalid date format or values";
        case ERROR_INVALID_PHONE:
            return "Invalid phone number format";
        case ERROR_INVALID_COUNTRY:
            return "Invalid country name";
        default:
            return "Unknown error occurred";
    }
}

void handleError(ErrorCode error, const char *context) {
    if (error == SUCCESS) {
        return;
    }
    
    printf("\n");
    printError(getErrorMessage(error));
    
    if (context && strlen(context) > 0) {
        printf(COLOR_RED "Context: %s" COLOR_RESET "\n", context);
    }
    
    logError(__func__, error, context);
}

void logError(const char *function, ErrorCode error, const char *details) {
    FILE *logFile = fopen("./data/error.log", "a");
    if (logFile == NULL) {
        return; // Can't log if we can't open the log file
    }
    
    time_t now;
    time(&now);
    char *timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0'; // Remove newline
    
    fprintf(logFile, "[%s] Function: %s, Error: %s", 
            timeStr, function, getErrorMessage(error));
    
    if (details && strlen(details) > 0) {
        fprintf(logFile, ", Details: %s", details);
    }
    
    fprintf(logFile, "\n");
    fclose(logFile);
}

void stayOrReturn(ErrorCode result, void (*func)(struct User), struct User u) {
    int choice;
    
    if (result != SUCCESS) {
        printf("\n");
        printError("Operation failed!");
        printf("\nOptions:\n");
        printf("0 - Try again\n");
        printf("1 - Return to main menu\n");
        printf("2 - Exit\n");
        
        ErrorCode inputResult = getUserChoice(&choice, 0, 2);
        if (inputResult != SUCCESS) {
            printError("Invalid choice! Returning to main menu.");
            mainMenu(u);
            return;
        }
        
        switch (choice) {
            case 0:
                if (func != NULL) {
                    func(u);
                } else {
                    mainMenu(u);
                }
                break;
            case 1:
                mainMenu(u);
                break;
            case 2:
                printInfo("Thank you for using ATM Management System!");
                exit(0);
                break;
        }
    } else {
        printSuccess("Operation completed successfully!");
        returnToMainMenu(u);
    }
}