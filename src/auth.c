#include <termios.h>
#include "header.h"
#include <unistd.h>

char *USERS = "./data/users.txt";

// display signup menu
void signUpMenu(char a[50], char pass[50]){
    struct termios oflags, nflags;

    system("clear");
    signup:
    printf("\n\n\n\t\t\t\t  ============== REGISTER =============="); 
    clearInputBuffer();
    username:
    printf("\n\n\t\t\t\tEnter Username:");
    fgets(a, 50, stdin);

    // Remove the newline character if it's read by fgets
    if (a[strlen(a) - 1] == '\n') {
        a[strlen(a) - 1] = '\0';  // Replace newline with null terminator
    } else {
        clearInputBuffer();  // Clear remaining input if user entered more than 49 characters
    }
    if (!is_valid_string(a)){
        printf("\n\n\t\t\t\tPlease enter a valid name!");
        goto username;
    }
    
    sanitize(a);
   
    password:
    printf("\n\n\t\t\t\tEnter Password:");

    // Get the username
    fgets(pass, 50, stdin);

    // Remove the newline character if it's read by fgets
    if (pass[strlen(pass) - 1] == '\n') {
        pass[strlen(pass) - 1] = '\0';  // Replace newline with null terminator
    } else {
        clearInputBuffer();  // Clear remaining input if user entered more than 49 characters
    }
    if (strlen(pass) < 1){
        printf("\n\n\t\t\t\tPlease enter a valid password!");
        goto password;
    }
    sanitize(pass);
    alphamirror(pass);
 
    FILE *fp;
    char names[100][100]; 
    struct User userChecker;
    char id[5];
    int counter = 0;

    if ((fp = fopen("./data/users.txt", "a+")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%s %s %s", id, userChecker.name, userChecker.password) != EOF)
    {
        strcpy(names[counter], userChecker.name);
        counter++;
    }

    if (counter != 0){
        fprintf(fp, "\n");
    }

    if(isPresent(names, a)){
        system("clear");
        printf("This user already exists!\n");
        sleep(2);
        goto username;
    }
    else{
        fprintf(fp, "%d %s %s", counter, a, pass);
    }
    
    fclose(fp);
};

// display login menu
void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t============= LOGIN =============");
     clearInputBuffer();
    usernameLogin:
    printf("\n\n\t\t\t\tEnter Username:");
    fgets(a, 50, stdin);

    if (a[strlen(a) - 1] == '\n') {
        a[strlen(a) - 1] = '\0'; 
    } else {
        clearInputBuffer(); 
    }
     if (!is_valid_string(a)){
        printf("\n\n\t\t\t\tPlease enter a valid name!");
        goto usernameLogin;
    }
    sanitize(a);

    //disable echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
    printf("\n\n\t\t\t\tEnter Password:");
    fgets(pass, 50, stdin);

    if (pass[strlen(pass) - 1] == '\n') {
        pass[strlen(pass) - 1] = '\0';  
    } else {
        clearInputBuffer(); 
    }
    sanitize(pass);
    alphamirror(pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
};


// retrieve password from storage
const char *getPassword(struct User *u) {
    FILE *fp;
    struct User userChecker;
    char line[150];
    char id[10];

    if ((fp = fopen("./data/users.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        sscanf(line, "%s %s %s", id, userChecker.name, userChecker.password);

        if (strcmp(userChecker.name, u->name) == 0) {
            fclose(fp);
            u->id = atoi(id);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}