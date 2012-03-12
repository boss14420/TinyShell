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
    static std::deque<shared_ptr<Command> > history;

public:
    HistoryCommand() {}
    virtual int execute() const;
    virtual std::string command() const { return "history"; }

/*     static void historyAdd(shared_ptr<Command> const& cmd) {
 *         history.push_back(cmd);
 *     }
 * 
 *     typedef std::deque<shared_ptr<Command>>::const_iterator const_iterator;
 * 
 *     inline static const_iterator cend() {
 *         return history.cend();
 *     }
 * 
 *     inline static const_iterator cbegin() {
 *         return history.cbegin();
 *     }
 */

};
#endif
