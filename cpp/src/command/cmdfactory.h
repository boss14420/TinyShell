/*
 * =====================================================================================
 *
 *       Filename:  cmdfactory.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/18/2012 08:26:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _CMDFACTORY_H_
#define _CMDFACTORY_H_

#include "command.h"
#include <wordexp.h>

class CommandParseException : public std::exception {
    int errcode;
public:
    CommandParseException(int e) : errcode(e) {}
    virtual const char* what() const throw() {
        switch(errcode) {
            case WRDE_BADCHAR:
                return "bad character";
            case WRDE_BADVAL:
                return "undefined shell variable";
            case WRDE_CMDSUB:
                return "command substitution occurred";
            case WRDE_NOSPACE:
                return "out of memory";
            case WRDE_SYNTAX:
                return "syntax error";
            default:
                return "parse error";
        }
    }
};

class BadCommandException : public std::exception {
    std::string errstr;
public:
    BadCommandException(std::string const& file) : errstr("bad command : " + file) {}
    virtual ~BadCommandException() throw() {}
    virtual const char*what() const throw() {
        return errstr.c_str();
    }
};

class RedirectException : public std::exception {

};

class NullCommand : public std::exception {
};

class CommandFactory {
    char* pathLookup(std::string const& file) const;
public:
    Command* parseCommand(std::string cmdstr) const;
    int execute_command(std::string cmdstr) const;
    int execute_script(char const* filename) const;
};

#endif

/* vim: set filetype=cpp: */
