/*
 * =====================================================================================
 *
 *       Filename:  bgcmd.h
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

#ifndef _BGCMD_H_
#define _BGCMD_H_

#include "../command/builtin.h"
#include <wordexp.h>
#include <vector>

class BackgroundCommand : public BuiltInCommand {
public:
    BackgroundCommand(wordexp_t *o,  std::string s);
    virtual ~BackgroundCommand();
    virtual int execute() const;

    static std::string helpString() {
        return "bg [JOB_ID ...]"
            "\nMove jobs JOB_ID ... to the background."
            "\n\nIf JOB_ID is not specified, move the default job "
            "(job with '+' when list by \"jobs\" command) to the background.\n";
    }

    static std::string descriptionString() {
        return "Move jobs list to the background";
    }
private:
    wordexp_t options;
    std::string cmdstr;
    std::vector<int> jobidList;
};

#endif
