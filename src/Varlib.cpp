//
// Created by pang on 19-1-18.
//

#include "Varlib.h"
#include <set>

#define	MAXVARS	200

using std::string;

void Varlib::vlist() {
    for(auto &x:tab){
        if(x.str.empty())break;
        if(x.global==1)
            std::cout<<"  * ";
        else
            std::cout<<"    ";
        std::cout<<x.str;
        std::cout<<std::endl;
    }
}

bool Varlib::vadd(string &s1) {
    string key = keyfind(s1);
    if(!okname(key))
        return false;
    for(auto &x:tab){
        if(x.str.empty() || keyfind(x.str)==key)
        {
            x.str = s1;
            return true;
        }
    }
    //存在空值???
    return false;
}

void Varlib::vexport(string &s1) {
    int tag=1;
    for(auto &x:tab){
        string tmp = keyfind(x.str);
        if(tmp==s1){
            x.global = 1;
            tag=0;
        }
    }
    if(tag){
        node tmp;
        tmp.str = s1+"=";
        tmp.global = 1;
        tab.push_back(tmp);
    }
}

bool Varlib::okname(string &str) {
    if(str.empty())return false;
    for(int i=0;i<str.size();i++){
        if((isdigit(str[i])&&i==0) || !(isalnum(str[i]) || str[i]=='_'))
            return false;
    }
    return true;
}

string Varlib::keyfind(string &str) {
    size_t pos = str.find('=');
    return str.substr(0,pos);
}

int Varlib::VLenviron2table(char *env[])
{
    int     i;
    char	*newstring;

    for(i = 0 ; env[i] != NULL ; i++ )
    {
        if ( i == MAXVARS )
            return 0;
        newstring =(char*) malloc(1+strlen(env[i]));
        if ( newstring == NULL )
            return 0;
        strcpy(newstring, env[i]);
        tab[i].str = string(newstring);
        tab[i].global = 1;
        //std::cout<<tab[i].str<<std::endl;
    }

    return 1;
}


char ** Varlib::VLtable2environ()

{
    int	    i,
            j,
            n = 0;
    char	**envtab;


    for( i = 0 ; i<MAXVARS && !tab[i].str.empty() ; i++ )
        if ( tab[i].global == 1 )
            n++;

    /* then, allocate space for that many variables	*/
    envtab = (char **) malloc( (n+1) * sizeof(char *) *500);
    if ( envtab == NULL )
        return NULL;

    /* then, load the array with pointers		*/
    for(i = 0, j = 0 ; i<MAXVARS && !tab[i].str.empty() ; i++ )
        if ( tab[i].global == 1 )
            envtab[j++] = const_cast<char*>(tab[i].str.c_str());
            //strcpy(envtab[j++],tab[i].str.c_str());
    envtab[j] = NULL;
    return envtab;
}

void Varlib::VLtabletoenviron(char *env[]) {
    /*
     * 确实能改变环境变量,但是echo仍然不能显示变量
     *  apue中写到,通常用putenv,getenv来访问环境变量,而不是environ
     *
     *
     */
    int     i;
    std::set<string> sysenv;
    char *newstring;
    for(i = 0 ; env[i] != NULL ; i++ )
    {
        if ( i == MAXVARS )
            return;
        string str(env[i]);
        sysenv.insert(str);
    }

    for(auto &x:tab){
        if(x.str.empty())break;
        if(x.global==1){
            auto it = sysenv.find(x.str);
            if(it==sysenv.end()){
                newstring = (char*)malloc(x.str.size()+5);
                strcpy(newstring,x.str.c_str());
                putenv(newstring);
            }
        }
    }
}
