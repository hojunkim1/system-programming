/* fork_test2.c: accessing variables, Sept. 26, choijm@dku.edu */
/* Note: This code is borrowed from “Advanced Programming in the UNIX Env.” */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int glob = 6;
char buf[] = "a write to stdout\n";

int main(void)
{
    int var = 88;
    pid_t fork_return;

    if (write(STDOUT_FILENO, buf, sizeof(buf)) != sizeof(buf))
    {
        perror("write error");
        exit(1);
    }
    printf("before fork\n"); /* we don't flush stdout */

    if ((fork_return = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (fork_return == 0)
    {
        /* child */
        /* modify variables */
        glob++;
        var++;
    }
    else
    {
        /* parent */
        sleep(2);
    }

    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
    return 0;
}
