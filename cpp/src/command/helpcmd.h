/*
 * =====================================================================================
 *
 *       Filename:  helpcmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/12/2012 08:36:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _HELP_COMMAND_
#define _HELP_COMMAND_

#include "builtin.h"
#include <wordexp.h>

class HelpCommand : public BuiltInCommand {
    wordexp_t options;
    BuiltInCommand::CommandType type;

public:
    HelpCommand(wordexp_t *o);
    virtual ~HelpCommand();

    virtual int execute() const;

    static std::string helpString() {
        return "help [COMMAND]"
            "\nDisplay information about builtin command COMMAND"
            "\n\nIf COMMAND is not specified, display description of all builtin commands\n";
    }

    static std::string descriptionString() {
        return "Display information about builtin commands.";
    }
};

#endif
