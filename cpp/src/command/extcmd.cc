/*
 * =====================================================================================
 *
 *       Filename:  extcmd.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/18/2012 03:22:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "extcmd.h"
//#include "../process/process.h"

//#include <vector>
//#include <deque>
//#include <iterator>
//#include <algorithm>
//#include <sstream>
#include <cstdlib>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#if 0
std::deque<std::string> 
ExternalCommand::split(std::string str) {
    std::deque<std::string> d;
    std::string current = "";

    char const * ps = str.c_str();
    size_t size = str.length();

    bool doublequote = false, singlequote = false;
    for(size_t i = 0; i < size; ++i) {
        switch(ps[i]) {
            case ' ':
                if((i+1 < size && ps[i+1] != ' ') && !doublequote && !singlequote) {
                    d.push_back(current);
                    current = "";
                } else if(doublequote || singlequote)
                    current += ' ';
                break;
            case '\"':
                if(i > 0 && ps[i-1] != '\\') {
                    if(doublequote)
                        doublequote = false;
                    else
                        doublequote = true;
                }
                current += '\"';
                break;
            case '\'':
                if(i > 0 && ps[i-1] != '\\') {
                    if(singlequote)
                        singlequote = false;
                    else
                        singlequote = true;
                }
                current += '\'';
                break;
            default:
                current += ps[i];
        }
    }
    if(current.length())
        d.push_back(current);
    if(singlequote || doublequote)
        throw "Command parse error";

    return d;
}
#endif


#if 0
ExternalCommand::ExternalCommand(std::string const& cmdstr, char *pth) 
            : Command(cmdstr), path(pth) {
#ifndef NDEBUG
    //        std::cout << cmdstr << std::endl;
#endif
    foreground = true;

    std::deque<std::string> d = split(cmdstr);

    file = new char[d[0].length()+1];
    std::strcpy(file, d[0].c_str());

    argc = d.size();

    if(d.back() == "&") {
        foreground = false;
        --argc;
    }

    argv = new char*[argc+1];
    argv[argc] = NULL;

    for(size_t i = 0; i < argc; ++i) {
        argv[i] = new char[d[i].size()+1];
        std::strcpy(argv[i], d[i].c_str());
    }
}
#endif

int ExternalCommand::execute() const {
    pid_t pid = fork();
    if(pid > 0) {
//        Process::addProcess(pid, path, options.we_wordv);
        if(foreground)
            //                wait(NULL);
            waitpid(pid, NULL, 0);
        return 0;
    } else if(pid == 0) {
        if(execv(path, options.we_wordv) == -1) {
            throw CommandExecuteException(errno, path);
        }
    } else {
        throw std::string(std::strerror(errno));
    }
    return 0;
}
