//
// Created by pang on 19-1-15.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#define MAXARGS 20
#define ARGLEN 100

int flag=0;

void execute(char *arglist[]){
    int pid,exitstatus;

    pid=fork();
    switch (pid){
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            execvp(arglist[0],arglist);
            perror("exec failed");
            exit(1);
        default:
            flag=1;
            while (wait(&exitstatus) != pid);
            printf("child exited with status %d,%d\n",
                    exitstatus>>8,exitstatus&0377);
            flag=0;
    }
}

char *makestring(char* buf){
    char *cp;

    buf[strlen(buf)-1] = '\0';

    cp = malloc(strlen(buf)+1);
    if(cp==NULL){
        fprintf(stderr,"no memory\n");
        exit(1);
    }
    strcpy(cp,buf);
    return cp;
}

void dealsigint(int signum){
    if(flag==0){
        printf("%d收到信号\n",getpid());
        exit(0);
    }else if(flag==1){

    }
}
int main(){


    signal(SIGINT,dealsigint);
    char *arglist[MAXARGS+1];
    int numargs;
    char argbuf[ARGLEN];

    numargs=0;
    while (numargs<MAXARGS){
        printf("Arg[%d]? ",numargs);
        if(fgets(argbuf,ARGLEN,stdin) && *argbuf!='\n'){
            arglist[numargs++] = makestring(argbuf);
            if(strcmp(arglist[numargs-1],"exit\0")==0)return 0;
        }
        else {
            if(numargs > 0){
                arglist[numargs] = NULL;
                execute(arglist);
                numargs=0;
            }
        }
    }
    return 0;
}
