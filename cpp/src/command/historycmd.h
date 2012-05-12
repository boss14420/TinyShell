/*
 * =====================================================================================
 *
 *       Filename:  historycmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/18/2012 07:13:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _HISTORYCMD_H_
#define _HISTORYCMD_H_

#include "command.h"
#include "builtin.h"
#include <deque>
#include <iostream>
#include <memory>
using std::shared_ptr;

class HistoryCommand : public BuiltInCommand {
//    static std::deque<shared_ptr<Command> > history;

public:
    HistoryCommand() {}
    virtual int execute() const;
    virtual std::string command() const { return "history"; }

    static std::string helpString() {
        return "history"
            "\nDisplay the history list.\n";
    }

    static std::string descriptionString() {
        return "Display the history list.";
    }
};
#endif

/* vim: set filetype=cpp: */
