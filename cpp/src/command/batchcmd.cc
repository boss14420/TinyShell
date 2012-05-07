/*
 * =====================================================================================
 *
 *       Filename:  batchcmd.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/29/2012 11:55:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "batchcmd.h"
#include "cmdfactory.h"

#include <fstream>
#include <cstdio>

int BatchCommand::execute() const {
    CommandFactory cf;

    std::ifstream ifs(batchfile);
    if(!ifs)
        throw BatchCommandException();

    std::string cmdstr;
    while(!ifs.eof()) {
        std::getline(ifs, cmdstr);

        if(cmdstr.length()) {
            try {
                Command *command = cf.parseCommand(cmdstr);
                command->execute();
                delete command;
            } catch (CommandExecuteException &ex) {
                std::fprintf(stderr, "%s\n", ex.what());
                return EXIT_FAILURE; // exit child process
            } catch (std::exception &ex) {
                std::fprintf(stderr, "%s\n", ex.what());
                delete command;
            }
        }
    }

    return 0;
}
