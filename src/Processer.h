//
// Created by pang on 19-1-18.
//

#ifndef MYSHELL_PROCESSER_H
#define MYSHELL_PROCESSER_H

#include "pch.h"
#include "Varlib.h"

class Processer {
    friend class Phsh;

    typedef std::vector<std::string> command;
    typedef std::stack<std::vector<std::string>> cmdstack;

public:
    Processer();
    ~Processer() = default;
    int process(command& cmd);

private:
    bool ok_to_execute();
    bool is_control_cmd(command  &cmd);
    bool do_control_cmd(command &cmd);
    bool buildin_command(command &cmd);
    bool execute(command &cmd);
    bool execute();
    void do_redirect(command &cmd,bool &reIn,bool &reOut,int &error_code);
    void fatal(std::string s1,std::string s2,int n);
    int syn_err(std::string msg);

    enum status {NEUTRAL, WANT_THEN, THEN_BLOCK,WANT_ELSE,THEN_FINISH,ELSE_FINISH,
            ELSE_BOLCK,IF_BLOCK,IF_EXIST,WANT_FINISH,FINISH};
    enum results {SUCCESS, FAIL};

    int ctl_state_;
    int resault_state_;

    cmdstack cmd_stack_;
    command if_cmd_;
    command then_cmd_;
    command else_cmd_;

    Varlib varlib;
};


#endif //MYSHELL_PROCESSER_H
