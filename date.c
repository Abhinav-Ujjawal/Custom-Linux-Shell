#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
int main(int n, char**args){
    // for(int i=0;i<n;i++){
    //     printf("%s<-\n",args[i]);
    // }
    struct tm* inf;
    time_t t = time(&t);
    char date[100];
    if (t==-1){
        printf("Error in the time function.\n");
        return 1;
    }
    if(n==2){
        inf = localtime(&t);
        strftime(date,100,"%a %b %d %I:%M:%S %p %Z %Y",inf);     // formatting date and time
        printf("%s\n",date);
    }
    else if (!strcmp(args[2],"-u")){
        inf = gmtime(&t);
        strftime(date,100,"%a %b %d %I:%M:%S %p UTC %Y",inf);
        printf("%s\n",date);
    }
    else if (!strcmp(args[2],"-R")){
        inf = localtime(&t);
        strftime(date,100,"%a, %d %b %Y %H:%M:%S %z",inf);
        printf("%s\n",date);
    }
    else if (args[2][0]=='-'){
        printf("No such flag found in the date command.\n");
    }
    else{
        printf("Too many arguments for date command.\n");
    }
    return 1;
}