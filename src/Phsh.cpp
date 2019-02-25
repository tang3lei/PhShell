//
// Created by pang on 19-1-17.
//

#include "Phsh.h"

using std::string;
using std::stack;
using std::vector;

const string prompt("> ");
char curdir[1024];

void Phsh::do_sh() {
    string cmdline;
    init();

    while ((cmdline=read_cmd())!="exit"){
        vector<string> lines = split_line(cmdline);
        if(!lines.empty()){
            int resault;
            resault = processer_.process(lines);
        }
    }
}

string Phsh::read_cmd() {
    int pos = 0;
    char c;
    string buf="";

    printf("\033[33m%s %s\033[m",curdir,prompt.c_str());
    while ((c=getc(stdin)) != EOF){
        if(c=='\n')
            break;
        buf.push_back(c);
        pos++;
    }
    if(c==EOF && pos==0)
        return "exit";
    return buf;
}

vector<string> Phsh::split_line(string cmdline) {
    unsigned long pos = 0;
    unsigned long start;
    unsigned long len;
    vector<string> res;
    while (pos<cmdline.size()){
        while (cmdline[pos]==' ' || cmdline[pos]=='\t')pos++;
        if(pos == cmdline.size()) break;
        start = pos;
        len = 1;
        while (++pos!=cmdline.size() && (cmdline[pos]!=' ' && cmdline[pos]!='\t'))
            len++;
        res.push_back(cmdline.substr(start,len));
    }
    return res;
}

void Phsh::init() {
    getcwd(curdir,1024);
    processer_.varlib.VLenviron2table(environ);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
}

