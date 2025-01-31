#include <termios.h>
#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void registerMenu(User *u) {
    FILE *fp = fopen(USERS_FILE, "a+");
    if (!fp) {
        perror("Error opening file");
        exit(1);
    }

    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Register <<--\n");

    // Check if username already exists
    rewind(fp); // Move file pointer to the beginning
    User existingUser;
    while (fscanf(fp, "%d %49s %49s", &existingUser.id, existingUser.name, existingUser.password) != EOF) {
        if (strcmp(existingUser.name, u->name) == 0) {
            printf("\nUsername already exists! Please choose a different username.\n");
            fclose(fp);
            return;
        }
    }

    // Get new user details
    printf("\nEnter Username: ");
    scanf("%49s", u->name);

    printf("\nEnter Password: ");
    scanf("%49s", u->password);

    // Assign a unique ID
    int nextId = 0;
    rewind(fp); // Reset file pointer to the beginning
    while (fscanf(fp, "%d %*s %*s", &existingUser.id) != EOF) {
        if (existingUser.id >= nextId) {
            nextId = existingUser.id + 1;
        }
    }

    // Save the new user to the file
    fprintf(fp, "%d %s %s\n", nextId, u->name, u->password);
    fclose(fp);

    printf("\nRegistration successful!\n");
}

void loginMenu(User *u) {
    struct termios oflags, nflags;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Login <<--\n");

    printf("\nEnter Username: ");
    scanf("%49s", u->name);

    // Disable echo for password input
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    printf("\nEnter Password: ");
    scanf("%49s", u->password);

    // Restore terminal settings
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }
}

const char *getPassword(User u) {
    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) {
        perror("Error opening file");
        exit(1);
    }

    User userChecker;
    while (fscanf(fp, "%d %49s %49s", &userChecker.id, userChecker.name, userChecker.password) != EOF) {
        if (strcmp(userChecker.name, u.name) == 0) {
            fclose(fp);
            
            char *password = malloc(strlen(userChecker.password) + 1);
            if (!password) {
                perror("malloc: failed to allocate memory");
                exit(1);
            }
            strcpy(password, userChecker.password);
            return password;
        }
    }

    fclose(fp);
    return "no user found";
}