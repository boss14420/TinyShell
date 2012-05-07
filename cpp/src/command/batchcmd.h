/*
 * =====================================================================================
 *
 *       Filename:  batchcmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/29/2012 11:51:41 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _BATCH_CMD_
#define _BATCH_CMD_

#include "builtin.h"
#include <string>

class BatchCommandException : public BuildInCommandException {};

class BatchCommand : public BuiltInCommand {
    std::string batchfile;
public:
    BatchCommand(std::string const& bf) : batchfile(bf) {}
    int execute() const;
};

#endif
