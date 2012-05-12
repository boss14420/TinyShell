/*
 * =====================================================================================
 *
 *       Filename:  fgcmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/11/2012 09:13:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _FGCMD_H_
#define _FGCMD_H_

#include "../command/builtin.h"
#include <wordexp.h>

class ForegroundCommand : public BuiltInCommand {
public:
    ForegroundCommand(wordexp_t *o,  std::string s);
    virtual ~ForegroundCommand();
    virtual int execute() const;

    static std::string helpString() {
        return "fg [JOB_ID]"
            "\n Move the job JOB_ID to the foreground."
            "\n\nIf JOB_ID is not specified, move the default job "
            "(job with '+' when list by \"jobs\" command) to the foreground.\n";
    }

    static std::string descriptionString() {
        return "Move job to the foreground.";
    }
private:
    wordexp_t options;
    std::string cmdstr;
    int jobid;
};

#endif
