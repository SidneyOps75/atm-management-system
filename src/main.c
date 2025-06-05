#include "header.h"
#include <unistd.h>

void mainMenu(struct User u)
{
    int option;
    char accountNum[20];  
    int choice;
    system("clear");
    printf("\n\n\t\t======= ATM MANAGEMENT SYSTEM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of all owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Check account balance\n");
    printf("\n\t\t[9]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u);     
        break;
  
    case 2:
        // Update account information function
        clearInputBuffer();
        case2Account:
        printf("\nPlease enter the account number:");
        //char doen[20]; 
            if (fgets(accountNum, sizeof(accountNum), stdin) != NULL) {
                // Remove the newline character if present
                size_t len = strlen(accountNum);
                if (len > 0 && accountNum[len-1] == '\n') {
                    accountNum[len-1] = '\0';
                }
            }
            if ((strlen(accountNum) < 1 ) || (strlen(accountNum) > 18)){
                printf("\nInvalid account number!\n");
                goto case2Account;
            }
            
            if(!is_valid_number(accountNum)){
                printf("\nInvalid account number!\n");
                goto case2Account;
            }
            
            
          //  accountNum = strtoll(doen, NULL, 10);

        if (!isAccountPresent(accountNum, u)){
            system("clear");
            printf("This account does not exist!");
           // sleep(2);
            mainMenu(u);
        }
       // clearInputBuffer();
        accountUpdate:
        printf("\nWhich field would you like to change?\n1-> phone number\n2-> country\n");
              
        char change[3]; 
            if (fgets(change, sizeof(change), stdin) != NULL) {
                size_t len = strlen(change);
                if (len > 0 && change[len-1] == '\n') {
                    change[len-1] = '\0';
                }
            }
            if ((strlen(change) != 1 )){
                printf("\nInvalid option!");
                goto accountUpdate;
            }
            
            if(!is_valid_number(change)){
                printf("\nInvalid option!");
                goto accountUpdate;
            }            
            // Convert to int
            choice = (int)strtol(change, NULL, 10);
        if (choice == 1 || choice == 2){
            updateAccountInfo(u, accountNum, choice);
        }else{
            printf("Invalid input!\n");
            goto accountUpdate;
        }
        break;
    case 3:

        clearInputBuffer();
        case3Account:
        printf("\nPlease enter the account number:");
        //char acc3[20]; 
            if (fgets(accountNum, sizeof(accountNum), stdin) != NULL) {
                size_t len = strlen(accountNum);
                if (len > 0 && accountNum[len-1] == '\n') {
                    accountNum[len-1] = '\0';
                }
            }
            if ((strlen(accountNum) < 1 ) || (strlen(accountNum) > 18)){
                printf("\nInvalid account number!\n");
                goto case3Account;
            }
            
            if(!is_valid_number(accountNum)){
                printf("\nInvalid account number!\n");
                goto case3Account;
            }
            
           // accountNum = strtoll(acc3, NULL, 10);

        if (!isAccountPresent(accountNum, u)){
            system("clear");
            printf("This account does not exist!");
            sleep(2);
            mainMenu(u);
        }
        checkDetailOfAccount(u, accountNum);
        break;
    case 4:
        clearInputBuffer();
        checkAllAccounts(u);
        break;
    case 5:
        clearInputBuffer();
        case5Account:
        printf("\nPlease enter the account number:");
        //char accountNum[20]; 
            if (fgets(accountNum, sizeof(accountNum), stdin) != NULL) {
                size_t len = strlen(accountNum);
                if (len > 0 && accountNum[len-1] == '\n') {
                    accountNum[len-1] = '\0';
                }
            }
            if ((strlen(accountNum) < 1 ) || (strlen(accountNum) > 18)){
                printf("\nInvalid account number!\n");
                goto case5Account;
            }
            
            if(!is_valid_number(accountNum)){
                printf("\nInvalid account number!\n");
                goto case5Account;
            }
            
           // accountNum = strtoll(accountNum, NULL, 10);

        if (!isAccountPresent(accountNum, u)){
            system("clear");
            printf("This account does not exist!");
            sleep(2);
            mainMenu(u);
        }
        transactionAcc(u, accountNum);
        transactionType:
        printf("Do you want to:\n        1-> Withdraw\n        2-> Deposit\n\n");
        char transaction[3]; 
            if (fgets(transaction, sizeof(transaction), stdin) != NULL) {
                size_t len = strlen(transaction);
                if (len > 0 && transaction[len-1] == '\n') {
                    transaction[len-1] = '\0';
                }
            }
            if ((strlen(transaction) != 1 )){
                printf("\nInvalid option!\n");
                goto transactionType;
            }
            
            if(!is_valid_number(transaction)){
                printf("\nInvalid option!\n");
                goto transactionType;
            }            
            // Convert to int
            choice = (int)strtol(transaction, NULL, 10);
        if (choice == 1 || choice == 2){
            makeTransaction(u, accountNum, choice);
        }else{
            printf("Invalid input!\n");
            goto transactionType;
        }
        break;
    case 6:

        clearInputBuffer();
        removeAccount: 
        printf("Enter account number to be deleted:");
        //char accountNum[20]; 
            if (fgets(accountNum, sizeof(accountNum), stdin) != NULL) {
                // Remove the newline character if present
                size_t len = strlen(accountNum);
                if (len > 0 && accountNum[len-1] == '\n') {
                    accountNum[len-1] = '\0';
                }
            }
            if ((strlen(accountNum) < 1 ) || (strlen(accountNum) > 18)){
                printf("\nInvalid account number!\n");
                goto removeAccount;
            }
            
            if(!is_valid_number(accountNum)){
                printf("\nInvalid account number!\n");
                goto removeAccount;
            }
            
            // Convert to char
         //   accountNum = strtoll(accountNum, NULL, 10);

        if (!isAccountPresent(accountNum, u)){
            system("clear");
            printf("This account does not exist!");
            sleep(2);
            mainMenu(u);
        }
        removeAccount(u, accountNum);
        break;
    case 7:
       
        clearInputBuffer();
        transferOwner: 
        printf("Enter the account number to be transferred:");
        //char accountNum[20]; 
            if (fgets(accountNum, sizeof(accountNum), stdin) != NULL) {
                // Remove the newline character if present
                size_t len = strlen(accountNum);
                if (len > 0 && accountNum[len-1] == '\n') {
                    accountNum[len-1] = '\0';
                }
            }
            if ((strlen(accountNum) < 1 ) || (strlen(accountNum) > 18)){
                printf("\nInvalid account number!\n");
                goto transferOwner;
            }
            
            if(!is_valid_number(accountNum)){
                printf("\nInvalid account number!\n");
                goto transferOwner;
            }
            
            // Convert to char
            //accountNum = strtoll(accountNum, NULL, 10);

        if (!isAccountPresent(accountNum, u)){
            system("clear");
            printf("This account does not exist!");
            sleep(2);
            mainMenu(u);
        }
        transferAccount(u, accountNum);
        break;
    case 8:
       
        clearInputBuffer();
        accountBalance: 
        printf("Enter account number:");
        //char accountNum[20]; 
            if (fgets(accountNum, sizeof(accountNum), stdin) != NULL) {
                size_t len = strlen(accountNum);
                if (len > 0 && accountNum[len-1] == '\n') {
                    accountNum[len-1] = '\0';
                }
            }
            if ((strlen(accountNum) < 1 ) || (strlen(accountNum) > 18)){
                printf("\nInvalid account number!\n");
                goto accountBalance;
            }
            
            if(!is_valid_number(accountNum)){
                printf("\nInvalid account number!\n");
                goto accountBalance;
            }
            
            // Convert to char
            //accountNum = strtoll(accountNum, NULL, 10);

        if (!isAccountPresent(accountNum, u)){
            system("clear");
            printf("This account does not exist!");
            sleep(2);
            mainMenu(u);
        }
        checkAccountBalance(u, accountNum);
        break;
    case 9:
        exit(1);
        break;
    default:
        printf("Invalid operation! Please choose one of the listed options.\n");
        sleep(2);
        mainMenu(u);
    }
};

void initMenu(struct User *u)
{
    int r = 0;
    int option;
   
    system("clear");
    
    printf("\n\n\t\t============= ATM MANAGEMENT SYSTEM =============\n");
    printf("\n\t\t-->> Welcome! Feel free to login / register <<--\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
    
        case 1:
            loginMenu(u->name, u->password);
            if (strcmp(u->password, getPassword(u)) == 0)
            {
                printf("\n\nLogin successful.");
            }
            else
            {
                printf("\nYou've entered wrong credentials! Try again\n");
                sleep(2);
                initMenu(u);
            }
            r = 1;
            break;
        case 2:
            // Registration function
            signUpMenu(u->name, u->password);
            r = 1;
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Please insert a valid operation!\n");
            sleep(2);
         initMenu(u);
        
        }
    }
};

int main()
{
    struct User u;
    
    initMenu(&u);
    mainMenu(u);
    return 0;
}
