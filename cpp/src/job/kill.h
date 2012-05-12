/*
 * =====================================================================================
 *
 *       Filename:  kill.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/12/2012 06:24:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _KILL_H_
#define _KILL_H_

#include "../command/extcmd.h"

class KillCommand : public ExternalCommand {
public:
    KillCommand() : ExternalCommand() {}
    KillCommand(char *p, wordexp_t *o, bool f, std::string s);

    static std::string helpString() {
        return " kill [-s sigspec | -n signum | -sigspec] pid | jobspec ... or kill -l [sigspec]\n"
    "Send a signal to a job.\n\n"
    
    "Send the processes identified by PID or JOBSPEC the signal named by"
    "SIGSPEC or SIGNUM.  If neither SIGSPEC nor SIGNUM is present, then"
    "SIGTERM is assumed.\n\n"
    
    "Options:"
    "  -s sig    SIG is a signal name\n"
    "  -n sig    SIG is a signal number\n"
    "  -l        list the signal names; if arguments follow '-l' they are"
    "    assumed to be signal numbers for which names should be listed\n\n"
    
    "Kill is a shell builtin for two reasons: it allows job IDs to be used"
    "instead of process IDs, and allows processes to be killed if the limit"
    "on processes that you can create is reached.\n\n"
    
    "Exit Status:\n"
    "Returns success unless an invalid option is given or an error occurs.\n";
    }

    static std::string descriptionString() {
        return "Send a signal to a job/process.";
    }
};

#endif
