//
// Created by pang on 19-1-18.
//

#include <fcntl.h>
#include "Processer.h"

using std::string;
using std::stack;
using std::vector;

char reOutPath[1024];
char reInPath[1024];
extern char curdir[1024];
int backupInfd = 9;
int backupOutfd = 10;

void debug_print(vector<string> &cmd);

Processer::Processer() : resault_state_(SUCCESS), ctl_state_(NEUTRAL){
}

int Processer::process(command &cmd) {
    int rv=0;
    if(cmd.empty())return rv;

    bool reIn = false;
    bool reOut = false;
    int ec = 0;
    do_redirect(cmd,reIn,reOut,ec);
    if(ec){
        fprintf(stderr,"cann't redirect\n");
        return rv;
    }

    if(is_control_cmd(cmd)){
        int ctl_res;
        ctl_res = do_control_cmd(cmd);
        if(ctl_res){
            rv = execute();
        }
    } else if(ok_to_execute()){
        rv = execute(cmd);

    }

    if(reIn){
        reIn = false;
        dup2(backupInfd,0);
    }
    if(reOut){
        reOut = false;
        dup2(backupOutfd,1);
    }
    return rv;
}

void Processer::do_redirect(Processer::command &cmd, bool &reIn, bool &reOut,int &error_code) {
    for(int i=0;i<cmd.size();i++){

        if(!cmd[i].empty() && cmd[i][0]=='>'){
            if(reOut){
                error_code=2;
                return;
            }
            if(cmd[i].size()==1){
                if(i==cmd.size()-1){
                    error_code=1;
                    return;
                }
                else{
                    reOut = true;
                    strcpy(reOutPath,cmd[i+1].c_str());
                    cmd.erase(cmd.begin()+i+1);
                    cmd.erase(cmd.begin()+i);
                }
            }
            else{
                reOut = true;
                strcpy(reOutPath,cmd[i].c_str()+1);
                cmd.erase(cmd.begin()+i);
            }

            int newfd = open(reOutPath,O_WRONLY|O_CREAT|O_TRUNC,0644);
            if(newfd==-1){
                error_code=1;
                return;
            }
            if(dup2(1,backupOutfd)!=backupOutfd){
                error_code=1;
                return;
            }
            if(dup2(newfd,1)!=1){
                error_code=1;
                return;
            }
            close(newfd);

        }

        else if(!cmd[i].empty() && cmd[i][0]=='<'){
            if(reIn){
                error_code=2;
                return;
            }
            if(cmd[i].size()==1){
                if(i==cmd.size()-1){
                    error_code=1;
                    return;
                }
                else{
                    reIn = true;
                    strcpy(reInPath,cmd[i+1].c_str());
                    cmd.erase(cmd.begin()+i+1);
                    cmd.erase(cmd.begin()+i);
                }
            }
            else{
                reIn = true;
                strcpy(reInPath,cmd[i].c_str()+1);
                cmd.erase(cmd.begin()+i);
            }

            int newfd = open(reInPath,O_RDONLY);
            if(newfd==-1){
                error_code=1;
                return;
            }
            if(dup2(0,backupInfd)!=backupInfd){
                error_code=1;
                return;
            }
            if(dup2(newfd,0)!=0){
                error_code=1;
                return;
            }
            close(newfd);

        }

        else if(!cmd[i].empty() && cmd[0]=="|"){
            if(i==0 || i==cmd.size()){
                error_code =1;
                return;
            }

        }
    }
}

bool Processer::is_control_cmd(command &cmd){
    if(cmd[0] == "if" || cmd[0]=="else" || cmd[0]=="then" || cmd[0]=="fi")
        return  true;
    else return false;
}

bool Processer::do_control_cmd(Processer::command &cmd) {
    if(ctl_state_ == NEUTRAL) ctl_state_ = IF_EXIST;
    string entity = cmd[0];
    cmd_stack_.push(cmd);
    if(entity != "fi"){
        return false;
    }
    else {
        cmdstack tmp;
        while (!cmd_stack_.empty()){
            command top = cmd_stack_.top();
            cmd_stack_.pop();
            tmp.push(top);
            if(top[0]=="if") {
                break;
            }

            if(cmd_stack_.empty() || ctl_state_ != IF_EXIST) {
                syn_err("unexpected fi");
                return false;
            }

        }

        while (!tmp.empty()){
            command top = tmp.top();
            tmp.pop();

            if(!is_control_cmd(top)){
                if(ctl_state_ == IF_BLOCK){
                    if_cmd_ = top;
                    ctl_state_ = WANT_THEN;
                }
                else if(ctl_state_ == THEN_BLOCK){
                    ctl_state_ = THEN_FINISH;
                    then_cmd_ = top;
                }
                else if(ctl_state_ == ELSE_BOLCK){
                    ctl_state_ = ELSE_FINISH;
                    else_cmd_ = top;
                }
                else {
                    syn_err("to many commands");
                    return false;
                }
            }

            if(top[0]=="fi"){
                if(ctl_state_==THEN_FINISH || ctl_state_==ELSE_FINISH)
                    return true;
                else {
                        syn_err("unexpected fi");
                        return false;
                    }
            }

            else if(top[0]=="if"){
                if(ctl_state_ != IF_EXIST){
                    syn_err("unexpected if");
                    return false;
                }
                else {
                    ctl_state_ = IF_BLOCK;
                    if(top.size()>1){
                        if_cmd_ = command(top.begin()+1,top.end());
                        ctl_state_ = WANT_THEN;
                    }
                }
            }

            else if(top[0]=="then"){
                if(ctl_state_ != WANT_THEN){
                    syn_err("unexpected then");
                    return false;
                }
                else {
                    ctl_state_ = THEN_BLOCK;
                    if(top.size()>1){
                        then_cmd_ = command(top.begin()+1,top.end());
                        ctl_state_ = THEN_FINISH;
                    }
                }
            }

            else if(top[0]=="else"){
                if(ctl_state_ != THEN_FINISH){
                    syn_err("unexpected else");
                    return false;
                }
                else {
                    ctl_state_= ELSE_BOLCK;
                    if(top.size()>1){
                        else_cmd_ = command(top.begin()+1,top.end());
                        ctl_state_ = ELSE_FINISH;
                    }
                }
            }
        }
        return false;
    }
}

bool Processer::ok_to_execute() {
    if(ctl_state_==NEUTRAL) return true;
    else return false;
}

bool Processer::buildin_command(Processer::command &cmd) {
    bool rv = false;

    if(cmd.size()==1 && cmd[0]=="exit"){
        exit(0);
    }
    if(cmd[0]=="set"){
        //display all item
        varlib.vlist();
        return true;
    }
    else if(cmd[0].find('=')!=-1){
        //add to table key=val
        varlib.vadd(cmd[0]);
        return true;
    }
    else if(cmd[0]=="export"){
        //cmd[0] export cmd[1] key
        varlib.vexport(cmd[1]);
        return true;
    }
    else if(cmd[0]=="cd" && cmd.size()==2){
        if(chdir(cmd[1].c_str())==-1){
            fatal("cd pathname","wrong path",1);
        }
        getcwd(curdir,1024);
        return true;
    }


    return rv;
}

bool Processer::execute(command &cmd){
    if(buildin_command(cmd)){
        return true;
    }
    int pid;
    int child_info = -1;

    if(cmd.empty() || cmd[0] == "")
        return false;

    if((pid=fork())==-1){
        perror("fork failed");
    } else if(pid == 0){
        varlib.VLtabletoenviron(environ);
        //environ = varlib.VLtable2environ();

        signal(SIGINT,SIG_DFL);
        signal(SIGQUIT,SIG_DFL);

        size_t need = cmd.size();
        char *argv[need+5];
        for(size_t i = 0;i<need;i++){
            argv[i] = const_cast<char*>(cmd[i].c_str());
        }
        argv[need] = NULL;
        execvp(cmd[0].c_str(),argv);
        perror("cannot execute command");
        exit(1);
    }else{
        if(wait(&child_info)==-1){
            perror("wait");
        }
    }
    return !child_info;
    //0 is true 1 is false
}

bool Processer::execute() {
    int res;
    bool rv= false;
    res = execute(if_cmd_);
    if(res)
        rv=execute(then_cmd_);
    else if(ctl_state_==ELSE_FINISH)
        rv=execute(else_cmd_);
    ctl_state_ = NEUTRAL;
    return rv;
}

void Processer::fatal(std::string s1, std::string s2, int n) {
    fprintf(stderr,"Error: %s, %s\n",s1.c_str(),s2.c_str());
    exit(n);
}

int Processer::syn_err(string msg){
    ctl_state_ = NEUTRAL;
    fprintf(stderr,"syntax error: %s\n",msg.c_str());
    return -1;
}

void debug_print(vector<string> &cmd){
    for(auto x:cmd)std::cout<<x;
    std::cout<<std::endl;
}
