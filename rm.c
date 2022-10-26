#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
int main(int n,char *args[]){       // args does not include the command it is the whole input itself
    // for(int i=0;i<n;i++){
    //     printf("%s<-\n",args[i]);
    // }
    // return 0;
    char*t;
    char command[100]="rm ";
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
    char *token = strtok(t," ");
    token = strtok(NULL," ");
    // printf(token);
    // printf("\n");
    // fflush(stdout);
    // return 0;
    char* flag = token;    // args [1] is  [rm .. ]
    if (!token){
        printf("rm: missing operand\n");
    }
    else{
        if (token[0]!='-'){
            while(token!=NULL){
                int e = unlink(token);
                if(e!=0){
                        printf("rm: cannot remove '%s': %s \n",token,strerror(errno));
                }
                token=strtok(NULL," ");
            }
        }
        else if (!strcmp(token,"-f")){
            while((token=strtok(NULL," "))!=NULL){
                unlink(token);
            }
        }
        else if (!strcmp(token,"-i")){
            token = strtok(NULL," ");
            if (!token){
                printf("rm: missing operand\n");
                return 0;
            }
            while((token)!=NULL){
                printf("Remove file \'%s\'? [Y/N] ",token);
                char *in=(char*)malloc(sizeof(char)*10);
			    fgets(in,10,stdin);
                char* last=in+strlen(in)-1;
                while(last>=in && isspace(*last)){
                    last--;
                }
                *(last+1)='\0';
                while(last>=in && isspace(*in)){
                    in+=1;
                }
                if (!strcmp("Y",in) || !strcmp("y",in)){
                    int e = unlink(token);
                    if(e!=0){
                        printf("rm: cannot remove '%s': %s \n",token,strerror(errno));
                    }
                }
                token=strtok(NULL," ");
            }
        }
        // else if (!strcmp(flag,"-r")){
        //     while((token=strtok(NULL," "))!=NULL){
        //         int e = remove(token);
        //         if(e!=0){
        //             printf("rm: cannot remove '%s': No such file or directory\n",token);
        //         }
        //     }
        // }
        else{
            printf("%c - No such flag found for the rm command\n",flag[1]);
        }
    }
    return 0;
}