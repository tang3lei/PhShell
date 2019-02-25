//
// Created by pang on 19-1-18.
//

#ifndef MYSHELL_VARLIB_H
#define MYSHELL_VARLIB_H

#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	MAXVARS	200

using std::string;

struct node{
    string str;
    int global;
    node():global(0){
    }
    node(string s,int g):str(s),global(g){

    }

};

class Varlib {
public:
    Varlib(){
        tab.assign(MAXVARS,node());
    }
    void vlist();
    bool vadd(string &s1);
    void vexport(string &s1);
    int VLenviron2table(char *env[]);
    char ** VLtable2environ();
    void VLtabletoenviron(char *env[]);

private:
    bool okname(string &str);
    string keyfind(string &str);
    std::vector<node> tab;
};


#endif //MYSHELL_VARLIB_H
