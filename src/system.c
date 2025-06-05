#include "header.h"
#include <unistd.h>  // For sleep()

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %s %d/%d/%d %s %s %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}



int getAccountFromUser(FILE *ptr, char name[50], struct User *r)
{
return  fscanf(ptr, "%d %s",
                  &r->id,
		          name);
            
}
 
void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %s %d/%d/%d %s %s %.2lf %s\n\n",        
            r.id,
	        u.id,
	        u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
} 

void saveRecordToFile(FILE *ptr, struct Record r)
{
    fprintf(ptr, "%d %d %s %s %d/%d/%d %s %s %.2lf %s\n\n",
            r.id,
	        r.userId,
	        r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}


void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    char choices[3]; 
    if (notGood == 0)
    {
        system("clear");
        printf("\n Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
           
            if (fgets(choices, sizeof(choices), stdin) != NULL) {
                // Remove the newline character if present
                size_t len = strlen(choices);
                if (len > 0 && choices[len-1] == '\n') {
                    choices[len-1] = '\0';
                }
            }
            if ((strlen(choices) != 1 )){
                printf("\nInvalid option!");
                goto invalid;
            }
            
            if(!is_valid_number(choices)){
                printf("\nInvalid option!");
                goto invalid;
            }            
           
            option = (int)strtol(choices, NULL, 10);

        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid option!\n");
            goto invalid;
        }
    }
    else
    {   
        invalid2:
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
       
            if (fgets(choices, sizeof(choices), stdin) != NULL) {
                size_t len = strlen(choices);
                if (len > 0 && choices[len-1] == '\n') {
                    choices[len-1] = '\0';
                }
            }
            if ((strlen(choices) != 1 )){
                printf("\nInvalid option!");
                goto invalid;
            }
            
            if(!is_valid_number(choices)){
                printf("\nInvalid option!");
                goto invalid;
            }            
            option = (int)strtol(choices, NULL, 10);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else if (option == 0)
    {
        system("clear");
        exit(1);
    }else {
        printf("\nInvalid option!");
        goto invalid2;
    }
}

void success(struct User u)
{
    int option;
    char choices[3];
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
     if (fgets(choices, sizeof(choices), stdin) != NULL) {
                size_t len = strlen(choices);
                if (len > 0 && choices[len-1] == '\n') {
                    choices[len-1] = '\0';
                }
            }
            if ((strlen(choices) != 1 )){
                printf("\nInvalid option!");
                goto invalid;
            }
            
            if(!is_valid_number(choices)){
                printf("\nInvalid option!");
                goto invalid;
            }            
            option = (int)strtol(choices, NULL, 10);


    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Please insert a valid option!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");
    if (pf == NULL) {
        printf("Error opening file.\n");
        return;
    }
 
    int ind = 0;
  
noAccount:
    system("clear");
    printf("\t\t\t============= New record =============\n");
clearInputBuffer();
date:
      
    printf("\nEnter today's date (mm/dd/yyyy): "); 
        
    char date_input[50];
    char extra_input[10];
    int num_fields;
   // fgets(date_input, sizeof(date_input), stdin);
    if (fgets(date_input, sizeof(date_input), stdin) != NULL) {
        size_t len = strlen(date_input);

        // Check if input was truncated (i.e., no newline at the end)
        if (len > 0 && date_input[len-1] == '\n') {
            date_input[len-1] = '\0';  // Remove the newline character
        } else {
            // Input was too long, clear the rest of the input buffer
            int ch;
            while (((ch = getchar()) != '\n') && ch != EOF);
        }

    }


    num_fields = sscanf(date_input, "%d/%d/%d %s", &r.deposit.month, &r.deposit.day, &r.deposit.year, extra_input);

    if (num_fields != 3 || r.deposit.month <= 0 || r.deposit.month > 12 ||
        r.deposit.day <= 0 || r.deposit.day > 31 ||
        r.deposit.year < 1900 || r.deposit.year > 3000) {
        printf("Invalid date! Usage Example: 8/8/2024\n");
        goto date;
    }

     if (num_fields == 4) {
        printf("Invalid date! Usage Example: 8/8/2024\n");
        goto date;
    } 
    
accountNo:
printf("\nEnter the account number:");
 //char doen[20]; 
    if (fgets(r.accountNbr, sizeof(r.accountNbr), stdin) != NULL) {
        size_t len = strlen(r.accountNbr);
        if (len > 0 && r.accountNbr[len-1] == '\n') {
            r.accountNbr[len-1] = '\0';
        } else {
        // Input was too long, clear the rest of the input buffer
        int ch;
        while (((ch = getchar()) != '\n') && ch != EOF);
         }
    }
    if ((strlen(r.accountNbr) < 1 ) || (strlen(r.accountNbr) > 18)){
        printf("\nInvalid account number!");
        goto accountNo;
    }
    
    if(!is_valid_number(r.accountNbr)){
        printf("\nInvalid account number!");
        goto accountNo;
    }
    
   // r.accountNbr = strtoll(doen, NULL, 10);
   //r.accountNbr = doen;
    // if (r.accountNbr == 0){
    //     printf("\nInvalid account number!");
    //     goto accountNo;
    // } 
    while (getAccountFromFile(pf, userName, &cr))
    {     
       // printf("cr.account:%s",cr.accountNbr);
        //printf("r.account:%s",r.accountNbr);
      //  printf("userName:%s", userName);
      //  printf("u.name:%s", u.name);

        if (strcmp(userName, u.name) == 0 && strcmp(cr.accountNbr, r.accountNbr) == 0)
        {
            printf("\n This Account already exists for this user\n\n");
           // sleep(2);
            goto accountNo;
        }else if (strcmp(cr.accountNbr, r.accountNbr) == 0)
        {
            printf("\n This Account number is already taken by another user\n\n");
            //sleep(2);
            goto accountNo;
        }

      ind++;
    }
   
    r.id = ind;
    u.id = getUserId(u.name); 
country:
    printf("\nEnter the country:");
    fgets(r.country, 70, stdin);
    
    // Remove the newline character if it's read by fgets
    if (r.country[strlen(r.country) - 1] == '\n') {
        r.country[strlen(r.country) - 1] = '\0';  // Replace newline with null terminator
    } else {
        clearInputBuffer();  // Clear remaining input if user entered more than 49 characters
    }

    if (!is_valid_string(r.country)){
       printf("\n Invalid input!\n\n");
       goto country;
    }
    if (strlen(r.country) < 2 || strlen(r.country) > 65){
        printf("\n Invalid input! input should be a minimum of 2 characters or maximum of 65\n\n");
       goto country;
    }
    sanitize(r.country);
phone:
    printf("\nEnter the phone number:");
     fgets(r.phone, 16, stdin);

    if (r.phone[strlen(r.phone) - 1] == '\n') {
        r.phone[strlen(r.phone) - 1] = '\0'; 
    } else {
        clearInputBuffer();  
    }

    if(!is_valid_number(r.phone)){
        printf("\nInvalid phone number:");
        goto phone;
    }
    if(strlen(r.phone) < 5){
        printf("\nInvalid phone number!\n");
        goto phone;
       
    }else if(strlen(r.phone) > 14){
         printf("\nInvalid phone number!\n");
        goto phone;
    }
amount:
   
    printf("\nEnter amount to deposit: $");
     char *endPtr;
    char amount[20];
    fgets(amount, 20, stdin);

    if (amount[strlen(amount) - 1] == '\n') {
        amount[strlen(amount) - 1] = '\0'; 
    } else {
        clearInputBuffer();
    }   
     r.amount = strtod(amount, &endPtr);

    if (endPtr == amount) {
        printf("Invalid input! Please enter only digits.\n");
        goto amount;
    }else if (*endPtr != '\0') {
        printf("Invalid input! Please enter only digits.\n");
        goto amount;
    } 

    if(r.amount <= 0 || r.amount > 9999999999){
        printf("\nInvalid amount! (min allowed:$1, max allowed: $9999999999)\n");
       goto amount;
       
    }
accType:
    printf("\nChoose the type of account:\n\t-> savings\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    if (fgets(r.accountType, 10, stdin) != NULL) {
    size_t len = strlen(r.accountType);

    // Check if input was truncated (i.e., no newline at the end)
    if (len > 0 && r.accountType[len-1] == '\n') {
        r.accountType[len-1] = '\0';  // Remove the newline character
    } else {
        // Input was too long, clear the rest of the input buffer
        int ch;
        while (((ch = getchar()) != '\n') && ch != EOF);
    }

    // Validate the length of the account number
    if ( len > 10) {
        printf("\nInvalid choice! Please choose and enter one of the listed options.\n");
        goto accType;
        }
    
}
    
     if (strcmp(r.accountType, "current") != 0 &&
        strcmp(r.accountType, "savings") != 0 &&
        strcmp(r.accountType, "fixed01") != 0 &&
        strcmp(r.accountType, "fixed02") != 0 &&
        strcmp(r.accountType, "fixed03") != 0){
        printf("\nInvalid choice! Please choose and enter one of the listed options.\n");
        goto accType;

    }

    saveAccountToFile(pf, u, r);
    fclose(pf);
    success(u);
}



void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nOwner:%s\nAccount number:%s\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   userName,
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}


int getUserId(char *name){
    FILE *pf = fopen("./data/users.txt", "r+");

    char id[5];
    struct User temp;
    while(fscanf(pf, "%s %s %s", id, temp.name, temp.password) != EOF){
        if(strcmp(temp.name, name) == 0){
            return atoi(id);
        }
    }

    return -1;
}

void updateAccountInfo(struct User u, char *accountNum, int choice){
    char userName[100];
    struct Record r;
    struct Record arr[100];
    FILE *pf = fopen(RECORDS, "a+");

    system("clear");

    int index = 0;
    while (getAccountFromFile(pf, userName, &r))
    {   
        strcpy(r.name, userName);

        if(strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0){
            if(choice == 1){
                phoneUpdate:
                printf("\nEnter the new phone number:");
                fgets(r.phone, 16, stdin);

                if (r.phone[strlen(r.phone) - 1] == '\n') {
                    r.phone[strlen(r.phone) - 1] = '\0'; 
                } else {
                    clearInputBuffer();  
                }

                if(!is_valid_number(r.phone)){
                    printf("\nInvalid phone number:");
                    goto phoneUpdate;
                }
                if(strlen(r.phone) < 5){
                    printf("\nInvalid phone number!\n");
                    goto phoneUpdate;
                
                }else if(strlen(r.phone) > 14){
                    printf("\nInvalid phone number!\n");
                    goto phoneUpdate;
                }
            }
            else if(choice == 2){
                countryUpdate:
                printf("\nEnter the new country name:");
                fgets(r.country, 20, stdin);
                
                if (r.country[strlen(r.country) - 1] == '\n') {
                    r.country[strlen(r.country) - 1] = '\0'; 
                } else {
                    clearInputBuffer(); 
                }

                if (!is_valid_string(r.country)){
                printf("\n Invalid input!\n\n");
                goto countryUpdate;
                }
                sanitize(r.country);
             }
        }

        arr[index] = r;
        index++;
    }

    fclose(fopen(RECORDS, "w"));

    for (int i = 0; i < index; i++){
        saveRecordToFile(pf, arr[i]);
    }

    fclose(pf);
    success(u);
}

void checkDetailOfAccount(struct User u, char *accountNum){
    char userName[100];
    struct Record r;
    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0)
        {
            printf("\nOwner:%s\nAccount number:%s\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
                    userName,
                    r.accountNbr,
                    r.deposit.month,
                    r.deposit.day,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType);

            float rate;
            if(strcmp(r.accountType, "savings") == 0){
                rate = 0.07;
                float interest = r.amount * (1 + rate / 12) - r.amount;
                printf(" -> You will get $%.2f as interest on day %d of every month\n", interest, r.deposit.day);
            }
            else if(strcmp(r.accountType, "fixed01") == 0){
                rate = 0.04;
                float interest = r.amount * (1 + rate / 12) - r.amount;
                interest *= 12;
                printf(" -> You will get $%.2f as interest on %d/%d/%d\n", interest, 
                    r.deposit.month, r.deposit.day, r.deposit.year+1);
            }
            else if(strcmp(r.accountType, "fixed02") == 0){
                rate = 0.05;
                float interest = r.amount * (1 + rate / 12) - r.amount;
                interest *= 24;
                printf(" -> You will get $%.2f as interest on %d/%d/%d\n", interest, 
                    r.deposit.month, r.deposit.day, r.deposit.year+2);
            }
            else if(strcmp(r.accountType, "fixed03") == 0){
                rate = 0.08;
                float interest = r.amount * (1 + rate / 12) - r.amount;
                interest *= 36;
                printf(" -> You will get $%.2f as interest on %d/%d/%d\n", interest, 
                    r.deposit.month, r.deposit.day, r.deposit.year+3);
            }
            else{
                printf("You will not get interests because the account is of type current\n");
            }

            fclose(pf);
            success(u);
        }
    }

}


void checkAccountBalance(struct User u, char *accountNum){
    char userName[100];
    struct Record r;
    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0)
        {
            printf("\nOwner:%s\nAccount number:%s\nAccount balance: $%.2f \n",
                    userName,
                    r.accountNbr,                 
                    r.amount);

            fclose(pf);
            success(u);
        }
    }

}



void makeTransaction(struct User u, char *accountNum, int choice){
    char userName[100];
    struct Record r;
    struct Record arr[100];
    FILE *pf = fopen(RECORDS, "a+");

    int index = 0;
    double available = 0;
    double input = 0;
    char *endPtr;
    char amount[20];
    while (getAccountFromFile(pf, userName, &r))
    {
        strcpy(r.name, userName);

        if(strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0){
            if(strcmp(r.accountType, "fixed01") == 0 ||
            strcmp(r.accountType, "fixed02") == 0 ||
            strcmp(r.accountType, "fixed03") == 0){
                system("clear");
                printf(" Sorry! Transactions are not allowed on fixed accounts.");
                mainMenu(u);
            }

            available = r.amount;

            
            if(choice == 1){
                amount:
                printf("Enter the amount you want to withdraw:");
                
                fgets(amount, 20, stdin);

                if (amount[strlen(amount) - 1] == '\n') {
                    amount[strlen(amount) - 1] = '\0';
                } else {
                    clearInputBuffer(); 
                }   
                input = strtod(amount, &endPtr);

                if (endPtr == amount) {
                    printf("Invalid input! Please enter only digits.\n");
                    goto amount;
                }else if (*endPtr != '\0') {
                    printf("Invalid input! Please enter only digits.\n");
                    goto amount;
                } 

                if(input <= 0 || input > 9999999999){
                    printf("\nInvalid amount! (min allowed:$1, max allowed: $9999999999)\n");
                goto amount;
                
                }
            }
            else if(choice == 2){
                printf("Enter the amount you want to deposit:");
                 fgets(amount, 20, stdin);

                // Remove the newline character if it's read by fgets
                if (amount[strlen(amount) - 1] == '\n') {
                    amount[strlen(amount) - 1] = '\0';  // Replace newline with null terminator
                } else {
                    clearInputBuffer();  // Clear remaining input if user entered more than 49 characters
                }   
                input = strtod(amount, &endPtr);

                // Check if the conversion was successful
                if (endPtr == amount) {
                    printf("Invalid input! Please enter only digits.\n");
                    goto amount;
                }else if (*endPtr != '\0') {
                    printf("Invalid input! Please enter only digits.\n");
                    goto amount;
                } 

                if(input <= 0 || input > 9999999999){
                    printf("\nInvalid amount! (min allowed:$1, max allowed: $9999999999)\n");
                goto amount;
                
                }

            }else{
                printf("Invalid choice! Try again.");
                mainMenu(u);
            }

            if (input <= available && input > 0 && choice == 1){
                r.amount -= input;
            }
            else if (choice == 1 && input > available){
                system("clear");
                printf(" Sorry! You don't have enough funds to withdraw %.2lf. Your account balance is %.2lf", input, available);
                mainMenu(u);
            }
            else if(choice == 1 && input <= 0){
                system("clear");
                printf(" Please enter amount greater than 0.");
                mainMenu(u);
            }
            else if (choice == 2 && input > 0 && input <= 9999999999){
                r.amount += input;
            }
            else if(choice == 2 && input > 9999999999){
                system("clear");
                printf(" You cannot deposit more than $9,999,999,999.");
                mainMenu(u);
            }
            else if (choice == 2 && input <= 0){
                system("clear");
                printf(" Please enter amount more than 0.");
                mainMenu(u);
            }
        }

        arr[index] = r;
        index++;
    }

    fclose(fopen(RECORDS, "w"));

    for (int i = 0; i < index; i++){
        saveRecordToFile(pf, arr[i]);
    }

    system("clear");
    fclose(pf);
    success(u);
}

void removeAccount(struct  User u, char *accountNum){
    char userName[100];
    struct Record r;
    struct Record deleted;
    struct Record arr[100];
    FILE *pf = fopen(RECORDS, "a+");

    int index = 0;
    while (getAccountFromFile(pf, userName, &r))
    {
        strcpy(r.name, userName);

        if(strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0){
            deleted = r;
        }
        else{
            arr[index] = r;
            index++;
        }
        
    }

    system("clear");
    printf("                       ===== Deleted account =====\n");
    printf("\nOwner:%s\nAccount number:%s\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
        deleted.name,
        deleted.accountNbr,
        deleted.deposit.day,
        deleted.deposit.month,
        deleted.deposit.year,
        deleted.country,
        deleted.phone,
        deleted.amount,
        deleted.accountType);
    
    fclose(fopen(RECORDS, "w"));

    for (int i = 0; i < index; i++){
        saveRecordToFile(pf, arr[i]);
    }

    fclose(pf);
    success(u);
}

void transferAccount(struct User u, char *accountNum){
    char userName[100];
    char newOwnerName[100];
    struct Record r;
    struct Record arr[100];
    FILE *pf = fopen(RECORDS, "a+");

    int index = 0;
    while (getAccountFromFile(pf, userName, &r))
    {
        strcpy(r.name, userName);
        if (strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0)
        {
            system("clear");
            printf("                       ===== Transfering account:\n");
            printf("\nAccount number:%s\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
                    r.accountNbr,
                    r.deposit.day,
                    r.deposit.month,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType);
        }

        arr[index] = r;
        index++;
    }
    
    fclose(pf);

    printf("Enter the name of the person you wish to transfer to:");

    fgets(newOwnerName, 50, stdin);

    if (newOwnerName[strlen(newOwnerName) - 1] == '\n') {
        newOwnerName[strlen(newOwnerName) - 1] = '\0'; 
    } else {
        clearInputBuffer(); 
    }
   
    sanitize(newOwnerName);

    if(!isNamePresent(newOwnerName)){
        printf("The user does not exist.");
        mainMenu(u);
    }
    int newOwnerId;
    newOwnerId = getUserId(newOwnerName);

    for(int i = 0; i < index; i++){
        if(strcmp(arr[i].name, u.name) == 0 && strcmp(arr[i].accountNbr, accountNum) == 0){
            strcpy(arr[i].name, newOwnerName);
            arr[i].userId = newOwnerId;
           
        }
    }
    
    fclose(fopen(RECORDS, "w"));
    
    pf = fopen(RECORDS, "a+");
    for (int i = 0; i < index; i++){
        saveRecordToFile(pf, arr[i]);
    }

    fclose(pf);
    success(u);
    
}




void transactionAcc(struct User u, char *accountNum){
    char userName[100];
    struct Record r;
  
    FILE *pf = fopen(RECORDS, "a+");

    while (getAccountFromFile(pf, userName, &r))
    {
        strcpy(r.name, userName);

        if(strcmp(userName, u.name) == 0 && strcmp(r.accountNbr, accountNum) == 0){
            if(strcmp(r.accountType, "fixed01") == 0 ||
            strcmp(r.accountType, "fixed02") == 0 ||
            strcmp(r.accountType, "fixed03") == 0){
                system("clear");
                printf(" Sorry! Transactions are not allowed on fixed accounts.");
                mainMenu(u);
            }
        }
    }
}
