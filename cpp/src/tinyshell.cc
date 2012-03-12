/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/17/2012 08:46:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include "command/command"

#ifdef EXECUTABLE_COMPLETE
    #include <boost/filesystem.hpp>
    #include <iterator>
#endif

#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

using std::shared_ptr;

void handle_signal(int signo);

char * get_shell_promt(char * src);
int start_shell();

void initialize_auto_completion();
char **tinyshel_completion PARAMS((const char *, int, int));
char *command_generator PARAMS((const char *, int));

int main() {

//    std::cout.sync_with_stdio(false);
//    std::cin.sync_with_stdio(false);
//    signal(SIGINT, SIG_IGN);
    signal(SIGINT, handle_signal);
    

    initialize_auto_completion();
//    std::atexit([] () { clear_history(); });

    return start_shell();

}

void initialize_auto_completion() {
    rl_bind_key('\t', rl_complete);

#ifdef EXECUTABLE_COMPLETE
    // Tell the completer that we want a crack first.
    rl_attempted_completion_function = tinyshel_completion;
#endif
}

#ifdef EXECUTABLE_COMPLETE

/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */
char** tinyshel_completion(char const * text, int start, int end) {
    char **matches = NULL;

    /* If this word is at the start of the line, then it is a command
       to complete.  Otherwise it is the name of a file in the current
       directory. */
    if(0 == start)
        matches = rl_completion_matches(text, command_generator);
    
    return matches;
}

char * command_generator(char const * text, int state) {
    static int len;
    static char PATH[1024];
    static boost::filesystem::directory_iterator di, di_end;
    boost::filesystem::path p;

    /* If this is a new word to complete, initialize now. This includes
     * saving the length of TEXT for efficiency, and initializing the
     * variable to 0. */
    if(!state) {
        len = std::strlen(text);
        std::strcpy(PATH, std::getenv("PATH"));
//        p.assign(std::strtok(PATH, ":"));
        p = std::strtok(PATH, ":");
        try {
            di = boost::filesystem::directory_iterator(p);
        } catch(boost::filesystem3::filesystem_error& fe) {}
    }

    /* Return the next name which partially matches from the command list. */

    while(true) {
        if(di == di_end) {
            char *path = std::strtok(NULL, ":");
            if(!path)
                break;
//            p.assign(path);
            p = path;
            try {
                di = boost::filesystem::directory_iterator(p);
            } catch(boost::filesystem3::filesystem_error& fe) {
                continue;
            }
        }

        try {
            for(; di != di_end; ++di) {
                boost::filesystem::file_status status = di->status();
                if(boost::filesystem::is_regular_file(status) ||
                        ( boost::filesystem::is_symlink(status) &&                // is a regular file
                          boost::filesystem::is_regular_file(                     // or link to a regular file
                              boost::filesystem::read_symlink(di->path())
                              ))) {
                    char const * cpath = di->path().filename().c_str();
                    if(!std::strncmp(cpath, text, len)) {      // partially matches
//                    if(is_executable(di))
                        char *res = (char*) std::malloc(std::strlen(cpath)+1);
                        return std::strcpy(res, cpath);
                    }
                }
            }
        } catch(boost::filesystem3::filesystem_error & fe) {
            continue;
        }
    }
    return NULL;
}
#endif

void handle_signal(int signo) {
//    std::printf("\n");
//    std::fflush(stdout);
//    start_shell();
    clear_history();
    std::exit(EXIT_SUCCESS);
}

char * get_shell_promt(char * src) {
    char *cwd;
    std::sprintf(src, "%s:%s", std::getenv("USER"), cwd = getcwd(NULL, 1024));
    if(getuid()) // Normal user
        std::strcat(src, " $ ");
    else         // root user
        std::strcat(src, " # ");

    free(cwd);
    return src;    
}

int start_shell() {
//    static int count = 0;
//
//    std::fprintf(stderr, "start shell % d\n", ++count);

    std::string strcmd;
    shared_ptr<Command> cmd;
    CommandFactory cf;

    char *input, shell_promt[1024];

    while(true) {
        input = readline(get_shell_promt(shell_promt));

        if(!input) {  // ^D dectected
            std::printf("\n");
            break;
        }

        if(!strcmp(input, "quit")) { // exit shell
            std::free(input);
            break;
        }

        if(std::strlen(input)) { // non blank command
            strcmd.assign(input);

            try {
                cmd.reset(cf.parseCommand(strcmd));

                // Execute command
                cmd->execute();
            } catch (CommandExecuteException e) {
                std::fprintf(stderr, "%s\n", e.what());
                return EXIT_FAILURE; // exit child process
            } catch (std::exception &ex) {
                std::fprintf(stderr, "%s\n", ex.what());
            }

//            HistoryCommand::historyAdd(cmd); // add history for history command
            add_history(input);              // add history for up/down key

        }
        std::free(input);
    }


//    std::fprintf(stderr, "exit shell %d \n", --count);
    clear_history();
    return EXIT_SUCCESS;
}
