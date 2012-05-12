/*
 * =====================================================================================
 *
 *       Filename:  builtin.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/18/2012 08:54:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "builtin.h"
#include <utility>

typedef std::pair<std::string, BuiltInCommand::CommandType> PSI;

std::unordered_map<std::string, BuiltInCommand::CommandType> BuiltInCommand::builtins(
        { {"history", HISTORY}, {"cd", CHDIR},
            {"help", HELP}, {"jobs", JOBS}, {"fg", FG}, {"bg", BG} } );

