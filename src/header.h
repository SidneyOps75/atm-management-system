#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[50];
    char country[70];
    char phone[16];
    char accountType[10];
   // char accountNbr;
   char accountNbr[20];
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// authentication functions
void loginMenu(char a[50], char pass[50]);
void signUpMenu(char a[50], char pass[50]);
const char *getPassword(struct User *u);

// system functions
void mainMenu(struct User u);
void createNewAcc(struct User u);
void transferAccount(struct User u, char *accountNum);
void checkDetailOfAccount(struct User u, char *accountID);
void updateAccountInfo(struct User u, char *accountNum, int choice);
void makeTransaction(struct User u, char *accountNum, int choice);
void removeAccount(struct  User u, char *accountNum);
void checkAllAccounts(struct User u);
void checkAccountBalance(struct User u, char *accountID);
void transactionAcc(struct User u, char *accountNum);



//utility functions
bool isNamePresent(char *name);
bool isPresent(char array[100][100], char* str);
bool isAccountPresent(char *num, struct User u);
bool isAccountPresentAny(char *num, struct User u);
int getUserId(char *name);
int getRecordId(char *name);
bool checkUsers();
bool checkRecords();
void alphamirror(char *str);
bool is_valid_number();
bool is_valid_string(char *strInput);
void clearInputBuffer();
void sanitize(char *str);
