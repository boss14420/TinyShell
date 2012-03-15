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
#include <sys/stat.h>
#include <cstdlib>

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif
#include <cstring>
#include <wordexp.h>

Command* CommandFactory::parseCommand(std::string cmdstr) {

    // Check for background sign '&'
    bool foreground = true;
    auto si = cmdstr.rbegin();
    for(; si != cmdstr.rend() && 
            (*si == ' ' || *si == '\t'); ++si);
    if(*si == '&') {
        foreground = false;
        cmdstr.erase(si.base());
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

char * CommandFactory::pathLookup(std::string const& file) {
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
