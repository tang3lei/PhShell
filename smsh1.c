//
// Created by pang on 19-1-16.
//

#include "smsh.h"
#include "pch.h"

#define DFL_PROMPT "> "

void setup();

int main(){
    char *cmdline,*prompt,**arglist;
    int result;

    prompt = DFL_PROMPT;
    setup();

    while ((cmdline=next_cmd(prompt,stdin)) != NULL){
        if((arglist = splitline(cmdline)) != NULL){
            result = process(arglist);
            freelist(arglist);
        }
        free(cmdline);
    }
    return 0;
}

void setup(){
    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
}

void fatal(char *s1, char *s2, int n){
    fprintf(stderr,"Error: %s, %s\n",s1,s2);
    exit(n);
}