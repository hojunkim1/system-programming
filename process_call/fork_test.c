/* fork_test.c example, Sept. 26, choijm@dku.edu */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t fork_return;
    printf("Hello, my pid is %d\n", getpid());

    if ((fork_return = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (fork_return == 0)
    {
        /* child process */
        printf("child: pid = %d, ppid = %d\n", getpid(), getppid());
    }
    else
    {
        /* parent process */
        // wait();
        printf("parent: I created child with pid=%d\n", fork_return);
    }

    /* Following line is executed by both parent and child */
    printf("Bye, my pid is %d\n", getpid());
    return 0;
}
