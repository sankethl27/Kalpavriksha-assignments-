#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


#define MAX_LEN 200
typedef struct{
    int age;
    int id;
    char name[20];
}User;

bool is_valid_id(int id){
    return id > 0;
}

bool is_valid_age(int age){
    return age > 0 && age < 130;
}


int read_user(const char* bufferString, User* user) {
    return sscanf(bufferString, " { \"id\": %d , \"name\": \"%19[^\"]\" , \"age\": %d } ",
        &user->id, user->name, &user->age);
}

void write_user(FILE* write_fptr, User* user) {
    fprintf(write_fptr, "{");
    fprintf(write_fptr, " \"id\": %d ,", user->id);
    fprintf(write_fptr, " \"name\": \"%s\" ,", user->name);
    fprintf(write_fptr, " \"age\": %d ", user->age);
    fprintf(write_fptr, "}\n");
}


bool user_present(int id){
    User temp;
    FILE* read_fptr = fopen("users.txt","r");
    char bufferString[MAX_LEN];
    bool ans = false;
    if (!read_fptr) {
        perror("Error opening file");
        fclose(read_fptr);
        ans = false;
    }
    else {
        while(fgets(bufferString , sizeof(bufferString) ,read_fptr)){
            if(read_user(bufferString , &temp) == 3){
                if(temp.id == id) ans =  true;
            }

        }
    }
   
    fclose(read_fptr);
    return ans;
}


void create(){

    FILE* write_fptr = fopen("users.txt","a");
    User user;
    char mystring[100];
    char bufferString[100];

    
    if(!write_fptr) {
        perror("Error writing to file");
        return;
    }

    printf("Enter id of the user (ENTER INTEGER):");
    scanf("%d", &user.id);
    getchar();
    if(!is_valid_id(user.id)){
        printf("Invalid ID");
        return;
    }

    if(!user_present(user.id)){
        printf("Enter name of the user (ENTER TEXT):");
        fgets(user.name,100,stdin);
        int len = strlen(user.name);
        if (len > 0 && user.name[len - 1] == '\n') {
                user.name[len - 1] = '\0';
        }
        else {
            printf("----Enter valid name----");
            fclose(write_fptr);
            return;
        }
        printf("Enter age of the user (ENTER INTEGER): ");
        scanf("%d",&user.age);
        if(!is_valid_age(user.age)){
            printf("----Enter valid age , age > 0 and age < 130----\n");
            fclose(write_fptr);
            return;
        }
        write_user(write_fptr ,&user);
    }
    else{
        printf("----User ID already exists----\n");
        fclose(write_fptr);  
        return;
    }

    fflush(write_fptr);
    printf("----Data entered successfully-----\n");
    fclose(write_fptr);
}

void read(){

    FILE* read_fptr = fopen("users.txt","r");

    char mystring[100];
    if (read_fptr == NULL) {
        perror("----Error opening file----");
        exit(EXIT_FAILURE);
    }

    while(fgets(mystring,sizeof(mystring),read_fptr)){
        printf("%s",mystring);
    }
    fclose(read_fptr);
    
}

void update(){

    FILE* read_fptr = fopen("users.txt","r");
    FILE* temp_fptr = fopen("temp.txt","w");
    User user;
    int targetId;
    char bufferString[100];

    if(!temp_fptr || !read_fptr){
        printf("Error opening file");
        fclose(temp_fptr);
        fclose(read_fptr);
        return ;
    }

    printf("Enter id you want to update");
    scanf("%d",&targetId);
    if(!is_valid_id(targetId)){
        printf("----Invalid ID----");
        fclose(temp_fptr);
        fclose(read_fptr);
        return;
    }
    if(user_present(targetId)){
        while(fgets(bufferString,sizeof(bufferString),read_fptr)){
            if(read_user(bufferString, &user) == 3){
                if(user.id == targetId){
                printf("Enter new name");
                scanf("%19s",user.name);
                printf("Enter new age");
                scanf("%d",&user.age);
                }
            write_user(temp_fptr , &user);
        }
        
        }
    }
    else{
        printf("User not Found\n");
        fclose(temp_fptr);
        fclose(read_fptr);
        remove("temp.txt");
        return;
    }
 
    fclose(temp_fptr);
    fclose(read_fptr);
    remove("users.txt");
    rename("temp.txt","users.txt");
    printf(" ----User updated succesfully----");
}

void delete(){

    FILE* read_fptr = fopen("users.txt","r");
    FILE* temp_fptr = fopen("temp.txt","w");
    User user;
    int targetId;
    char bufferString[100];

    if(!temp_fptr || !read_fptr){
        printf("----Error opening file----");
        return;
    }
    printf("Enter the id you want to delete");
    scanf("%d",&targetId);
    if(!is_valid_id(targetId)){
        printf("Invalid ID");
        return;
    }

    if(user_present(targetId)){
        while(fgets(bufferString,100,read_fptr)){
            if(read_user(bufferString , &user) == 3){
                if(user.id != targetId){
                    write_user(temp_fptr , &user);
                }
                
            }
        }
    }
    else{
        printf("----User Id not present , Please Enter a valid Id\n----");
        fclose(read_fptr);
        fclose(temp_fptr);  
        remove("temp.txt");
        return;
    }
    
    fclose(read_fptr);
    fclose(temp_fptr);
    remove("users.txt");
    rename("temp.txt" ,"users.txt");
    printf("----User Deleted Succesfully----");
}

int main(){
    int op;
    while(1){
        printf("\nEnter the operation you want to perform\n");
        printf("1.Create\n");
        printf("2.Read\n");
        printf("3.Update\n");
        printf("4.Delete\n");
        printf("5.exit\n");
        scanf("%d",&op);
        switch(op){
            case 1 : 
                    create();
                    break;
            case 2 : 
                    read();
                    break;
            case 3 : 
                    update();
                    break;

            case 4 : 
                    delete();
                    break;

            case 5 :
                    printf("-------------EXITING-------------");
                    return 0;

            default : printf("Enter a valid operation\n");

        }
    }
    // create();
    // user_present(1) == 1?printf("User id is present\n") : printf("user id not present\n");
    // user_present(2) == 1?printf("User id is present\n") : printf("user id not present\n");
    
    return 0;

}