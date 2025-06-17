#include "header.h"

void printHeader(const char *title) {
    system("clear");
    printf("\n");
    printf(COLOR_CYAN COLOR_BOLD);
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    ATM MANAGEMENT SYSTEM                     ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║ %-60s ║\n", title);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf(COLOR_RESET);
    printf("\n");
}

void printSuccess(const char *message) {
    printf(COLOR_GREEN COLOR_BOLD "✓ SUCCESS: " COLOR_RESET COLOR_GREEN "%s" COLOR_RESET "\n", message);
}

void printError(const char *message) {
    printf(COLOR_RED COLOR_BOLD "✗ ERROR: " COLOR_RESET COLOR_RED "%s" COLOR_RESET "\n", message);
}

void printWarning(const char *message) {
    printf(COLOR_YELLOW COLOR_BOLD "⚠ WARNING: " COLOR_RESET COLOR_YELLOW "%s" COLOR_RESET "\n", message);
}

void printInfo(const char *message) {
    printf(COLOR_BLUE COLOR_BOLD "ℹ INFO: " COLOR_RESET COLOR_BLUE "%s" COLOR_RESET "\n", message);
}

void printSeparator(void) {
    printf(COLOR_CYAN "────────────────────────────────────────────────────────────────\n" COLOR_RESET);
}

ErrorCode getUserChoice(int *choice, int min, int max) {
    char input[10];
    char *endPtr;
    
    printf(COLOR_BOLD "Enter your choice (%d-%d): " COLOR_RESET, min, max);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    // Remove newline
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    } else {
        clearInputBuffer();
    }
    
    // Validate input is numeric
    if (!is_valid_number(input)) {
        return ERROR_INVALID_INPUT;
    }
    
    *choice = (int)strtol(input, &endPtr, 10);
    
    if (*choice < min || *choice > max) {
        return ERROR_INVALID_INPUT;
    }
    
    return SUCCESS;
}

ErrorCode getStringInput(char *buffer, int maxLength, const char *prompt, bool allowSpaces) {
    printf(COLOR_BOLD "%s: " COLOR_RESET, prompt);
    
    if (fgets(buffer, maxLength, stdin) == NULL) {
        return ERROR_INVALID_INPUT;
    }

    //debug
    printf("raw input: %s\n", buffer);
    
    // Remove newline
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    } else {
        clearInputBuffer();
    }
    
    // // Check if empty
    // if (strlen(buffer) == 0) {
    //     return ERROR_INVALID_INPUT;
    // }
    
    // // Only validate string content for text inputs that should contain letters
    // // For numeric inputs (like account numbers), skip this validation
    // // The specific validation will be done by the calling function
    // if (allowSpaces && !is_valid_string(buffer)) {
    //     return ERROR_INVALID_INPUT;
    // }
    if(!allowSpaces){
        return SUCCESS;
    }
    
    return SUCCESS;
}

ErrorCode getDoubleInput(double *value, const char *prompt, double min, double max) {
    char input[20];
    char *endPtr;
    
    printf(COLOR_BOLD "%s ($%.2f - $%.2f): $" COLOR_RESET, prompt, min, max);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    // Remove newline
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    } else {
        clearInputBuffer();
    }
    
    *value = strtod(input, &endPtr);
    
    if (endPtr == input || *endPtr != '\0') {
        return ERROR_INVALID_INPUT;
    }
    
    if (*value < min || *value > max) {
        return ERROR_INVALID_AMOUNT;
    }
    
    return SUCCESS;
}

ErrorCode getDateInput(struct Date *date, const char *prompt) {
    char input[20];
    char extra[10];
    int fields;
    
    printf(COLOR_BOLD "%s (MM/DD/YYYY): " COLOR_RESET, prompt);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    // Remove newline
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    } else {
        clearInputBuffer();
    }
    
    fields = sscanf(input, "%d/%d/%d %s", &date->month, &date->day, &date->year, extra);
    
    if (fields != 3) {
        return ERROR_INVALID_DATE;
    }
    
    // Validate date ranges
    if (date->month < 1 || date->month > 12 ||
        date->day < 1 || date->day > 31 ||
        date->year < 1900 || date->year > 3000) {
        return ERROR_INVALID_DATE;
    }
    
    return SUCCESS;
}

void returnToMainMenu(struct User u) {
    int choice;
    printf("\n");
    printSeparator();
    printf(COLOR_BOLD "Press 1 to return to main menu, 0 to exit: " COLOR_RESET);
    
    ErrorCode result = getUserChoice(&choice, 0, 1);
    if (result != SUCCESS || choice == 0) {
        printInfo("Thank you for using ATM Management System!");
        exit(0);
    }
    
    mainMenu(u);
}