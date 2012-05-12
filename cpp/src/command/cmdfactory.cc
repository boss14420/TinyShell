/*
 * =====================================================================================
 *
 *       Filename:  cmdfactory.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/18/2012 08:29:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "command"

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif
#include <cstring>
#include <cctype>
#include <cstdlib>

#include <wordexp.h>
#include <sys/stat.h>

#include <utility>
#include <fstream>

#ifdef _ENABLE_REDIRECT_
std::string nextWord();

Command* CommandFactory::parseCommand(std::string const& cmdstr) {

    // remove leading white space
    auto si = cmdstr.begin();
    while((si != cmdstr.end()) && std::isspace(*si)) ++si;

    // remove ending white space
    auto eri = cmdstr.rbegin();
    while((eri != cmdstr.rend()) && std::isspace(*eri)) ++eri;
    
    // Check for background sign '&'
    bool foreground = true;
    if(eri != cmdstr.rend() && *eri == '&') { /* cmdstr ended by '&' */
        /*  and is not a redirect */
        if((eri+1) == cmdstr.rend() 
                || (*(eri+1) != '&' && *(eri+1) != '>' && *(eri+1) != '<')) {
            foreground = false;
            ++eri;
        }
    }
    auto ei = (eri-1).base();

    // 
    std::string formattedcmd = "";

    std::string word;
    std::deque<Redirect> rdlist;
    Redirect rd;

    while(si != ei) {
        while(std::isspace(*si)) {
            ++si;
            continue;
        }
        word = "";
        word = nextWord();
        if(word.size() && word[0] != '\'' && word[0] != '\"'
              && ( (word.find_first_of('<') != std::string::npos) 
                || (word.find_first_of('>') != std::string::npos) )) {
            int number = 0;
            auto wi = word.begin();
            Redirect rd;
            for(; wi != word.end() && std::isdigit(*wi); ++wi)
                number = number*10 + (*wi-'0');

            if(*wi == '>') {
                rd.fd = number ? number : STDOUT_FILENO;
                rd.type = Redirect::RD_OUT;
                if(++wi != word.end() && (*wi == '>'))
                    rd.type = Redirect::RD_APPEND;
                else
                    --wi;
            } else if(*wi == '<') {
                rd.fd = number ? number : STDIN_FILENO;
                rd.type = Redirect::RD_IN;
                if(++wi != word.end() && (*wi == '>'))
                    rd.type |= Redirect::RD_OUT;
                else --wi;
            } else {
                // not a redirect
                formattedcmd += word + ' ';
                continue;
            }

            auto si_dest = wi;
            if(++wi != word.end() && *wi == '&') {
                if(++wi != word.end() && std::isdigit(*wi)) {
                    number = 0;
                    for(; wi != word.end() && std::isdigit(*wi); ++wi)
                        number = number*10 + (*wi - '0');
                    if(wi != word.end() && *wi == '-') {
                        if(std::isdigit(*(wi-1)))
                            rd.type |= Redirect::RD_MOVE;
                        else
                            rd.type |= Redirect::RD_CLOSE;
                    }
                    
                    if(++wi == word.end()) {
                        rd.dest_fd = number;
                        rdlist.push_back(std::move(rd));
                    } else {
                        throw RedirectException();
                    }
                } else
                    throw RedirectException();
            } else {
                word = nextWord();
                if(word.size() && word[0] != '&')
                    rd.filename = word;
                else
                    throw RedirectException();
            }
        }


/*         switch(*si) {
 *             case ' ':
 *             case '\t':
 *                 if((si+1) != ei && std::isspace(*(si+1)))
 *                     ++si;
 *                 else {
 *                     word = nextWord();
 *                     if(word.size() && word[0] != '\'' && word[0] 
 *                     }
 * 
 *                     formattedcmd += ' ' + word;
 *                 }
 *                 break;
 * 
 *             case '<':
 *                 if(si != cmdstr.begin() && std::isdigit(*(si-1)))
 *                     rd.fd = *(si-1) - '0';
 *                 else rd.fd = STDIN_FILENO;
 *                 rd.type = Redirect::RD_IN;
 *                 while(si != ei && std::isspace(*si)) ++si;
 *                 word = "";
 *                 word = nextWord();
 * 
 *             case '>':
 *                 if(si != cmdstr.begin() && std::isdigit(*(si-1)))
 *                     rd.fd = *(si-1) - '0';
 *                 else rd.fd = STDOUT_FILENO;
 */
       
    }

    // word expansion
    wordexp_t options;
    int ret;
    switch(ret = wordexp(cmdstr.c_str(), &options, WRDE_SHOWERR)) {
        case 0:                 // Success
            break;
        case WRDE_NOSPACE:
            wordfree(&options);
        default:
            throw CommandParseException(ret);
    }

    // create command object
    std::string first_word = options.we_wordv[0];
    if(BuiltInCommand::hasCommand(first_word)) {
        switch(BuiltInCommand::getCommand(first_word)) {
            case BuiltInCommand::HISTORY:
                return new HistoryCommand();
                wordfree(&options);
                break;
            case BuiltInCommand::KILL:
                wordfree(&options);
                break;
            case BuiltInCommand::CHDIR:
                return new ChdirCommand(&options);
                break;
            case BuiltInCommand::HELP:
                wordfree(&options);
                break;
        }
    } else {
        char * path = pathLookup(first_word);
        if(path) {
            return new ExternalCommand(path, &options, foreground);
        } else {
            wordfree(&options);
            throw BadCommandException(first_word);
        }
    }
    return NULL;

}

#else
Command* CommandFactory::parseCommand(std::string cmdstr) const {

    // find first non-quote comment sign (#)
    size_t const npos = std::string::npos;
    size_t new_size = 0;
    size_t singlequote = npos, doublequote = npos;
    for(new_size = 0; new_size < cmdstr.size(); ++new_size) {
        if( (cmdstr[new_size] == '#') && 
                (singlequote == npos) && (doublequote == npos) )
            break;

        if(cmdstr[new_size] == '\'') {
            if(singlequote != npos) {
                if(doublequote > singlequote)
                    doublequote = npos;
                singlequote = npos;
            } else
                singlequote = new_size;
        } else if(cmdstr[new_size] == '"') {
            if(doublequote != npos) {
                if(singlequote > doublequote)
                    singlequote = npos;
                doublequote = npos;
            } else
                doublequote = new_size;
        }
    }
    // remove comment
    cmdstr.resize(new_size);

    // Check for background sign '&'
    bool foreground = true;
    auto si = cmdstr.rbegin();
    new_size = cmdstr.size();
    for(; si != cmdstr.rend() && 
            (*si == ' ' || *si == '\t'); ++si, --new_size);
    if(*si == '&') {
        foreground = false;
//        cmdstr.erase(si.base());
        --new_size;
    }
    cmdstr.resize(new_size);

    // word expansion
    wordexp_t options;
    int ret;
    switch(ret = wordexp(cmdstr.c_str(), &options, WRDE_SHOWERR)) {
        case 0:                 // Success
            break;
        case WRDE_NOSPACE:
            wordfree(&options);
        default:
            throw CommandParseException(ret);
    }

    // create command object

    if(!options.we_wordc)
        throw NullCommand();

    std::string first_word = options.we_wordv[0];
    if(BuiltInCommand::hasCommand(first_word)) {
        switch(BuiltInCommand::getCommand(first_word)) {
            case BuiltInCommand::HISTORY:
                wordfree(&options);
                return new HistoryCommand();
                break;
            case BuiltInCommand::KILL:
                wordfree(&options);
                break;
            case BuiltInCommand::CHDIR:
                return new ChdirCommand(&options);
                break;
            case BuiltInCommand::HELP:
                wordfree(&options);
                break;
            case BuiltInCommand::JOBS:
                wordfree(&options);
                return new JobsCommand;
            case BuiltInCommand::FG:
                return new ForegroundCommand(&options, cmdstr);
            case BuiltInCommand::BG:
                return new BackgroundCommand(&options, cmdstr);
        }
    } else {
        char * path = pathLookup(first_word);
        if(path) {
            return new ExternalCommand(path, &options, foreground, cmdstr);
        } else {
            wordfree(&options);
            throw BadCommandException(first_word);
        }
    }
    return NULL;

}
#endif

#if 0
Command* CommandFactory::parseCommand(std::string const& cmdstr) {
    char const *ps = cmdstr.c_str();
    std::string first = "";
    bool doublequote = false, singlequote = false;
    
    while(*ps == ' ' || *ps == '\t') ++ps;

    while(*ps) {
        switch(*ps) {
            case ' ':
                if(!doublequote && !singlequote)
                    goto done;
                else
                    first += ' ';
                break;
            case '\"':
                if(singlequote)
                    first += '\"';
                else if(doublequote)
                    doublequote = false;
                else
                    doublequote = true;
                break;
            case '\'':
                if(doublequote)
                    first += '\'';
                else if(singlequote)
                    singlequote = false;
                else
                    singlequote = true;
                break;
            default:
                first += *ps;
        }
        ++ps;
    }
done:
    if(BuiltInCommand::hasCommand(first)) {
        switch(BuiltInCommand::getCommand(first)) {
            case BuiltInCommand::HISTORY:
                return new HistoryCommand();
                break;
            case BuiltInCommand::KILL:
                break;
            case BuiltInCommand::CHDIR:
                return new ChdirCommand(cmdstr);
                break;
            case BuiltInCommand::HELP:
                break;
        }
    } else {
        char * path = pathLookup(first);
        if(path) {
            return new ExternalCommand(cmdstr, path);
        } else 
            return new InvalidCommand(cmdstr, first);
    }
    return NULL;
}
#endif

char * CommandFactory::pathLookup(std::string const& file) const{
    char* filepath = NULL;
    struct stat st;
    
    /*  'file' is not a path name */
    if(file.find_first_of('/') == std::string::npos) {
        char *PATH = strdup(getenv("PATH"));
        if(PATH) {
            filepath = new char[std::strlen(PATH)+file.length()+2];

            char * path = std::strtok(PATH, ":");
            for(; path; path = std::strtok(NULL, ":")){
                std::strcpy(filepath, path);
                std::strcat(filepath, "/");
                std::strcat(filepath, file.c_str());
                if(!stat(filepath, &st)) {
                    std::free(PATH);
                    return filepath;
                }
            }
            delete[] filepath;
        }
        std::free(PATH);
#if 0
        do {
            if(*PATH == ':')
                ++PATH;
            size_t pathlength  = (strchrnul(PATH, ':') - PATH);
            std::strncpy(path, PATH, pathlength);
            path[pathlength] = '/';
            path[pathlength+1] = '\0';
            std::strcat(path, file.c_str());

            if(!stat(path, &st))
                return path;

            PATH += pathlength;
        } while(*PATH);
#endif

        return NULL;
    } else if(!stat(file.c_str(), &st)) {
        filepath = new char[file.length()+1];
        std::strcpy(filepath, file.c_str());
        return filepath;
    }
    return NULL;
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  execute_command
 *  Description:  
 * =====================================================================================
 */
int CommandFactory::execute_command(std::string cmdstr) const {
    Command *cmd = NULL;

    try {
        cmd = parseCommand(cmdstr);
        cmd->execute();
    } catch(CommandExecuteException &ex) {
        std::fprintf(stderr, "%s\n", ex.what());
        std::exit(EXIT_FAILURE); // exit child process
    } catch(NullCommand ) {
    } catch (std::exception &ex) {
        std::fprintf(stderr, "%s\n", ex.what());
        delete cmd;
        return EXIT_FAILURE;
    }
    delete cmd;
    return EXIT_SUCCESS;
}
/* ---------- end of function execute_command ------------------------ */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  execute_script
 *  Description:
 * =====================================================================================
 */

int CommandFactory::execute_script (char const* filename ) const{
//    std::ifstream ifs;
//    try {
//        ifs.open(filename);
//        ifs.exceptions(std::ifstream::badbit | std::ifstream::failbit
//                        | std::ifstream::eofbit);
//    } catch(std::exception& ex) {
//        std::cerr << ex.what() << '\n';
//        return EXIT_FAILURE;
//    }

    std::ifstream ifs(filename);
    if(!ifs) {
        std::perror(filename);
        return EXIT_FAILURE;
    }

    std::string cmdstr;
    while(!ifs.eof()) {
        std::getline(ifs, cmdstr);
        execute_command(cmdstr);
    }
    ifs.close();
    return EXIT_SUCCESS;
}
/* -----  end of function execute_script  ----- */
