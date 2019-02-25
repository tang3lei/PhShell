//
// Created by pang on 19-1-16.
//

#ifndef MYSHELL_SMSH_H
#define MYSHELL_SMSH_H

#include "../pch.h"

#define YES 1
#define NO 1


//splitline.c
char *next_cmd();
char **splitline(char *);
void freelist(char **);
void *emalloc(size_t);
void *erealloc(void *, size_t);

//execute.c
int execute(char **);

//smsh.c
void fatal(char*,char*,int);

//process.c
extern int process(char **);

//controflow.c
extern int ok_to_execute(void);
extern int is_control_command(char *s);
extern int do_control_command(char **arglist);

//buildin.c
extern int buildin_command(char **arglist,int *rv);
#endif //MYSHELL_SMSH_H
