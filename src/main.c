#include "header.h"

void mainMenu( User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        // student TODO : add your **Update account information** function
        // here
        break;
    case 3: {
        int accountId;
        printf("Enter account number you want to check:  ");
        scanf("%d", &accountId);
        checkAccountDetails(u, accountId);
        break;
    }
    case 4:
        checkAllAccounts(u);
        break;
    case 5: {
        int accountId, transactionType;
                printf("\nEnter the account ID for the transaction: ");
                scanf("%d", &accountId);

                printf("\nChoose the transaction type:\n");
                printf("1. Deposit\n");
                printf("2. Withdraw\n");
                printf("Enter your choice: ");
                scanf("%d", &transactionType);

                if (transactionType != 1 && transactionType != 2) {
                    printf("\n✖ Invalid transaction type!\n");
                    break;
                }

                makeTransaction(u.id, accountId, transactionType);
        break;
    }
    case 6:
        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
};

void initMenu( User *u)
{
    int r = 0;
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(u);
            if (strcmp(u->password, getPassword(*u)) == 0)
            {
                printf("\n\nPassword Match!");
                r = 1;
            }
            else
            {
                printf("\nWrong password!! or User Name\n");
                exit(1);
            }
            break;
        case 2:
            registerMenu(u);
            printf("\n\nRegistration successful! Please log in.\n");
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }
};

int main()
{
     User u;
    
    initMenu(&u);
    mainMenu(u);
    return 0;
}
