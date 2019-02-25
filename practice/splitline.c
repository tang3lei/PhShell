//
// Created by pang on 19-1-16.
//

#include "../pch.h"
#include "smsh.h"

#define is_delime(x) ((x)==' ' || (x)=='\t')

char * next_cmd(char * prompt,FILE *fp){
    char *buf;
    int bufspace = 0;
    int pos = 0;
    int c;

    printf("%s",prompt);
    while ((c=getc(fp)) != EOF){
        if(pos+1>=bufspace){
            if(bufspace == 0){
                buf = emalloc(BUFSIZ);
            }
            else {
                buf = erealloc(buf,bufspace+BUFSIZ);
            }
            bufspace += BUFSIZ;
        }

        if(c=='\n')
            break;

        buf[pos++] = c;
    }
    if(c==EOF && pos==0)
        return NULL;
    buf[pos] = '\0';
    return buf;
}

char *newstr(char *s, int len) {
    char *rv = emalloc(len + 1);

    rv[len] = '\0';
    strncpy(rv, s, len);
    return rv;
}

char **splitline(char *line){
    char **args;
    int spots = 0;
    int bufspace = 0;
    int argnum = 0;
    char *cp = line;
    char *start;
    int len;

    if(line==NULL)
        return NULL;

    args = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ/ sizeof(char *);

    while (*cp != '\0'){
        while (is_delime(*cp))
            cp++;
        if(*cp=='\0')
            break;

        if(argnum + 1>=spots){
            args = erealloc(args,bufspace+BUFSIZ);
            bufspace += BUFSIZ;
            spots += (BUFSIZ/ sizeof(char*));
        }

        start = cp;
        len = 1;
        while (*++cp != '\0' && !(is_delime(*cp)))
            len++;

        args[argnum++] = newstr(start,len);
    }

    args[argnum] = NULL;
    return args;

}

void freelist(char **list) {
    char **cp = list;
    while (*cp)
        free(*cp++);
    free(list);
}

void *emalloc(size_t n) {
    void *rv;
    if ((rv = malloc(n)) == NULL)
        fatal("out of memory", "", 1);
    return rv;
}

void *erealloc(void *p, size_t n) {
    void *rv;
    if ((rv = realloc(p, n)) == NULL)
        fatal("realloc() failed", "", 1);
    return rv;
}

