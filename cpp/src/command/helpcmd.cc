/*
 * =====================================================================================
 *
 *       Filename:  helpcmd.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/12/2012 08:39:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "helpcmd.h"
#include "command"

#include <sys/utsname.h>
#include <iostream>

static struct utsname my_uname;
static int ret = uname(&my_uname);

HelpCommand::HelpCommand(wordexp_t *o) : options(*o) {
    if(options.we_wordc == 1)
        type = BuiltInCommand::OTHER;
    else if (2 == options.we_wordc) {
        if(BuiltInCommand::hasCommand(options.we_wordv[1]))
            type = BuiltInCommand::getCommand(options.we_wordv[1]);
        else
            throw BuiltInCommandException(std::string(options.we_wordv[1])+": no such command");
    } else
        throw BuiltInCommandException("Invalid arguments");
}

HelpCommand::~HelpCommand() {
    wordfree(&options);
}

int HelpCommand::execute() const {
    (void) ret;

    switch(type) {
        case BuiltInCommand::HELP:
            std::cout << HelpCommand::helpString();
            break;
        case BuiltInCommand::CHDIR:
            std::cout << ChdirCommand::helpString();
            break;
        case BuiltInCommand::HISTORY:
            std::cout << HistoryCommand::helpString();
            break;
        case BuiltInCommand::JOBS:
            std::cout << JobsCommand::helpString();
            break;
        case BuiltInCommand::FG:
            std::cout << ForegroundCommand::helpString();
            break;
        case BuiltInCommand::BG:
            std::cout << BackgroundCommand::helpString();
            break;
        default:
            std::cout << "Tiny Shell, version " << GET_VERSION
                << " (" << my_uname.sysname << ' ' << my_uname.release
                << ' ' << my_uname.machine << ") \nAuthor: BOSS14420 (boss14420@gmail.com)\n\n"
                << "help   \t" << HelpCommand::descriptionString() << '\n'
                << "cd     \t" << ChdirCommand::descriptionString() << '\n'
                << "history\t" << HistoryCommand::descriptionString() << '\n'
                << "jobs   \t" << JobsCommand::descriptionString() << '\n'
                << "fg     \t" << ForegroundCommand::descriptionString() << '\n'
                << "bg     \t" << BackgroundCommand::descriptionString() << '\n';
    }
    std::cout << '\n';

    return 0;
}

