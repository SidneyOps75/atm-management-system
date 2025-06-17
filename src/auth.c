#include "header.h"

ErrorCode validatePassword(const char *password) {
    if (strlen(password) < MIN_PASSWORD_LENGTH) {
        return ERROR_INVALID_INPUT;
    }
    return SUCCESS;
}

ErrorCode validateUsername(const char *username) {
    if (strlen(username) < 2 || strlen(username) >= MAX_USERNAME_LENGTH) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!is_valid_string((char*)username)) {
        return ERROR_INVALID_INPUT;
    }
    
    return SUCCESS;
}

ErrorCode signUpMenu(char *username, char *password) {
    FILE *fp;
    char names[100][100];
    struct User userChecker;
    char id[5];
    int counter = 0;
    ErrorCode result;
    
    printHeader("USER REGISTRATION");
    
    // Get username
    while (1) {
        result = getStringInput(username, MAX_USERNAME_LENGTH, "Enter Username", false);
        if (result != SUCCESS) {
            handleError(result, "Username must contain only letters and spaces");
            continue;
        }
        
        result = validateUsername(username);
        if (result != SUCCESS) {
            handleError(result, "Username must be 2-49 characters long");
            continue;
        }
        
        sanitize(username);
        break;
    }
    
    // Get password
    while (1) {
        printf(COLOR_BOLD "Enter Password (minimum %d characters): " COLOR_RESET, MIN_PASSWORD_LENGTH);
        
        if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL) {
            handleError(ERROR_INVALID_INPUT, "Failed to read password");
            continue;
        }
        
        // Remove newline
        size_t len = strlen(password);
        if (len > 0 && password[len-1] == '\n') {
            password[len-1] = '\0';
        } else {
            clearInputBuffer();
        }
        
        result = validatePassword(password);
        if (result != SUCCESS) {
            handleError(result, "Password too short");
            continue;
        }
        
        sanitize(password);
        alphamirror(password);
        break;
    }
    
    // Open users file
    fp = fopen(USERS_FILE, "a+");
    if (fp == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Read existing users
    while (fscanf(fp, "%s %s %s", id, userChecker.name, userChecker.password) != EOF) {
        strcpy(names[counter], userChecker.name);
        counter++;
    }
    
    // Check if user already exists
    if (isPresent(names, username)) {
        fclose(fp);
        return ERROR_DUPLICATE_USER;
    }
    
    // Add newline if file is not empty
    if (counter != 0) {
        fprintf(fp, "\n");
    }
    
    // Write new user
    if (fprintf(fp, "%d %s %s", counter, username, password) < 0) {
        fclose(fp);
        return ERROR_FILE_WRITE;
    }
    
    fclose(fp);
    return SUCCESS;
}

ErrorCode loginMenu(char *username, char *password) {
    struct termios oflags, nflags;
    ErrorCode result;
    
    printHeader("USER LOGIN");
    
    // Get username
    while (1) {
        result = getStringInput(username, MAX_USERNAME_LENGTH, "Enter Username", false);
        if (result != SUCCESS) {
            handleError(result, "Please enter a valid username");
            continue;
        }
        
        sanitize(username);
        break;
    }
    
    // Disable echo for password input
    tcgetattr(STDIN_FILENO, &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    
    if (tcsetattr(STDIN_FILENO, TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        return ERROR_INVALID_INPUT;
    }
    
    printf(COLOR_BOLD "Enter Password: " COLOR_RESET);
    if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL) {
        // Restore terminal
        tcsetattr(STDIN_FILENO, TCSANOW, &oflags);
        return ERROR_INVALID_INPUT;
    }
    
    // Remove newline
    size_t len = strlen(password);
    if (len > 0 && password[len-1] == '\n') {
        password[len-1] = '\0';
    } else {
        clearInputBuffer();
    }
    
    sanitize(password);
    alphamirror(password);
    
    // Restore terminal
    if (tcsetattr(STDIN_FILENO, TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        return ERROR_INVALID_INPUT;
    }
    
    return SUCCESS;
}

const char *getPassword(struct User *u) {
    FILE *fp;
    struct User userChecker;
    char line[150];
    char id[10];
    
    fp = fopen(USERS_FILE, "r");
    if (fp == NULL) {
        return "file_error";
    }
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%s %s %s", id, userChecker.name, userChecker.password) == 3) {
            if (strcmp(userChecker.name, u->name) == 0) {
                fclose(fp);
                u->id = atoi(id);
                static char password[MAX_PASSWORD_LENGTH];
                strcpy(password, userChecker.password);
                return password;
            }
        }
    }
    
    fclose(fp);
    return "no_user_found";
}