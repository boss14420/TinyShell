/*
 * =====================================================================================
 *
 *       Filename:  shell.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/11/2012 05:08:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "shell.h"
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <iostream>

pid_t Shell::shell_pgid;
struct termios Shell::shell_tmodes;
int Shell::shell_terminal;
int Shell::shell_is_interactive;

std::list<Shell::Job> Shell::jobList;
int Shell::jobIndex = 0;
Shell::job_iter Shell::first_default = jobList.end();
Shell::job_iter Shell::second_default = jobList.end();

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init
 *  Description:  initialize the shell
 * =====================================================================================
 */
/* static */
void Shell::init() {
    /* See if we are running interactively.  */
    shell_terminal = STDIN_FILENO;
    shell_is_interactive = isatty (shell_terminal);

    if (shell_is_interactive)
    {
        /* Loop until we are in the foreground.  */
        while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
            kill (- shell_pgid, SIGTTIN);

        /* Ignore interactive and job-control signals.  */
        signal (SIGINT, SIG_IGN);
        signal (SIGQUIT, SIG_IGN);
        signal (SIGTSTP, SIG_IGN);
        signal (SIGTTIN, SIG_IGN);
        signal (SIGTTOU, SIG_IGN);
        signal (SIGCHLD, Shell::sigchld_handler);

        /* Put ourselves in our own process group.  */
        shell_pgid = getpid ();
        if (setpgid (shell_pgid, shell_pgid) < 0)
        {
            perror ("Couldn't put the shell in its own process group");
            exit (EXIT_FAILURE);
        }

        /* Grab control of the terminal.  */
        tcsetpgrp (shell_terminal, shell_pgid);

        /* Save default terminal attributes for shell.  */
        tcgetattr (shell_terminal, &shell_tmodes);
    }

}
/* ----- end of function init ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  sigchld_handler
 *  Description:
 * =====================================================================================
 */
void Shell::sigchld_handler ( int ) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if(!WIFSTOPPED(status)) {
        job_iter j = findJobByPid(pid);
        if(j != jobList.end()) {
            if(!j->foreground)
                j->status = Job::FINISH;

            if(j == first_default) {
                first_default = second_default;
                second_default = end();
            }
        }
    }
}
/* -----  end of function sigchld_handler  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  report_finished_job
 *  Description:
 * =====================================================================================
 */
void Shell::report_finished_job (  ) {
    jobList.remove_if([](Job const& j) { 
            if(j.status == Job::FINISH) {
                std::cerr << j << std::endl;
                return true;
            }
            return false;
        } );

    if(jobList.empty())
        jobIndex = 0;
}
/* -----  end of function report_finished_job  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  init_child_process
 *  Description:
 * =====================================================================================
 */
void Shell::init_child_process ( pid_t pid, bool foreground ) {
    if (shell_is_interactive)
    {
        /* Put the process into the process group and give the process group
           the terminal, if appropriate.
           This has to be done both by the shell and in the individual
           child processes because of potential race conditions.  */
        pid = getpid ();
//        if (pgid == 0) pgid = pid;
//        setpgid (pid, pgid);
        setpgid(pid, pid);
        if (foreground)
            tcsetpgrp (shell_terminal, pid);
//            tcsetpgrp (shell_terminal, pgid);

        /* Set the handling for job control signals back to the default.  */
        signal (SIGINT, SIG_DFL);
        signal (SIGQUIT, SIG_DFL);
        signal (SIGTSTP, SIG_DFL);
        signal (SIGTTIN, SIG_DFL);
        signal (SIGTTOU, SIG_DFL);
        signal (SIGCHLD, SIG_DFL);
    }
}
/* -----  end of function init_child_process  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  restore_shell
 *  Description:
 * =====================================================================================
 */
void Shell::restore_shell (  ) {

    /* Put the shell back in the foreground.  */
    tcsetpgrp (shell_terminal, shell_pgid);

    /* Restore the shell's terminal modes.  */
    //            tcgetattr (shell_terminal, &j->tmodes);
    tcsetattr (shell_terminal, TCSADRAIN, &shell_tmodes);


}
/* -----  end of function restore_shell  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  moveToForeground
 *  Description:
 * =====================================================================================
 */
void Shell::moveToForeground (job_iter j) {
    tcsetpgrp(shell_terminal, j->pgid);

    if(j->status == Job::STOPPED) {
        tcsetattr(shell_terminal, TCSADRAIN, &j->tmode);
        if(kill(-j->pgid, SIGCONT) == -1)
            perror("kill (SIGCONT)");
        j->status = Job::RUNNING;
        j->foreground = true;
    }

    int status;
    waitpid(j->pgid, &status, WUNTRACED);
    if(WIFSTOPPED(status)) {
        /* j became default */
        second_default = first_default;
        first_default = j;

        j->status = Job::STOPPED;
        std::cerr << *j << std::endl;

        /* save job's termios */
        tcgetattr(shell_terminal, &j->tmode);

        /* move this job to the end of jobList */
        Job job = *j;
        jobList.erase(j);
        jobList.push_back(job);

        /* restore shell */
        restore_shell(); 

    } else {
        removeJob(j);
    }

    tcsetpgrp(shell_terminal, shell_pgid);
}
/* -----  end of function moveToForeground  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  moveToBackground
 *  Description:
 * =====================================================================================
 */
void Shell::moveToBackground (job_iter j) {
    if(j->status == Job::STOPPED) {
        if(kill(-j->pgid, SIGCONT) == -1)
            perror("kill (SIGCONT)");
        j->status = Job::RUNNING;

        if(j == first_default) {
            first_default = second_default;
            second_default = end();
        } else if(j == second_default)
            second_default = end();
    }
    j->foreground = false;
}
/* -----  end of function moveToBackground  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  addJob
 *  Description:
 * =====================================================================================
 */
Shell::job_iter Shell::addJob ( pid_t pgid, std::string cmdstr ) {
    static struct termios tmod;
    return jobList.insert(jobList.end(), {++jobIndex, pgid, 
            Job::RUNNING, tmod, true, cmdstr});
}
/* -----  end of function addJob  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  removeJob
 *  Description:
 * =====================================================================================
 */
void Shell::removeJob ( job_iter j ) {
    if(j == first_default) {
        first_default = second_default;
        second_default = end();
    } else if(j == second_default)
        second_default = end();

    if(j->id == jobIndex)
        --jobIndex;
    jobList.erase(j);
}
/* -----  end of function removeJob  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  findJobById
 *  Description:
 * =====================================================================================
 */
Shell::job_iter Shell::findJobById ( int id ) {
    if( id > 0 ) {
        auto i = jobList.begin();
        for(; i != jobList.end(); ++i)
            if(i->id == id)
                break;
        return i;
    } else {
        return first_default;
    }
}
/* -----  end of function findJobById  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  findJobByPid
 *  Description:
 * =====================================================================================
 */
Shell::job_iter Shell::findJobByPid (pid_t pgid ) {
    auto i = jobList.begin();
    for(; i != jobList.end(); ++i)
        if(i->pgid == pgid)
            break;
    return i;
}
/* -----  end of function findJobByPid  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  end
 *  Description:
 * =====================================================================================
 */
Shell::job_iter Shell::end ( ) {
    return jobList.end();
}
/* -----  end of function end  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  operator<<
 *  Description:
 * =====================================================================================
 */
std::ostream& operator<< ( std::ostream& os, Shell::Job const& j ) {
    os << '[' << j.id << ']'
        << ((Shell::first_default != Shell::end() && j.id == Shell::first_default->id) ? '+' 
            : ((Shell::second_default != Shell::end() && j.id == Shell::second_default->id) ? '-' : ' ' ) )
        << "\t" << ( j.status == Shell::Job::STOPPED ? "Stopped" 
                    : (j.status == Shell::Job::RUNNING ? "Running" : "Done  ") )
        << "\t\t" << j.cmdstr;
    return os;
}
/* -----  end of function operator<<  ----- */

