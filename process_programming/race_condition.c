/* Race condition example by choijm. From Advanced Programming in UNIX Env. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

static void charatatime(char *str)
{
    int i;

    for (; *str; str++)
    {
        for (i = 0; i < 1000; i++)
            ;
        write(STDOUT_FILENO, str, 1);
    }
}

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        charatatime("output from child\n");
    }
    else
    {
        charatatime("output from parent\n");
    }
}
