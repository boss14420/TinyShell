/*
 * =====================================================================================
 *
 *       Filename:  fgcmd.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/11/2012 09:26:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "shell.h"
#include "fgcmd.h"

#ifndef DNDEBUG
    #include <iostream>
#endif

ForegroundCommand::ForegroundCommand(wordexp_t *o, std::string s)
    : options(*o), cmdstr(s)
{
    if(options.we_wordc == 1)
        jobid = -1;
    else if(options.we_wordc == 2)
        jobid = std::atoi(options.we_wordv[1]);
    else 
        throw BuildInCommandException("fg: Invalid arguments");
}

ForegroundCommand::~ForegroundCommand() {
    wordfree(&options);
}

int ForegroundCommand::execute() const {
    Shell::job_iter j = Shell::findJobById(jobid);
    if(j == Shell::end())
        throw BuildInCommandException(cmdstr + ": no such job");

    Shell::moveToForeground(j);
    return 0;
}
