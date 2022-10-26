#include <sys/stat.h> 
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h> 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
int main(int n, char*args[]){       // args does not include the command it is the whole input itself
    // for(int i=0;i<n;i++){
    //     printf("%s<-\n",args[i]);
    // }

    char*t;
    char command[100]="mkdir ";
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
    // printf(t);
    // return 0;
    char * token = strtok(t," ");
    token = strtok(NULL," ");
    if (!token){
        printf("mkdir: missing operand\n");
        return 0;
    }
    char CWD[400]="\0";
    if (token[0]!='-'||!strcmp(token,"-")){
        while (token)
        {
            if (token[0]!='/'){
                getcwd(CWD,sizeof(CWD));
                strcat(CWD,"/");
            }
            strcat(CWD,token);
            int h = mkdir(CWD,0777);
            if (h==0){
                // printf("mkdir: created directory \'%s\'\n",token);
            }
            else{
                printf("mkdir: cannot create directory \'%s\': %s\n",token,strerror(errno));
            }
            token=strtok(NULL," ");
        }
    }
    else if (!strcmp(token,"-v")){
        token = strtok(NULL," ");
        if (!token){
            printf("mkdir: missing operand\n");
            return 0;
        }
        while (token)
        {
            if (token[0]!='/'){
                getcwd(CWD,sizeof(CWD));
                strcat(CWD,"/");
            }
            strcat(CWD,token);
            int h = mkdir(CWD,0777);
            if (h==0){
                printf("mkdir: created directory \'%s\'\n",token);
            }
            else
            {
                // error for both 'mkdir a/b' as well as 'mkdir <already existing file>'
                printf("mkdir: cannot create directory \'%s\': %s\n",token,strerror(errno));
            }
            token=strtok(NULL," ");
        }
    }
    else if (!strcmp(token,"-p")){
        token = strtok(NULL," ");
        if (!token){
            printf("mkdir: missing operand\n");
            return 0;
        }
        char *arr[100];
        int i=0;
        char*r;
        while(token){
            r=(char*)malloc(100*sizeof(char));
            for(int p=0;p<strlen(token);p++){
                r[p]=token[p];
            }
            r[strlen(token)]='\0';
            arr[i]=r;
            // printf("-> %s ",arr[i]);
            // printf("- %s\n",token);
            i++;
            token = strtok(NULL," ");
        }
        // for(int j=0;j<i;j++){
        //    printf("%d->%s",j,arr[j]);
        // }
        // printf("!! i=%d\n",i);
        while((--i)!=-1){
            char*t=(char*)malloc(400*sizeof(char));
            getcwd(CWD,sizeof(CWD));
            strcpy(t,CWD);
            strcat(CWD,"/");
            char*u = (char*)malloc(100*sizeof(char));
            strcpy(u,arr[i]);
            char*cc = strtok(u,"/");
            if (arr[i][0]=='/'){
                strcpy(CWD,"/\0");
            }
            while(cc!=NULL){
                    strcat(CWD,cc);
                if (chdir(CWD)!=0){
                    mkdir(CWD,0777);
                    chdir(CWD);
                }
                strcat(CWD,"/");
                cc = strtok(NULL,"/");
            }
            free(u);
            chdir(t);
            free(t);
        }

    }
    else {
        printf("mkdir: invalid option -- '%c'\n",token[1]);
    }
    return 0;
}