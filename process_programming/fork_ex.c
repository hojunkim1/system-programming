/* fork example by J. Choi (choijm@dku.edu) */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int a = 10;

void *func()
{
    a++;
    printf("pid = %d\n", getpid());
}

int main()
{
    int pid;

    if ((pid = fork()) == 0)
    { // need exception handle
        func();
        exit(0);
    }
    // wait();
    printf("a = %d by pid = %d\n", a, getpid());
}
