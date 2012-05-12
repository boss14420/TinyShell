/*
 * =====================================================================================
 *
 *       Filename:  shell.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/11/2012 05:04:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _SHELL_H_
#define _SHELL_H_

#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include <list>
#include <string>

class JobsCommand;
class ForegroundCommand;
class BackgroundCommand;

class Shell {
public:
    static void init();
    static void init_child_process(pid_t pid, bool foreground);
    static void restore_shell();
    static void report_finished_job();
    
    struct Job {
        int id;
        pid_t pgid;
        enum Status { RUNNING, STOPPED, FINISH } status;
        struct termios tmode;
        bool foreground;
        std::string cmdstr;
    };

    typedef std::list<Job>::iterator job_iter;

    static job_iter addJob(pid_t, std::string);
    static job_iter findJobById(int);
    static job_iter findJobByPid(pid_t);
    static void removeJob(job_iter);
    static void moveToForeground(job_iter);
    static void moveToBackground(job_iter);

    static job_iter end();


    friend class JobsCommand;
    friend std::ostream& operator<< (std::ostream&, Job const&);
//    friend class ForegroundCommand;
//    friend class BackgroundCommand;
private:
    static void sigchld_handler(int);


    static pid_t shell_pgid;
    static struct termios shell_tmodes;
    static int shell_terminal;
    static int shell_is_interactive;

    static std::list<Job> jobList;
    static int jobIndex;
    static job_iter first_default;
    static job_iter second_default;
};

//std::ostream& operator<< (std::ostream& os, Shell::Job const& j);

#endif
