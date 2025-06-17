#include "header.h"

void sanitize(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            str[i] = '_';
        }
    }
}

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Clear buffer
    }
}

void alphamirror(char *str) {
    char c;
    while (*str) {
        c = *str;
        if (isalpha(c)) {
            if (islower(c)) {
                *str = 'a' + ('z' - c);
            } else if (isupper(c)) {
                *str = 'A' + ('Z' - c);
            }
        }
        str++;
    }
}

bool is_valid_number(char *num) {

    printf("valid number: %s\n", num);
    if (num == NULL || strlen(num) == 0) {
        return false;
    }
    
    for (size_t i = 0; i < strlen(num); i++) {
        if (!isdigit(num[i])) {
             printf("Found non-digit at position %zu: %c\n", i, num[i]);
            return false;
        }
    }
    
    return true;
}

bool is_valid_string(char *strInput) {
    if (strInput == NULL || strlen(strInput) < 1) {
        return false;
    }
    
    for (size_t i = 0; i < strlen(strInput); i++) {
        if (!isalpha(strInput[i]) && strInput[i] != ' ') {
            return false;
        }
    }
    
    return true;
}

bool isPresent(char array[100][100], char* str) {
    if (str == NULL) {
        return false;
    }
    
    for (int i = 0; i < 100; i++) {
        if (strcmp(array[i], str) == 0) {
            return true;
        }
    }
    
    return false;
}

bool isAccountPresent(char *num, struct User u) {
    struct Record r;
    FILE *pf;
    
    if (num == NULL) {
        return false;
    }
    
    pf = fopen(RECORDS_FILE, "r");
    if (pf == NULL) {
        return false;
    }
    
    while (fscanf(pf, "%d %d %s %s %d/%d/%d %s %s %lf %s",
                  &r.id, &r.userId, r.name, r.accountNbr,
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, r.phone, &r.amount, r.accountType) != EOF) {
        
        if (strcmp(num, r.accountNbr) == 0 && strcmp(r.name, u.name) == 0) {
            fclose(pf);
            return true;
        }
    }
    
    fclose(pf);
    return false;
}

bool isAccountPresentAny(char *num, struct User u __attribute__((unused))) {
    struct Record r;
    FILE *pf;
    
    if (num == NULL) {
        return false;
    }
    
    pf = fopen(RECORDS_FILE, "r");
    if (pf == NULL) {
        return false;
    }
    
    while (fscanf(pf, "%d %d %s %s %d/%d/%d %s %s %lf %s",
                  &r.id, &r.userId, r.name, r.accountNbr,
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, r.phone, &r.amount, r.accountType) != EOF) {
        
        if (strcmp(num, r.accountNbr) == 0) {
            fclose(pf);
            return true;
        }
    }
    
    fclose(pf);
    return false;
}

bool isNamePresent(char *name) {
    char id[5];
    char userName[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    FILE *pf;
    
    if (name == NULL) {
        return false;
    }
    
    pf = fopen(USERS_FILE, "r");
    if (pf == NULL) {
        return false;
    }
    
    while (fscanf(pf, "%s %s %s", id, userName, password) != EOF) {
        if (strcmp(name, userName) == 0) {
            fclose(pf);
            return true;
        }
    }
    
    fclose(pf);
    return false;
}

bool checkUsers(void) {
    FILE* pf;
    char id[100];
    struct User temp;
    int userPosition = 0;
    int userID = 0;
    float floatID = 0;
    
    pf = fopen(USERS_FILE, "r");
    if (pf == NULL) {
        return false;
    }
    
    while (fscanf(pf, "%s %s %s", id, temp.name, temp.password) != EOF) {
        floatID = atof(id);
        userID = atoi(id);
        
        if (userID == 0 && userPosition != 0) {
            fclose(pf);
            return false;
        } else if (userID != userPosition || userID != floatID) {
            fclose(pf);
            return false;
        } else if (strcmp(temp.name, "") == 0 || strcmp(temp.name, " ") == 0) {
            fclose(pf);
            return false;
        } else if (strcmp(temp.password, "") == 0 || strcmp(temp.password, " ") == 0) {
            fclose(pf);
            return false;
        }
        userPosition++;
    }
    
    fclose(pf);
    return true;
}

bool checkRecords(void) {
    FILE *ptr;
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
    
    ptr = fopen(RECORDS_FILE, "r");
    if (ptr == NULL) {
        return false;
    }
    
    while (fscanf(ptr, "%s %s %s %s %d/%d/%d %s %s %s %s",
                  id, userId, name, accountNbr,
                  &depositMonth, &depositDay, &depositYear,
                  country, phone, amount, accountType) != EOF) {
        
        int intId = atoi(id);
        float floatId = atof(id);
        int intUserId = atoi(userId);
        
        if (intId == 0 && strcmp(id, "0") != 0) {
            fclose(ptr);
            return false;
        } else if (intId != index || intId != floatId) {
            fclose(ptr);
            return false;
        } else if (!isNamePresent(name)) {
            fclose(ptr);
            return false;
        } else if (getUserId(name) != intUserId) {
            fclose(ptr);
            return false;
        } else if (strcmp(accountType, "current") != 0 &&
                   strcmp(accountType, "fixed01") != 0 &&
                   strcmp(accountType, "fixed02") != 0 &&
                   strcmp(accountType, "fixed03") != 0 &&
                   strcmp(accountType, "savings") != 0) {
            fclose(ptr);
            return false;
        } else if ((depositMonth < 1 || depositMonth > 12) ||
                   (depositDay < 1 || depositDay > 31) ||
                   (depositYear < 1900 || depositYear > 2900)) {
            fclose(ptr);
            return false;
        }
        
        index++;
    }
    
    fclose(ptr);
    return true;
}

int getRecordId(char *name __attribute__((unused))) {
    // This function can be implemented if needed for future features
    return -1;
}