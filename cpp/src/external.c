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

int main() {
    printf("Sleeping...\n");
    sleep(2);
    printf("Waked!\n");

    return 0;
}
