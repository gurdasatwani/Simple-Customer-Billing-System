#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

FILE *filePointer;
int numberCustomer = 0;

struct date
{
    int day;
    int month;
    int year;
};

struct customer
{
    char customerNumber[6]; // input
    char *customerStatues;
    char phoneNumber[11]; // input
    char name[21];        // input
    char street[21];      // input
    char city[21];        // input
    int CustomerId;
    float dueAmount;
    float totalAmount;           // input
    float totalPaidAmount;       // input
    struct date lastPaymentDate; // input
};

void addAccount(void);
void addSingleAccount(void);
void addMultipleAccount(void);
void inputAppend(void);

void searchAccount(void);
void searchName(void);
void searchNumber(void);
void search(char input[21], int index);
void output(char rowData[150]);

int main()
{

    system("cls");

    puts("\t\tCUSTOMER BILLING SYSTEM");
    puts("\t\t=======================");
    puts("\t\t1. Add Account");
    puts("\t\t2. Search Account");
    puts("\t\t3. Exit");
    puts("\t\t=======================");

    char ask;
    char data[5];

    if (access("database.txt", F_OK) == 0) // access function from <unistd.h>
    {
        filePointer = fopen("database.txt", "r+"); // if file exist open file in read mode
    }
    else
    {
        filePointer = fopen("database.txt", "w+"); // if file not exist create file in write mode
        fprintf(filePointer, "%.4d\n\n",
                numberCustomer);
    }

    fseek(filePointer, 0, SEEK_SET);
    fgets(data, 5, filePointer);
    fclose(filePointer);

    numberCustomer = atoi(data); // convert the data into intger

    do
    {
        puts("\nSelect what you what to do:");
        ask = getchar();
        fflush(stdin);
    } while ((ask != '1') && (ask != '2') && (ask != '3'));

    switch (ask)
    {
    case ('1'):
    {
        addAccount();
    }
    break;
    case ('2'):
    {
        searchAccount();
    }
    break;
    case ('3'):
    {
        exit(1);
    }
    break;
    }

    return 0;
}

void addAccount(void)
{
    system("cls");

    puts("\t\t       ADD ACCOUNT");
    puts("\t\t========================");
    puts("\t\t1. Add Single Account");
    puts("\t\t2. Add Multiple Accounts");
    puts("\t\t3. Back");
    puts("\t\t========================");

    char ask;

    do
    {
        puts("\nSelect what you what to do:");
        ask = getchar();
        fflush(stdin);
    } while ((ask != '1') && (ask != '2') && (ask != '3'));

    switch (ask)
    {
    case ('1'):
    {
        addSingleAccount();
    }
    break;
    case ('2'):
    {
        addMultipleAccount();
    }
    break;
    case ('3'):
    {
        main();
    }
    break;
    }
}

void addSingleAccount(void)
{
    system("cls");

    filePointer = fopen("database.txt", "a+"); // open file in append mode

    inputAppend();

    filePointer = fopen("database.txt", "r+"); // open file in read mode

    fseek(filePointer, 0, SEEK_SET); // set cursor to beggining of file
    fprintf(filePointer, "%.4d\n\n", // overwrite number of customer in file
            numberCustomer);

    fclose(filePointer);
    fflush(stdin);
    addAccount();
}

void addMultipleAccount(void)
{
    int number;
    system("cls");

    do
    {
        puts("How many accounts do you want to add:");
        scanf("%d", &number);

        if ((number > 0) && (number <= 5))
        {
            break;
        }
        else
        {
            puts("Maximum 5 records are allowed at a time.");
        }

        fflush(stdin);
    } while (1);

    filePointer = fopen("database.txt", "a+"); // open file in append mode

    fflush(stdin);

    for (int i = 0; i < number; i++)
    {
        system("cls");

        printf("%d more to go.\n", (number - (i + 1)));

        inputAppend();

        fflush(stdin);
    }

    filePointer = fopen("database.txt", "r+"); // open file in read mode

    fseek(filePointer, 0, SEEK_SET); // set cursor to beggining os file
    fprintf(filePointer, "%.4d\n\n", // overwrite number of customer in file
            numberCustomer);

    fclose(filePointer);
    addAccount();
}

void inputAppend(void)
{
    struct customer account;

    account.CustomerId = ++numberCustomer;
    printf("Customer Id: %.4d\n\n", account.CustomerId);

    puts("Name (only upto 20 characters):");
    gets(account.name);

    puts("Phone Number (only upto 10 numbers):");
    scanf("%s", account.phoneNumber);
    fflush(stdin);

    puts("City (only upto 20 characters):");
    gets(account.city);

    puts("Street (only upto 20 characters):");
    gets(account.street);

    puts("Account Number (only upto 5 numbers)");
    scanf("%s", account.customerNumber);

    puts("Total Amount:");
    scanf("%f", &account.totalAmount);

    puts("Total Paid Amount:");
    scanf("%f", &account.totalPaidAmount);

    puts("Last Payment Date (DD/MM/YYYY):");
    scanf("%d/%d/%d",
          &account.lastPaymentDate.day,
          &account.lastPaymentDate.month,
          &account.lastPaymentDate.year);

    fprintf(filePointer, "%.4d", account.CustomerId);
    fprintf(filePointer, "|%s", account.name); // separating data with (|)
    fprintf(filePointer, "|%s", account.phoneNumber);
    fprintf(filePointer, "|%s", account.city);
    fprintf(filePointer, "|%s", account.street);
    fprintf(filePointer, "|%s", account.customerNumber);
    fprintf(filePointer, "|%.2f", account.totalAmount);
    fprintf(filePointer, "|%.2f", account.totalPaidAmount);

    account.dueAmount = fabs(account.totalAmount - account.totalPaidAmount);

    if (account.dueAmount > account.totalPaidAmount)
    {
        account.customerStatues = "OVERDUE";
    }
    else if ((account.dueAmount < account.totalPaidAmount) && (account.dueAmount > 0))
    {
        account.customerStatues = "UNDERDUE";
    }
    else if (account.dueAmount == account.totalPaidAmount)
    {
        account.customerStatues = "HALFDUE";
    }
    else
    {
        account.customerStatues = "PAID";
    }

    fprintf(filePointer, "|%.2f", account.dueAmount);
    fprintf(filePointer, "|%s", account.customerStatues);
    fprintf(filePointer, "|%.2d/%.2d/2%.3d|\n",
            account.lastPaymentDate.day,
            account.lastPaymentDate.month,
            account.lastPaymentDate.year);
}

void searchAccount(void)
{
    system("cls");

    puts("\t\t     SEARCH ACCOUNT");
    puts("\t\t========================");
    puts("\t\t1. Search Name");
    puts("\t\t2. Search Customer Number");
    puts("\t\t3. Back");
    puts("\t\t========================");

    char ask;

    do
    {
        puts("\nSelect what you what to do:");
        ask = getchar();
        fflush(stdin);
    } while ((ask != '1') && (ask != '2') && (ask != '3'));

    switch (ask)
    {
    case ('1'):
    {
        searchName();
    }
    break;
    case ('2'):
    {
        searchNumber();
    }
    break;
    case ('3'):
    {
        main();
    }
    break;
    }
}

void searchName(void)
{
    system("cls");

    char name[21];
    int index = 5;

    puts("Enter name (only upto 20 characters):");
    gets(name);

    search(name, index);
}

void searchNumber(void)
{
    system("cls");

    char customerId[5];
    int index = 0;

    puts("Enter Customer Id (only upto 4 numbers):");
    scanf("%s", customerId);

    search(customerId, index);
}

void search(char *input, int index)
{
    char rowData[150];

    filePointer = fopen("database.txt", "r+");
    fscanf(filePointer, "%*[^\n]\n"); // this line of code will skip a line in a file

    while (!feof(filePointer))
    {
        fgets(rowData, 150, filePointer);

        if (!feof(filePointer))
        {
            int i = index;
            int j = 0;
            char target[21];
            while (rowData[i] != '|') // getting data before seperator (|)
            {
                target[j] = rowData[i];
                i++;
                j++;
            }
            target[j] = '\0';
            if (strcmp(target, input) == 0)
            {
                output(rowData);
                break;
            }
        }
    }

    fclose(filePointer);
}

void output(char rowData[150])
{
    system("cls");

    char *title[11] = {
        "Customer Id: ", "Name: ", "Phone Number: ",
        "City: ", "Street: ", "Account Number: ",
        "Total Amount: ", "Total Paid Amount: ",
        "Due Amount: ", "Account Status: ",
        "Last Payment Date: "};

    int i, j, oldSepIdx;
    i = j = oldSepIdx = 0;

    while (rowData[i] != '\n')
    {
        if (rowData[i] == '|') // printing data before seprator (|)
        {
            printf("%s", title[j]);
            for (int j = oldSepIdx; j < i; j++)
            {
                if (rowData[j] == '|')
                {
                    continue;
                }
                printf("%c", rowData[j]);
            }
            oldSepIdx = i;
            puts("");
            j++;
        }
        i++;
    }
}