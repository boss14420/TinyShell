/*
 * =====================================================================================
 *
 *       Filename:  extcmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/18/2012 03:16:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _EXTCMD_H_
#define _EXTCMD_H_

#include "command.h"


#include <errno.h>
#include <wordexp.h>

#include <string>
#include <deque>
#include <cstring>
#include <stdexcept>

#ifndef NDEBUG
    #include <iostream>
#endif

class CommandExecuteException : public std::exception {
    std::string strerr;
public:
    CommandExecuteException(int error, char const *file) 
        : strerr(std::string(file) + " : " + strerror(error)) {}
    virtual const char* what() const throw() { return strerr.c_str(); }
    virtual ~CommandExecuteException() throw() {}
};

class ExternalCommand : public Command {
    char *path;
    wordexp_t options;
    bool foreground;

//    static std::deque<std::string> split(std::string str);
public:
    ExternalCommand() : path(0) {}
    ExternalCommand(char * p, wordexp_t * o, bool f) 
        : path(p), options(*o), foreground(f) {}

    virtual ~ExternalCommand() {
        delete[] path;
        wordfree(&options);
    }

    virtual int execute() const;

};

#endif
