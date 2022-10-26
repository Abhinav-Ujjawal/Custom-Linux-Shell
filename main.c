#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>
#include <pthread.h>
char cpyinput[200];
void *Thread(void *);
void handleThread(char*u){
    strcpy(cpyinput,u);
    char*t=strtok(u," ");
    // char *cur=malloc(100*sizeof(char));
    char cur[100] = "/home/abhinav/os/assignment/assignment1/";
    if (!strcmp(t,"ls")){
        strcat(cur,"./ls.out ");
        strcat(cur,cpyinput);
        pthread_t t;
        pthread_create(&t,NULL,Thread,(void*)cur);
        pthread_join(t,NULL);
    }
    else if (!strcmp(t,"cat")){
        strcat(cur,"./cat.out ");
        strcat(cur,cpyinput);
        pthread_t t;
        pthread_create(&t,NULL,Thread,(void*)cur);
        pthread_join(t,NULL);
    }
    else if (!strcmp(t,"date")){
        strcat(cur,"./date.out ");
        strcat(cur,cpyinput);
        pthread_t t;
        pthread_create(&t,NULL,Thread,(void*)cur);
        pthread_join(t,NULL);
    }
    else if (!strcmp(t,"mkdir")){
        strcat(cur,"./Mkdir.out ");
        strcat(cur,cpyinput);
        pthread_t t;
        pthread_create(&t,NULL,Thread,(void*)cur);
        pthread_join(t,NULL);
    }
    else if (!strcmp(t,"rm")){
        strcat(cur,"./rm.out ");
        strcat(cur,cpyinput);
        pthread_t t;
        pthread_create(&t,NULL,Thread,(void*)cur);
        pthread_join(t,NULL);
    }
    else{
        printf("Shell: no such command detected\n");
    }
    return;
}

void* Thread(void*g){
    char*u=(char*)g;
    system(u);
}

void pwd(char*args){
    char CWD[PATH_MAX];
    getcwd(CWD,sizeof(CWD));
    if(!args || !strcmp(args,"-L")){
        printf("%s\n",CWD);
    }
    else if (!strcmp(args,"-P")){
        printf("%s\n",CWD);
    }
    else if (args[0]=='-'){
        printf("Invalid option detected. The available options are -n and -E\n");
    }
    else{
        printf("%s\n",CWD);
    }
}
char LAST_DIR[PATH_MAX];
int cd(char*args){
    char CUR_DIR[PATH_MAX];
    getcwd(CUR_DIR,sizeof(CUR_DIR));
    if (!args){
        args = getenv("HOME");
    }
    else if (!strcmp(args,"~")){
        args = getenv("HOME");
    }
    else if (!strcmp(args,"-")){
        if (*LAST_DIR=='\0'){
            printf("shell: cd: OLDPWD not set\n");
            return 1;
        }
        else{
            printf("%s\n",LAST_DIR);
            args = LAST_DIR;
        }
    }
    else if (args[0]=='-'){
        printf("shell: cd: %c: invalid option\n",args[1]);
        return 1;
    }
    
    if(chdir(args)!=0){
        printf("Shell: cd: %s\n",strerror(errno));
    }
    else{
        strcpy(LAST_DIR,CUR_DIR);
        // setenv("PWD",args,1);
    }
    return 0;
}
int echo(char*args){
    char*flag = strtok(NULL," ");
    char* copy = flag;
    if (flag==NULL){
        printf("\n");
    }
    else if (!strcmp(flag,"~")){
        copy+=1;
        printf("%s\n",getenv("HOME"));
    }
    else if (!strcmp(flag,"-n")){
        copy=strtok(NULL," ");
        while (copy!=NULL){
            printf("%s ",copy);
            copy=strtok(NULL," ");
        }
    }
    else if (!strcmp(flag,"-E")){   // default
        copy+=3;
        printf("%s",copy);
        printf("\n");
    }
    else if (!strcmp(flag,"--help")){
        // enable -n echo
        printf("Usage: echo [SHORT-OPTION]... [STRING]...\n\
  or:  echo LONG-OPTION\n\
Echo the STRING(s) to standard output.\n\
\n\
  -n             do not output the trailing newline\n\
  -E             disable interpretation of backslash escapes (default)\n\
  --help         display this help and exit\n\
\n\
NOTE: your shell may have its own version of echo, which usually supersedes\n\
the version described here.  Please refer to your shell's documentation\n\
for details about the options it supports.\n\
\n\
NOTE: printf(1) is a preferred alternative,\n\
which does not have issues outputting option-like strings.\n\
\n\
GNU coreutils online help: <https://www.gnu.org/software/coreutils/>\n\
Full documentation <https://www.gnu.org/software/coreutils/echo>\n\
or available locally via: info \'(coreutils) echo invocation\'\n");
    }
    else if(flag[0]!='-'){
        printf("%s",copy);
        copy=copy+strlen(copy)+1;
        printf(" %s\n",copy);
    }
    else{
        printf("Invalid option given to echo command. The available options are -n and -E");
        return 1;
    }
    return 0;
}
int startInternal(char *input){
    char* command = strtok(input," ");
    char cdlist[]="cd";
    char echolist[]="echo";
    if (!strcmp(command,cdlist)){
        command = strtok(NULL," ");
        cd(command);
        return 1;
    }
    else if (!strcmp(command,echolist)){
        echo(command);
        return 1;
    }
    else if (!strcmp(command,"pwd")){
        command = strtok(NULL," ");
        pwd(command);
        return 1;
    }
    else if (!strcmp(command,"ls")|| !strcmp(command,"cat")|| !strcmp(command,"rm")|| !strcmp(command,"date")|| !strcmp(command,"mkdir")){
        return 0;
    }
    return -1;
}
int startExternal(char*input){
    char * todo[10];
    char* command;
    int index=1;
    todo[0]=input;
    todo[1]=strtok(NULL," ");
    todo[2]=strtok(NULL," ");
    todo[3]=strtok(NULL," ");
    todo[4]=strtok(NULL," ");
    todo[5]=strtok(NULL," ");
    todo[6]=strtok(NULL," ");
    todo[7]=strtok(NULL," ");
    todo[8]=strtok(NULL," ");
    todo[9]=strtok(NULL," ");
    // todo[1]=strtok(NULL," ");
    // printf("---->%s",todo[0]);
    // printf("---->%s",todo[1]);
    // if (command!=NULL){
    //     todo[1]=command;
    // }
    // if (!todo[1]){
    //     printf("!%s\n",todo[0]);
    // }
    if (!strcmp(todo[0],"ls")){
        // char *x [50];
        char*args=strtok(NULL," ");
        // printf("%s\n",args);
        // args=strtok(input," ");
        // printf("~~~~%s\n\n",args);
        // while (args){
        // }
        pid_t k = fork();
        if (k==0){
                if (todo[3]!=NULL){
                    printf("ls: Too many arguments\n");
                }
                else if (todo[2]!=NULL){
                    execl("/home/abhinav/os/assignment/assignment1/ls.out","./ls.out",todo[0],todo[1],todo[2],NULL);
                }
                else if (todo[1]!=NULL)
                    execl("/home/abhinav/os/assignment/assignment1/ls.out","./ls.out",todo[0],todo[1],NULL);
                else{
                    execl("/home/abhinav/os/assignment/assignment1/ls.out","./ls.out",todo[0],NULL);
                }
        }
        else if (k<1){
            printf("Fork system call execution failed. Please try again");
        }
        else{
            wait(NULL);
        }
    }
    else if (!strcmp(todo[0],"date")){
        char*args=strtok(NULL," ");
        pid_t k = fork();
        if (k==0){
            if (todo[3]!=NULL){
                printf("date: Too many arguments\n");
            }
            if (todo[2]!=NULL){
                execl("/home/abhinav/os/assignment/assignment1/date.out","./date.out",todo[0],todo[1],todo[2],NULL);
            }
            else if (todo[1]!=NULL)
                execl("/home/abhinav/os/assignment/assignment1/date.out","./date.out",todo[0],todo[1],NULL);
            else{
                execl("/home/abhinav/os/assignment/assignment1/date.out","./date.out",todo[0],NULL);
            }
        }
        else if (k<1){
            printf("Fork system call execution failed. Please try again.");
        }
        else{
            wait(NULL);
        }
    }
    else if (!strcmp(todo[0],"rm")){
        pid_t k = fork();
        if (k==0){
            // if (todo)
            // if (todo[10]!=NULL){
            // char**hell = malloc(10*sizeof(char*));
            // for (int i=0;i<10;i++){
            //     hell[i] = malloc(20*(sizeof(char)));
            //     hell[i]=todo[i];
            //     printf("%s<-\n",hell[i]);
            // }
            execl("/home/abhinav/os/assignment/assignment1/rm.out","./rm.out",cpyinput,NULL);
            // }
            // else{
                // execl("/home/abhinav/os/assignment/assignment1/rm.out","./rm.out",todo[0],NULL);
            // }
        }
        else if (k<1){
            printf("Fork system call execution failed. Please try again.\n");
        }
        else{
            wait(NULL);
        }
    }
    else if (!strcmp(todo[0],"mkdir")){
        pid_t k = fork();
        if (k==0){
            execl("/home/abhinav/os/assignment/assignment1/Mkdir.out","./Mkdir.out",cpyinput,NULL);
        }
        else if (k<1){
            printf("Fork system call execution failed. Please try again.\n");
        }
        else{
            wait(NULL);
        }
    }
    else if (!strcmp(todo[0],"cat")){
        pid_t k = fork();
        if (k==0){
            execl("/home/abhinav/os/assignment/assignment1/cat.out","./cat.out",cpyinput,NULL);
        }
        else if (k<1){
            printf("Fork system call execution failed. Please try again.\n");
        }
        else{
            wait(NULL);
        }
    }
}
int main(){
    char CWD[PATH_MAX];
    *LAST_DIR='\0';
    while(1){
        getcwd(CWD,sizeof(CWD));
        printf("\033[1;36m");
        printf("[%s]",CWD);
        printf("\033[1;37m");
        printf("$ ");
        printf("\033[0m");
        char*input = (char*)malloc(sizeof(char)*200);
        if (!input){
            printf("Shell encountered a memory allocation failure! Please try again.");
            return 0;
        }
        else{
            fgets(input,200,stdin);
            char* last=input+strlen(input)-1;
            while(last>=input && isspace(*last)){
                last--;
            }
            *(last+1)='\0';
            while(last>=input && isspace(*input)){
                input+=1;
            }
            strcpy(cpyinput,input);
        }
        // printf(input);
        if (!strcmp(input,"")){
            continue;
        }
        int tflag=0;
        if (strlen(input)>2 && input[strlen(input)-1]=='t' && input[strlen(input)-2]=='&'){
            tflag=1;
        }
        if (strlen(input)==2 && input[strlen(input)-1]=='t' && input[strlen(input)-2]=='&'){
            printf("Command not found before &t\n");
            continue;
        }
        char * inputcopy = input;
        if (tflag==0){
            int execute = startInternal(input);
            if (!execute){
                // printf("---!->%s",inputcopy);
                startExternal(inputcopy);
            }
            else if(execute==-1) {
                printf("Shell: No such command detected\n");
            }
        }
        else{
            input[strlen(input)-1]='\0';
            input[strlen(input)-1]='\0';
            char* last=input+strlen(input)-1;
            while(last>=input && isspace(*last)){
                last--;
            }
            *(last+1)='\0';
            while(last>=input && isspace(*input)){
                input+=1;
            }
            strcpy(cpyinput,input);
            handleThread(input);
        }
        // printf("\n");
        // execv("../rm.c",);
        // break;
    }
    return 0;
}