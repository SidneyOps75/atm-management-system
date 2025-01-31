#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define USERS_FILE "data/users.txt"
#define RECORDS_FILE "data/records.txt"

// Structs
typedef struct {
    int day, month, year;
} Date;

typedef struct {
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    Date deposit;
    Date withdraw;
} Record;

typedef struct {
    int id;
    char name[50];
    char password[50];
} User;

// Authentication functions
void loginMenu(User *u);
void registerMenu(User *u);
const char *getPassword(User u);

// System functions
void createNewAcc(User u);
void mainMenu(User u);
void checkAllAccounts(User u);
void updateAccountInfo(User u, int accountId);
void checkAccountDetails(User u, int accountId);
//void makeTransaction(User u, int accountId, char action[], double amount);
void removeAccount(User u, int accountId);
void transferOwnership(User u, int accountId, int newOwnerId);
int getNextRecordId();
void clearInputBuffer();
void checkAccountDetails(User u, int accountId);
void makeTransaction(int userId, int accountId, int transactionType);

#endif // HEADER_H