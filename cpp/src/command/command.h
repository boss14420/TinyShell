/*
 * =====================================================================================
 *
 *       Filename:  command.h
 *
 *    Description:  Command classes
 *
 *        Version:  1.0
 *        Created:  02/17/2012 07:53:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _COMMAND_H_
#define _COMMAND_H_

//#include "command"
#include <string>

class Command {
//    std::string cmdstr;
public:
//    Command(std::string s) : cmdstr(s) {}
    Command() {}

    virtual ~Command() {}

//    virtual std::string command() const { return cmdstr; }
    int virtual execute() const = 0;
};

struct Redirect {
    static const int RD_OUT = 1;
    static const int RD_APPEND = 2;
    static const int RD_IN = 4;
    static const int RD_CLOSE = 8;
    static const int RD_MOVE = 16;

    int type;
    int fd;
//    int dest_fd;
    union {
        int dest_fd;
        std::string filename;
    };

};

#endif

/* vim: set filetype=cpp: */
