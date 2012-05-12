/*
 * =====================================================================================
 *
 *       Filename:  jobcmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/11/2012 09:02:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _JOBSCMD_H_
#define _JOBSCMD_H_

#include "../command/builtin.h"
#include "shell.h"
#include <iostream>

class JobsCommand : public BuiltInCommand {
public:
    JobsCommand() {}

    virtual int execute() const {
        for(auto j : Shell::jobList) {
            std::cout << j << '\n';
        }
        return 0;
    }

    virtual std::string command() const { return "jobs"; }
};

#endif
