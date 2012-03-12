#include "chdircmd.h"

char ChdirCommand::old_dir[1024] = {'\0'};

int ChdirCommand::execute() const {
    char *new_dir = options.we_wordv[1];

    bool needfree = false;
    if(!new_dir)
        new_dir = std::getenv("HOME");
    else if(!std::strcmp(new_dir, "-")) {
        if(old_dir[0]) {
            new_dir = new char[std::strlen(old_dir)+1];
            std::strcpy(new_dir, old_dir);
            needfree = true;
        } else
            return -1;
    }

    getcwd(old_dir, 1024);

    int r = chdir(new_dir);
    if(-1 == r) {
        std::string errstr(new_dir);
        if(needfree)
            delete[] new_dir;
        throw BuildInCommandException(errstr + " : " + strerror(errno));
    }
    if(needfree)
        delete[] new_dir;

    return r;
}
