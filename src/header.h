#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>
#include <time.h>

// Configuration constants
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_ACCOUNT_NUMBER_LENGTH 20
#define MAX_COUNTRY_LENGTH 70
#define MAX_PHONE_LENGTH 16
#define MAX_ACCOUNT_TYPE_LENGTH 10
#define MIN_PASSWORD_LENGTH 4
#define MAX_TRANSACTION_AMOUNT 9999999999.99
#define MIN_TRANSACTION_AMOUNT 1.0

// File paths
#define USERS_FILE "./data/users.txt"
#define RECORDS_FILE "./data/records.txt"

// ANSI Color codes for better UI
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BOLD "\033[1m"

// Error codes
typedef enum {
    SUCCESS = 0,
    ERROR_FILE_NOT_FOUND,
    ERROR_INVALID_INPUT,
    ERROR_INSUFFICIENT_FUNDS,
    ERROR_ACCOUNT_NOT_FOUND,
    ERROR_USER_NOT_FOUND,
    ERROR_DUPLICATE_ACCOUNT,
    ERROR_DUPLICATE_USER,
    ERROR_INVALID_CREDENTIALS,
    ERROR_TRANSACTION_NOT_ALLOWED,
    ERROR_MEMORY_ALLOCATION,
    ERROR_FILE_WRITE,
    ERROR_FILE_READ,
    ERROR_INVALID_AMOUNT,
    ERROR_INVALID_DATE,
    ERROR_INVALID_PHONE,
    ERROR_INVALID_COUNTRY
} ErrorCode;

// Structures
struct Date {
    int month, day, year;
};

struct Record {
    int id;
    int userId;
    char name[MAX_USERNAME_LENGTH];
    char country[MAX_COUNTRY_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char accountType[MAX_ACCOUNT_TYPE_LENGTH];
    char accountNbr[MAX_ACCOUNT_NUMBER_LENGTH];
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User {
    int id;
    char name[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

// Function prototypes

// Authentication functions
ErrorCode loginMenu(char *username, char *password);
ErrorCode signUpMenu(char *username, char *password);
const char *getPassword(struct User *u);

// System functions
void mainMenu(struct User u);
ErrorCode createNewAcc(struct User u);
ErrorCode transferAccount(struct User u, char *accountNum);
ErrorCode checkDetailOfAccount(struct User u, char *accountID);
ErrorCode updateAccountInfo(struct User u, char *accountNum, int choice);
ErrorCode makeTransaction(struct User u, char *accountNum, int choice);
ErrorCode removeAccount(struct User u, char *accountNum);
ErrorCode checkAllAccounts(struct User u);
ErrorCode checkAccountBalance(struct User u, char *accountID);
ErrorCode transactionAcc(struct User u, char *accountNum);

// Utility functions
bool isNamePresent(char *name);
bool isPresent(char array[100][100], char* str);
bool isAccountPresent(char *num, struct User u);
bool isAccountPresentAny(char *num, struct User u);
int getUserId(char *name);
int getRecordId(char *name);
bool checkUsers(void);
bool checkRecords(void);
void alphamirror(char *str);
bool is_valid_number(char *num);
bool is_valid_string(char *strInput);
void clearInputBuffer(void);
void sanitize(char *str);

// UI functions
void printHeader(const char *title);
void printSuccess(const char *message);
void printError(const char *message);
void printWarning(const char *message);
void printInfo(const char *message);
void printSeparator(void);
ErrorCode getUserChoice(int *choice, int min, int max);
ErrorCode getStringInput(char *buffer, int maxLength, const char *prompt, bool allowSpaces);
ErrorCode getDoubleInput(double *value, const char *prompt, double min, double max);
ErrorCode getDateInput(struct Date *date, const char *prompt);

// Error handling functions
void handleError(ErrorCode error, const char *context);
const char *getErrorMessage(ErrorCode error);
void logError(const char *function, ErrorCode error, const char *details);

// File operations
ErrorCode saveAccountToFile(FILE *ptr, struct User u, struct Record r);
ErrorCode saveRecordToFile(FILE *ptr, struct Record r);
int getAccountFromFile(FILE *ptr, char name[MAX_USERNAME_LENGTH], struct Record *r);
int getAccountFromUser(FILE *ptr, char name[MAX_USERNAME_LENGTH], struct User *r);

// Navigation functions
void stayOrReturn(ErrorCode result, void (*func)(struct User), struct User u);
void returnToMainMenu(struct User u);

#endif // HEADER_H