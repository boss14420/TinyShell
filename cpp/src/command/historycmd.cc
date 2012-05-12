/*
 * =====================================================================================
 *
 *       Filename:  historycmd.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/18/2012 07:17:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "historycmd.h"
#include <cstdio>
#include <readline/history.h>

//std::deque<shared_ptr<Command> > HistoryCommand::history;

int HistoryCommand::execute() const {
//    size_t n = history.size();
//    for(size_t i = 0; i < n; ++i) {
//        std::cout << i + 1 << ". " <<  history[i]->command() << "\n";
//    }
//    std::cout.flush();
    
    HIST_ENTRY **lst_hist = history_list();
    if(lst_hist) {
        HIST_ENTRY *hist = lst_hist[0];
        size_t n = 0;
        for(; hist; hist = lst_hist[++n]) {
            std::printf("%3lu. %s\n", n+1, hist->line);
        }
        std::fflush(stdout);
    }
    return 0;
}
