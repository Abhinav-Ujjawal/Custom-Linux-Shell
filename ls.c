#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
int main(int n,char*args []){
    DIR*directory = opendir(".");
    if (!directory){
        printf("Error faced in execution of opendir function\n");
        return 0;
    }
    struct dirent*d;
    // for(int i=0;i<n;i++){
    //     printf("%s<-\n",args[i]);
    // }
    int c=0;
    if(n==2){
            while((d=readdir(directory))!=NULL){
                if ('.'==d->d_name[0]){
                    continue;
                }
                c=1;
                printf("%s  ",d->d_name);
            }
    }
    else if (n==3){
        if (!strcmp("-a",args[2])){     // list all
            while((d=readdir(directory))!=NULL){
                c=1;
                printf("%s  ",d->d_name);
            }
        }
        else if (!strcmp(args[2],"-1")){     // list in line
            while((d=readdir(directory))!=NULL){
                if ('.'==d->d_name[0]){
                    continue;
                }
                c=1;
                printf("%s\n",d->d_name);
            }
            return 0;
        }
        else if (!strcmp(args[2],"-A")){     // list all except . and ..
            while((d=readdir(directory))!=NULL){
                if (!strcmp(".",d->d_name) || !strcmp("..",d->d_name)){
                    continue;
                }
                printf("%s  ",d->d_name);
                c=1;
            }
        }
        else if(args[2][0]=='-'){
            printf("Invalid option provided with the command ls\n");return 1;
        }
        else{                           // case of ls <filename>
            char path[PATH_MAX]="\0";
            if (args[2][0]!='/'){
                getcwd(path,sizeof(path));
                strcpy(path,"/");
            }
            strcpy(path,args[2]);
            directory = opendir(path);
            if (!directory){
                printf("Error: %s : %s\n",path,strerror(errno));
                return 0;
            }
            while((d=readdir(directory))!=NULL){
                if ('.'==d->d_name[0]){
                    continue;
                }
                c=1;
                printf("%s  ",d->d_name);
            }
        }
    }
    else if (n==4){                           // case of ls -option <filename>
        char path[PATH_MAX]="\0";
        if (args[3][0]!='/'){
            getcwd(path,sizeof(path));
            strcpy(path,"/");
        }
        strcpy(path,args[3]);
        directory = opendir(path);
        if (!directory){
            printf("Error: %s : %s\n",path,strerror(errno));
            return 0;
        }
        if (!strcmp("-a",args[2])){     // list all
            while((d=readdir(directory))!=NULL){
                c=1;
                printf("%s  ",d->d_name);
            }
        }
        else if (!strcmp(args[2],"-1")){     // list in line
            while((d=readdir(directory))!=NULL){
                if ('.'==d->d_name[0]){
                    continue;
                }
                c=1;
                printf("%s\n",d->d_name);
            }
            return 0;
        }
        else if (!strcmp(args[2],"-A")){     // list all except . and ..
            while((d=readdir(directory))!=NULL){
                if (!strcmp(".",d->d_name) || !strcmp("..",d->d_name)){
                    continue;
                }
                printf("%s  ",d->d_name);
                c=1;
            }
        }
        else if(args[2][0]=='-'){
            printf("Invalid option provided with the command ls\n");return 1;
        }
    }
    else{
        printf("No such options found for command ls -%d\n",n);return 1;
    }
    if (c)
        printf("\n");
    
    closedir(directory);
    return 0;
}