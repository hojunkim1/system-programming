/* test_floyd.c : optimize test by Hojun Kim. meringyee@gmail.com */
#define __TEST__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#define INF 10000
#define MAX_VERTICES 5

int A[MAX_VERTICES][MAX_VERTICES];

void print_step(int step);
void floyd_shortest_path(int n);
void test_floyd(void floyd_shortest_path(int), int n);

int weight[MAX_VERTICES][MAX_VERTICES] = {
    {0, 10, 5, INF, INF},
    {INF, 0, 2, 1, INF},
    {INF, 3, 0, 9, 2},
    {INF, INF, INF, 0, 4},
    {7, INF, INF, 6, 0},
};

int main()
{
    int i;
#ifndef __TEST__
    printf("Shrotest Path\n");
#endif
    for (i = 0; i < 10; i++)
        test_floyd(floyd_shortest_path, MAX_VERTICES);
    return 0;
}

void test_floyd(void floyd_shortest_path(int), int n)
{
    int i;
    struct timeval stime, etime, gap;

    gettimeofday(&stime, NULL);
    for (i = 0; i < 1000000; i++)
        (*floyd_shortest_path)(n);
    gettimeofday(&etime, NULL);

    gap.tv_sec = etime.tv_sec - stime.tv_sec;
    gap.tv_usec = etime.tv_usec - stime.tv_usec;

    if (gap.tv_usec < 0)
    {
        gap.tv_sec = gap.tv_sec - 1;
        gap.tv_usec = gap.tv_usec + 1000000;
    }

    printf("Elapsed time %ldsec :%dusec\n", gap.tv_sec, gap.tv_usec);
}

void print_step(int step)
{
    int i, j;
    printf("\nA%d: ", step);
    for (i = 0; i < MAX_VERTICES; i++)
    {
        printf("\t");
        for (j = 0; j < MAX_VERTICES; j++)
        {
            if (A[i][j] == INF)
                printf("%4c", '*');
            else
                printf("%4d", A[i][j]);
        }
        printf("\n\n");
    }
}

void floyd_shortest_path(int n)
{
    int v, w, k, step = -1;

    for (v = 0; v < n; v++)
        for (w = 0; w < n; w++)
            A[v][w] = weight[v][w];

#ifndef __TEST__
    print_step(step);
#endif

    for (k = 0; k < n; k++)
    {
        for (v = 0; v < n; v++)
            for (w = 0; w < n; w++)
                if (A[v][k] + A[k][w] < A[v][w])
                    A[v][w] = A[v][k] + A[k][w];
#ifndef __TEST__
        print_step(++step);
#endif
    }
}
