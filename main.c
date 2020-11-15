#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

void split(char line[],char cmd[],char arg[],char arg2[],int isEB){
    int ind1=0,ind2=0,ind3=0;
    int isArg=0;
    int n;
    if(isEB==1)n=strlen(line)-2;
    else n=strlen(line)-1;                          //(-1) becasu the fgets store the \n in the string
    for(int i=0;i<n;i++){
            if(line[i]==' '&&isArg==0&&i!=0){isArg=1;}        //split where the space
            else if(line[i]==' '&&isArg==1&&strlen(arg)!=0){isArg=2;}
            if(isArg==0){
                cmd[ind1++]=line[i];
            }else if(isArg==1){
                if(line[i]==' '&&ind2==0)continue;
                arg[ind2++]=line[i];
            }else if(isArg==2){
                if(line[i]==' '&&ind3==0)continue;
                arg2[ind3++]=line[i];
            }
    }
        cmd[ind1]='\0';                             //the end of string
        arg[ind2]='\0';                             //the end of string
        arg2[ind3]='\0';

}

void printInfo(){
        char cwd[PATH_MAX];                                          //string to hold the path
        getcwd(cwd, sizeof(cwd));                                    //get the working directory path

        printf("\033[0;32m");                          //make the color green
        printf("Bahaa@khalf:");
        printf("\033[0;33m");                          //make the color yellow
        printf(" %s >> ",cwd);                         //print the working directory
        printf("\033[0m");                             //make the color white
}

int main()
{

    char line[150];                                          //string to store the input from user in it
    char cmd[100];                                           //string to hold the command
    char arg[100];
    char arg2[100];                                          //string to hold the argument if it exist
    int isEB=0;                                             //flag to detect the &

    while(1){
        printInfo();                                          //printing the informatio touser
        isEB=0;
       // scanf("%[^\n]",line);
        fgets(line,100,stdin);                                  //take the input
        if(strcmp(line,"exit\n") == 0)exit(0);                  //check the exit command
        if(line[strlen(line)-2]=='&')isEB=1;                    //check the & command
        split(line,cmd,arg,arg2,isEB);                               //split the input to command string and argument string

        if(strcmp(cmd,"cd")==0&&strlen(arg)!=0) chdir(arg);          // check to change directory
        else{
            int n=2;
            if(strlen(arg)!=0)n=3;                                   //check if there an argument make the size of next array 3
            if(strlen(arg2)!=0)n=4;
            char *c[n];                                             //array of string to pass to execvp function
            c[0]=cmd;
            c[n-1]=NULL;
            if(n==3)c[1]=arg;
            else if(n==4){c[1]=arg;c[2]=arg2;}

            pid_t id;
            id=fork();

            if(id==0){                                             //child process
                printf("%s",execvp(c[0], c));

            }else{                                                 //parent process
                if(isEB==0)waitpid(id, NULL, WUNTRACED);
            }
        }

    }

    return 0;
}
