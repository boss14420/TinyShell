/*
 * =====================================================================================
 *
 *       Filename:  chdircmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/01/2012 03:11:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _CHDIR_COMMAND_
#define _CHDIR_COMMAND_

#include "builtin.h"
#include <unistd.h>
#include <wordexp.h>
#include <cstdlib>
#include <cstring>

class ChdirCommand : public BuiltInCommand {
        wordexp_t options;
        static char old_dir[1024];
    public:
        ChdirCommand(wordexp_t *o) : options(*o) { }
        virtual int execute() const;

        virtual ~ChdirCommand() {
            wordfree(&options);
        }
};

//char ChdirCommand::old_dir[1024] = {'\0'};

#endif

/* vim: set filetype=cpp: */
