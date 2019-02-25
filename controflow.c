//
// Created by pang on 19-1-16.
//

#include "pch.h"
#include "smsh.h"

enum status {NEUTRAL, WANT_THEN, THEN_BLOCK,ELSE_BOLCK,WANT_ELSE};
enum results {SUCCESS, FAIL};

static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;

int syn_err(char *);

int ok_to_execute(){
    int rv = 1;
    if( if_state == WANT_THEN){
        syn_err("then expected");
        rv = 0;
    }
    else if(if_state == THEN_BLOCK && if_result == SUCCESS)
        rv = 1;
    else if(if_state == THEN_BLOCK && if_result == FAIL)
        rv = 0;
    else if(if_state == ELSE_BOLCK && if_result == FAIL)
        rv = 1;
    else if(if_state == ELSE_BOLCK && if_result == SUCCESS)
        rv = 0;
    return rv;
}

int is_control_command(char *s) {
    return (strcmp(s,"if")==0 || strcmp(s,"then")==0
    || strcmp(s,"fi")==0 || strcmp(s,"else")==0);
}

int do_control_command(char **args){
    char *cmd = args[0];
    int rv = -1;
    if(strcmp(cmd,"if") == 0){
        if(if_state != NEUTRAL)
            rv = syn_err("if unexpected");
        else {
            last_stat = process(args+1);
            if(last_stat == 0){
                if_state = WANT_THEN;
                if_result = SUCCESS;
            } else{
                if_result = FAIL;
                if_state = WANT_ELSE;
            }
            rv = 0;
        }
    }
    else if(strcmp(cmd,"then") == 0){
        if(if_state == WANT_ELSE){
            rv=0;
        }
        else if(if_state != WANT_THEN)
            rv = syn_err("then unexpected");
        else{
         if_state = THEN_BLOCK;
         rv = 0;
        }
    }
    else if(strcmp(cmd,"fi") == 0){
        if(if_state != THEN_BLOCK)
            rv = syn_err("fi unexpected");
        else {
            if_state = NEUTRAL;
            rv = 0;
        }
    }
    else if(strcmp(cmd,"else") == 0){
        if(if_state != WANT_ELSE)
            rv = syn_err("else unexpected");
        else{
            if_state = ELSE_BOLCK;
            rv = 0;
        }
    }
    else
        fatal("internal error processing:",cmd,2);
    return rv;
}

int syn_err(char *msg){
    if_state = NEUTRAL;
    fprintf(stderr,"syntax error: %s\n",msg);
    return -1;
}