/*
 * =====================================================================================
 *
 *       Filename:  builtin.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/18/2012 07:11:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _BUILTIN_H_
#define _BUILTIN_H_

#include "command.h"
#include <unordered_map>
#include <utility>

class BuildInCommandException : public std::exception {
    std::string strerr;
public:
//    BuildInCommandException(int error, char const *file) 
//        : strerr(std::string(file) + " : " + strerror(error)) {}
    BuildInCommandException(std::string const& str) : strerr(str) {}
    virtual const char* what() const throw() {
        return strerr.c_str();
    }
    virtual ~BuildInCommandException() throw() {}
};

class BuiltInCommand : public Command {
public:

    BuiltInCommand() {}
//    BuiltInCommand(std::string const& cmdstr) : Command(cmdstr) {}

    virtual ~BuiltInCommand() {}

    virtual int execute() const = 0;

    static bool hasCommand(std::string const& cmd) {
        return builtins.find(cmd) != builtins.end();
    }

    static int getCommand(std::string const& cmd) {
        return builtins[cmd];
    }

    enum CommandType { HISTORY, KILL, CHDIR, HELP, JOBS, FG, BG};
//    static const int HISTORY = 1;
//    static const int KILL = 2;
//    static const int CHDIR = 3;
//    static const int HELP = 4;

private:
    static std::unordered_map<std::string, CommandType> builtins;
};

#endif

/* vim: set filetype=cpp: */
