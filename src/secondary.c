#include "header.h"
#include <ctype.h>

// replace space character in the string with an underscore
void sanitize(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            str[i] = '_';
        }
    }
}

// clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

// password encryption function
void alphamirror(char *str) {
    char c;
    while (*str) {
        c = *str;
        if (isalpha(c)) {
            if (islower(c)) {
                // For lowercase letters: 'a' becomes 'z', 'b' becomes 'y', etc.
                *str = 'a' + ('z' - c);
            } else if (isupper(c)) {
                // For uppercase letters: 'A' becomes 'Z', 'B' becomes 'Y', etc.
                *str = 'A' + ('Z' - c);
            }
        }
        str++;
    }
}

// check if all the characters of the string are digits
bool is_valid_number(char *num) {
    // Check each character to ensure it's a digit
    for (int i = 0; i < strlen(num); i++) {
        if (!isdigit(num[i])) {
            return false;  // Invalid if a non-digit is found
        }
    }

    return true;  // Valid if all characters are digits
}

// check if all the characters of the string are alphabets
bool is_valid_string(char *strInput) {
    if (strlen(strInput) < 1) {
        return false;
    }
    for (int i = 0; i < strlen(strInput); i++) {
        if (!isalpha(strInput[i]) && (strInput[i] != ' ')) {
            return false;
        }
    }

    return true;
}

// check if the value is available in storage
bool isPresent(char array[100][100], char* str){
    for (int i = 0; i < 100; i++){
        if (strcmp(array[i], str) == 0){
            return true;
        }
    }

    return false;
}

// check if the user logged already has an account with the account number being entered
bool isAccountPresent(char *num, struct User u){
    struct Record r;
    FILE *pf = fopen("./data/records.txt", "r+");
    if (pf == NULL) {
        // Handle file opening error
        return false;
    }

    while(fscanf(pf, "%d %d %s %s %d/%d/%d %s %s %lf %s",
        &r.id, &r.userId, r.name, r.accountNbr,
        &r.deposit.month, &r.deposit.day,
        &r.deposit.year, r.country,
        r.phone, &r.amount, r.accountType) != EOF){
        
        char numStr[21]; // Buffer to hold the string representation (char can be up to 20 digits)
        char accountNbrStr[21];
        sprintf(numStr, "%s", num);
        sprintf(accountNbrStr, "%s", r.accountNbr);
        if((strcmp(num, r.accountNbr) == 0 && strcmp(r.name, u.name) == 0 && (strlen(numStr) == strlen(accountNbrStr)))){
            return true;
        }
        
    }

    return false;
}

// check if the account number being entered has already been used for any account
bool isAccountPresentAny(char *num, struct User u){
    struct Record r;
    FILE *pf = fopen("./data/records.txt", "r+");

    while(fscanf(pf, "%d %d %s %s %d/%d/%d %s %s %lf %s",
        &r.id, &r.userId, r.name, r.accountNbr,
        &r.deposit.month, &r.deposit.day,
        &r.deposit.year, r.country,
        r.phone, &r.amount, r.accountType) != EOF){
        
        if(strcmp(num, r.accountNbr) == 0){
            return true;
        }
    }
    return false;
}

// check if the name has already been used
bool isNamePresent(char *name){
    char id[5];
    char userName[100];
    char password[100];

    FILE *pf = fopen("./data/users.txt", "r");
    while(fscanf(pf, "%s %s %s", id, userName, password) != EOF){
        if(strcmp(name, userName) == 0){
            return true;
        }
    }
    return false;
}

// check if the user credentials being entered are registered
bool checkUsers(){
    FILE* pf = fopen("./data/users.txt", "r+");

    char id[100];
    struct User temp;
    int userPosition = 0;
    int userID = 0;
    float floatID = 0;

    while(fscanf(pf, "%s %s %s", id, temp.name, temp.password) != EOF){
        floatID = atof(id);
        userID = atoi(id);
        
        if(userID == 0 && userPosition != 0){
            return false;
        }
        else if(userID != userPosition || userID != floatID){
            return false;
        }
        else if(strcmp(temp.name, "") == 0 || strcmp(temp.name, " ") == 0){
            return false;
        }
        else if(strcmp(temp.password, "") == 0 || strcmp(temp.password, " ") == 0){
            return false;
        }
        userPosition++;
    }

    return true;

}

bool checkRecords(){
    FILE *ptr = fopen("./data/records.txt", "r");

    char id[100];
    char userId[100];
    char name[100];
    char accountNbr[100];
    int depositDay = -1;
    int depositMonth = -1;
    int depositYear = -1;
    char country[100];
    char phone[100];
    char amount[100];
    char accountType[100];
    int index = 0;
    while(fscanf(ptr, "%s %s %s %s %d/%d/%d %s %s %s %s",
        id, userId, name, accountNbr,
        &depositMonth, &depositDay,
        &depositYear, country, phone,
        amount, accountType) != EOF){
        int intId = atoi(id);
        float floatId = atof(id);
        int intUserId = atoi(userId);
        float floatUserId = atof(userId);
        
        if(intId == 0 && strcmp(id, "0") != 0){
            printf("1");
            return false;
        }
        else if(intId != index || intId != floatId){
            printf("2");
            return false;
        }
        else if (!isNamePresent(name)){
            printf("3");
            return false;
        }
       else if(getUserId(name) != intUserId){
            printf("4");
            return false;
        } 
        else if(strcmp(accountType, "current") != 0 &&
            strcmp(accountType, "fixed01") != 0 &&
            strcmp(accountType, "fixed02") != 0 &&
            strcmp(accountType, "fixed03") != 0 &&
            strcmp(accountType, "savings") != 0){
            printf("5");
            return false;
        }
        else if((depositMonth < 1 || depositMonth > 12) ||
            (depositDay < 1 || depositDay > 31) ||
            (depositYear < 1900 || depositYear > 2900)){
            return false;
        }
        

        index++;
    }
    return true;
}