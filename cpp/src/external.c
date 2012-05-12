/*
 * =====================================================================================
 *
 *       Filename:  external.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/17/2012 09:45:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (boss14420), boss14420@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int tm = 2;
    if(argc == 2)
        tm = atoi(argv[1]);

    printf("Sleeping...\n");
    sleep(tm);
    printf("Waked!\n");

    return 0;
}
