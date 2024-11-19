#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char* USER_FORMAT = "{%d,%s,%d}\n";
typedef struct users{
    int age;
    int id;
    char name[20];
}users;


void create(){

    FILE* write_fptr = fopen("users.txt","a");
    FILE* read_fptr = fopen("users.txt","r");
    users user;
    // users user ={
    //     .id = 1,
    //     .name = "xxxx",
    //     .age = 20,
    // };
    users temp;
    char mystring[100];
    char bufferString[100];
    printf("Enter id of the user :");
    scanf("%d", &user.id);
    
    while(fgets(bufferString,100,read_fptr)){
        sscanf(bufferString,USER_FORMAT,&temp.id,temp.name,&temp.age);

        if(temp.id == user.id){
            printf("User ID already exists\n");
            return;
        }
    }

    printf("Enter name of the user :");
    scanf("%19s", user.name);
    printf("Enter age of the user : ");
    scanf("%d",&user.age);
    
    

    fprintf(write_fptr,USER_FORMAT,user.id,user.name,user.age);
    fflush(write_fptr);
    printf("Data entered successfully\n");
    fclose(read_fptr);
    fclose(write_fptr);
}

void read(){
   
    FILE* read_fptr = fopen("users.txt","r");

    char mystring[100];
    if (read_fptr == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while(fgets(mystring,sizeof(mystring),read_fptr)){
        printf("%s",mystring);
    }
    fclose(read_fptr);
    
}

void update(){

    FILE* read_fptr = fopen("users.txt","r");
    users user;
    int targetId;
    FILE* temp_fptr = fopen("temp.txt","a");
    char bufferString[100];

    if(temp_fptr == NULL){
        printf("Could not create temp file");
        exit(-1);
    }

    printf("Enter id you want to update");
    scanf("%d",&targetId);


    while(fgets(bufferString,sizeof(bufferString),read_fptr)){
        sscanf(bufferString,USER_FORMAT,&user.id,&user.name,&user.age);
        
        if(user.id == targetId){
            printf("Enter new name");
            scanf("%19s",user.name);

            printf("Enter new age");
            scanf("%d",&user.age);
        }
        fprintf(temp_fptr,USER_FORMAT,user.id,user.name,user.age);
    }

    fclose(temp_fptr);
    fclose(read_fptr);
    remove("users.txt");
    rename("temp.txt","users.txt");

    printf("users updated succesfully");

    fclose(read_fptr);
    
}

void delete(){

    FILE* read_fptr = fopen("users.txt","r");
    FILE* temp_fptr;
    users user;
    temp_fptr = fopen("temp.txt","a");
    int targetId;
    char bufferString[100];
    printf("Enter the id you want to delete");
    scanf("%d",&targetId);

    while(fgets(bufferString,100,read_fptr)){
        sscanf(bufferString,USER_FORMAT,&user.id,&user.name,&user.age);

        if(user.id == targetId) continue;

        fprintf(temp_fptr,USER_FORMAT,user.id,user.name,user.age);

    }
    fclose(read_fptr);
    fclose(temp_fptr);

    remove("users.txt");
    rename( "temp.txt" ,"users.txt");
}


void main(){
    FILE* write_fptr;
    FILE *read_fptr; 

    int op;
    write_fptr = fopen("users.txt","a");

    read_fptr = fopen("users.txt","r");


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
                    create(write_fptr,read_fptr);
                    break;
            case 2 : 
                    read(read_fptr);
                    break;
            case 3 : 
                    update(read_fptr);
                    break;

            case 4 : 
                    delete(read_fptr);
                    break;

            case 5 : 
                    fclose(write_fptr);
                    fclose(read_fptr);
                    exit(0);

            default : printf("Enter a valid operation\n");

        }
    }
    
  
}