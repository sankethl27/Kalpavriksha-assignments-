#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxLen 200

typedef struct
{
    int Age;
    int Id;
    char Name[20];
} User;

bool IsValidId(int Id)
{
    return Id > 0;
}

bool IsValidAge(int Age)
{
    return Age > 0 && Age < 130;
}

int StringLength(const char *str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

int ReadUser(const char *BufferString, User *User)
{
    return sscanf(BufferString, " { \"id\": %d , \"name\": \"%19[^\"]\" , \"age\": %d } ",
                  &User->Id, User->Name, &User->Age);
}

void WriteUser(FILE *WriteFilePointer, User *User)
{
    fprintf(WriteFilePointer, "{");
    fprintf(WriteFilePointer, " \"id\": %d ,", User->Id);
    fprintf(WriteFilePointer, " \"name\": \"%s\" ,", User->Name);
    fprintf(WriteFilePointer, " \"age\": %d ", User->Age);
    fprintf(WriteFilePointer, "}\n");
}

bool UserPresent(int Id)
{
    User Temp;
    FILE *ReadFilePointer = fopen("users.txt", "r");
    char BufferString[MaxLen];
    bool Answer = false;
    if (!ReadFilePointer)
    {
        perror("Error opening file");
        return false;
    }
    else
    {
        while (fgets(BufferString, sizeof(BufferString), ReadFilePointer))
        {
            if (ReadUser(BufferString, &Temp) == 3)
            {
                if (Temp.Id == Id)
                    Answer = true;
            }
        }
    }
    fclose(ReadFilePointer);
    return Answer;
}

int GetId()
{
    int Id;
    char Input[50];
    while (1)
    {
        printf("Enter ID of the user: ");
        if (fgets(Input, sizeof(Input), stdin))
        {
            if (sscanf(Input, "%d", &Id) == 1 && IsValidId(Id))
            {
                return Id;
            }
        }
        printf("Invalid ID. Please enter a positive number.\n");
    }
}

int GetAge()
{
    int Age;
    char Input[50];
    while (1)
    {
        printf("Enter age of the user: ");
        if (fgets(Input, sizeof(Input), stdin))
        {
            if (sscanf(Input, "%d", &Age) == 1 && IsValidAge(Age))
            {
                return Age;
            }
        }
        printf("Invalid age. Age must be > 0 and < 130.\n");
    }
}

void GetName(char *Name, int MaxLength)
{
    while (1)
    {
        printf("Enter name of the user: ");
        if (fgets(Name, MaxLength, stdin))
        {
            int Length = StringLength(Name);
            if (Length > 0 && Name[Length - 1] == '\n')
            {
                Name[Length - 1] = '\0';
            }
            if (StringLength(Name) > 0)
            {
                return;
            }
        }
        printf("Invalid name. Please enter a non-empty name.\n");
    }
}

void Create()
{
    FILE *WriteFilePointer = fopen("users.txt", "a");
    User User;
    if (!WriteFilePointer)
    {
        perror("Error writing to file");
        return;
    }
    User.Id = GetId();
    if (UserPresent(User.Id))
    {
        printf("----User ID already exists----\n");
        fclose(WriteFilePointer);
        return;
    }
    GetName(User.Name, sizeof(User.Name));
    User.Age = GetAge();
    WriteUser(WriteFilePointer, &User);
    printf("\n----Data entered successfully-----\n");
    fclose(WriteFilePointer);
}

void Read()
{
    FILE *ReadFilePointer = fopen("users.txt", "r");
    char BufferString[100];
    if (ReadFilePointer == NULL)
    {
        perror("----Error opening file----");
        exit(EXIT_FAILURE);
    }
    while (fgets(BufferString, sizeof(BufferString), ReadFilePointer))
    {
        printf("%s", BufferString);
    }
    fclose(ReadFilePointer);
}

void Update()
{
    FILE *ReadFilePointer = fopen("users.txt", "r");
    FILE *TempFilePointer = fopen("temp.txt", "w");
    User User;
    int TargetId;
    char BufferString[100];
    if (!TempFilePointer || !ReadFilePointer)
    {
        printf("Error opening file");
        fclose(TempFilePointer);
        fclose(ReadFilePointer);
        return;
    }
    TargetId = GetId();
    if (UserPresent(TargetId))
    {
        while (fgets(BufferString, sizeof(BufferString), ReadFilePointer))
        {
            if (ReadUser(BufferString, &User) == 3)
            {
                if (User.Id == TargetId)
                {
                    GetName(User.Name, sizeof(User.Name));
                    User.Age = GetAge();
                }
                WriteUser(TempFilePointer, &User);
            }
        }
    }
    else
    {
        printf("User not found\n");
        fclose(TempFilePointer);
        fclose(ReadFilePointer);
        remove("temp.txt");
        return;
    }

    fclose(TempFilePointer);
    fclose(ReadFilePointer);
    remove("users.txt");
    rename("temp.txt", "users.txt");
    printf(" ----User updated successfully----\n");
}

void Delete()
{
    FILE *ReadFilePointer = fopen("users.txt", "r");
    FILE *TempFilePointer = fopen("temp.txt", "w");
    User User;
    int TargetId;
    char BufferString[100];

    if (!TempFilePointer || !ReadFilePointer)
    {
        printf("----Error opening file----");
        return;
    }
    TargetId = GetId();
    if (UserPresent(TargetId))
    {
        while (fgets(BufferString, 100, ReadFilePointer))
        {
            if (ReadUser(BufferString, &User) == 3)
            {
                if (User.Id != TargetId)
                {
                    WriteUser(TempFilePointer, &User);
                }
            }
        }
    }
    else
    {
        printf("----User ID not present, Please Enter a valid ID\n----");
        fclose(ReadFilePointer);
        fclose(TempFilePointer);
        remove("temp.txt");
        return;
    }
    fclose(ReadFilePointer);
    fclose(TempFilePointer);
    remove("users.txt");
    rename("temp.txt", "users.txt");
    printf("----User Deleted Successfully----\n");
}

int main()
{
    char Input[10];
    int Operation;
    while (1)
    {
        printf("\nEnter the operation you want to perform\n");
        printf("1. Create\n");
        printf("2. Read\n");
        printf("3. Update\n");
        printf("4. Delete\n");
        printf("5. Exit\n");
        fgets(Input, sizeof(Input), stdin);

        if (sscanf(Input, "%d", &Operation) != 1)
        {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            continue;
        }

        switch (Operation)
        {
        case 1:
            Create();
            break;
        case 2:
            Read();
            break;
        case 3:
            Update();
            break;
        case 4:
            Delete();
            break;
        case 5:
            printf("-------------EXITING-------------\n");
            return 0;
        default:
            printf("Enter a valid operation\n");
        }
    }

    return 0;
}
