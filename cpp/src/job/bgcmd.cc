/*
 * =====================================================================================
 *
 *       Filename:  bgcmd.cc
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
#include "bgcmd.h"

#include <sstream>

#ifndef DNDEBUG
    #include <iostream>
#endif

BackgroundCommand::BackgroundCommand(wordexp_t *o, std::string s)
    : options(*o), cmdstr(s)
{
    if(options.we_wordc == 1)
        jobidList.assign( { -1 } );
    else {
        jobidList.resize(options.we_wordc-1);
        for(size_t i = 1; i < options.we_wordc; ++i)
            jobidList[i] = std::atoi(options.we_wordv[i]);
    }
}

BackgroundCommand::~BackgroundCommand() {
    wordfree(&options);
}

int BackgroundCommand::execute() const {
    std::vector<Shell::job_iter> iterList (jobidList.size());
    for(size_t i = 0; i < jobidList.size(); ++i) {
        Shell::job_iter j = Shell::findJobById(jobidList[i]);
        if( j == Shell::end() ) {
            std::stringstream ss;
            if(jobidList[i] != -1) 
                ss << "bg: [" << jobidList[i] << "]: no such job";
            else 
                ss << "bg: no such job";

            throw BuildInCommandException(ss.str());
        }
        else iterList[i] = j;
    }

    for(auto j : iterList)
        Shell::moveToBackground(j);

    return 0;
}
