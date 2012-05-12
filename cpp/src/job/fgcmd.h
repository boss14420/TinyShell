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
private:
    wordexp_t options;
    std::string cmdstr;
    int jobid;
};

#endif
