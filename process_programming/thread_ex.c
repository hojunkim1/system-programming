/* thread example by J. Choi (choijm@dku.edu) */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int a = 10;

void *func()
{
    a++;
    printf("pid = %d\n", getpid());
}

int main()
{
    int p_thread;
    if ((pthread_create(&p_thread, NULL, func, (void *)NULL)) < 0)
    {
        exit(0);
    }
    pthread_join(p_thread, (void *)NULL);
    printf("a = %d by pid = %d\n", a, getpid());
}
