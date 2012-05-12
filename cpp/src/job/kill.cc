/*
 * =====================================================================================
 *
 *       Filename:  kill.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/12/2012 06:29:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "kill.h"
#include "shell.h"

KillCommand::KillCommand(char *p, wordexp_t *o, bool f, std::string s)
    : ExternalCommand(p,o,f,s)
{
    // substite job id by job's pgid
    for(size_t i = 1; i < o->we_wordc; ++i) {
        if(o->we_wordv[i][0] == '%') {
            auto j = Shell::findJobById(std::atoi(o->we_wordv[i]+1));
            if(j != Shell::end()) {
                char *pidstr = (char*) std::malloc(7);
                std::sprintf(pidstr, "%d", j->pgid);
                std::free(o->we_wordv[i]);
                o->we_wordv[i] = pidstr;
            }
        }
    }
}
