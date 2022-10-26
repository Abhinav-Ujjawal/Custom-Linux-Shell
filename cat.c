#include <dirent.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
int main(int n, char*args[]){

    char*t;
    char command[100]="cat ";
    if (n>2){
        for(int i=2;i<n;i++){
            strcat(command,args[i]);
            if (i!=n-1)
                strcat(command," ");
        }
        t = command;
    }
    else{
        t=args[1];
    }
    
    char *token = strtok(t," "),*flag;
    token = strtok(NULL," ");
    flag=token;
    char input[1000];
    if (!flag || !strcmp(flag,"-")){
        while(1){               // special behaviour for "cat" and "cat -"
            fgets(input,1000,stdin);
            if (!strcmp(input,"^C\n")){
                break;
            }
            fputs(input,stdout);
        }
    }
    else if (!strcmp(flag,"-n")){
        char *file =flag;
        file=strtok(NULL," ");
        if (!file){
            long long f=1;
            while(1){           // special behaviour for "cat -n"
                fgets(input,1000,stdin);
                if (!strcmp(input,"^C\n")){
                    break;
                }
                printf("     %lld  ",f++);
                fflush(stdout);
                fputs(input,stdout);
            }
        }
        long long f=1;
        int h=0;
        while(file!=NULL){          // normal "cat -n ..."
            struct stat s={0};
            char path[PATH_MAX]="\0";
            if (file[0]!='/'){
                getcwd(path,sizeof(path));
                strcat(path,"/");
            }
            strcat(path,file);
            if (stat(path,&s)==-1){
                printf("cat: %s: %s\n",file,strerror(errno));
                file=strtok(NULL," ");
                continue;
            }
            DIR* h = opendir(path);
            if (h!=NULL){
                printf("cat: %s: Is a directory\n",file);
                file=strtok(NULL," ");
                continue;
            }
            FILE* a=fopen(path,"r");
            char output[170];
            while(fgets(output,170,a)){
                // if (h==0)
                printf("     %lld  ",f++);
                fputs(output,stdout);
                fflush(stdout);
            }
            fclose(a);
            printf("\n");
            file=strtok(NULL," ");
        }
    }
    else if (!strcmp(flag,"-E")){
        char *file =flag;
        file=strtok(NULL," ");
        if (!file){             // special behaviour for "cat -E"
            while(1){
                fgets(input,1000,stdin);
                if (!strcmp(input,"^C\n")){
                    break;
                }
                input[strlen(input)-1]='\0';
                fputs(input,stdout);
                fputs("$\n",stdout);
                fflush(stdout);
            }
        }
        while(file){          // normal "cat -E ..."
            struct stat s={0};
            char path[PATH_MAX]="\0";
            if (file[0]!='/'){
                getcwd(path,sizeof(path));
                strcat(path,"/");
            }
            strcat(path,file);
            if (stat(path,&s)==-1){
                printf("cat: %s: %s\n",file,strerror(errno));
                file=strtok(NULL," ");
                continue;
            }
            DIR* h = opendir(path);
            if (h!=NULL){
                printf("cat: %s: Is a directory\n",file);
                file=strtok(NULL," ");
                continue;
            }
            FILE* a=fopen(path,"r");
            char output[170];
            char c=0;
            c=fgetc(a);
            // int y=0;
            while(c){
                if ((int)c==-1)break;
                if (c=='\n'){
                    fputs("$\n",stdout);
                    c=fgetc(a);
                    fflush(stdout);
                    continue;
                }
                fputc(c,stdout);
                // printf("%d",(int) c);
                // y=1;
                fflush(stdout);
                c=fgetc(a);
            }
            // if (y){
            //     fputc('$',stdout);
            // }
            fclose(a);
            file=strtok(NULL," ");
        }
    }
    else if (flag[0]=='-'){
        printf("cat: invalid option -- '%c'\n",flag[1]);
    }
    else {                // normal "cat ..."
        // printf("yes here\n");;
        // printf(flag);
        char *file =flag;
        while(file!=NULL){
            struct stat s={0};
            char path[PATH_MAX]="\0";
            if (file[0]!='/'){
                getcwd(path,sizeof(path));
                strcat(path,"/");
            }
            strcat(path,file);
            if (stat(path,&s)==-1){
                printf("cat: %s: %s\n",file,strerror(errno));
                file=strtok(NULL," ");
                continue;
            }
            DIR* h = opendir(path);
            if (h!=NULL){
                printf("cat: %s: Is a directory\n",file);
                file=strtok(NULL," ");
                continue;
            }
            FILE* a=fopen(path,"r");
            if (a==NULL){
                printf("Error: %s\n",strerror(errno));
                file=strtok(NULL," ");
                continue;
            }
            char output;
            while((output=fgetc(a))!=-1){
                fputc(output,stdout);
            }
            fflush(stdout);
            fclose(a);
            file=strtok(NULL," ");
        }
    }
    return 0;
}