//
// Created by pang on 19-1-17.
//

#ifndef MYSHELL_PHSH_H
#define MYSHELL_PHSH_H

#include <list>
#include "pch.h"
#include "Processer.h"

class Phsh{
public:
    Phsh(): MAX_CMDS_(50){

    }
    ~Phsh() = default;
    void do_sh();
    std::string read_cmd();


private:
    Processer processer_;
    std::string cur_dir_;
    int MAX_CMDS_;
    std::list<std::string> pre_cmds_;

    std::vector<std::string> split_line(std::string cmdline);
    void init();
};




#endif //MYSHELL_PHSH_H
