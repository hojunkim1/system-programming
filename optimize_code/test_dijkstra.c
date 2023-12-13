/* test_dijkstra.c : optimize test by Hojun Kim. meringyee@gmail.com */
#define __TEST__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#define TRUE 1
#define FALSE 0
#define INF 10000
#define MAX_VERTICES 5

int distance[MAX_VERTICES];
int S[MAX_VERTICES];

int next_vertex(int n);
int print_step(int step);
void dijkstra_shortest_path(int start, int n);
void test_dijkstra(void dijkstra_shortest_path(int, int), int start, int n);

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
    printf("Shortest Path\n");
#endif
    for (i = 0; i < 10; i++)
        test_dijkstra(dijkstra_shortest_path, 0, MAX_VERTICES);
    return 0;
}

void test_dijkstra(void dijkstra_shortest_path(int, int), int start, int n)
{
    int i;
    struct timeval stime, etime, gap;

    gettimeofday(&stime, NULL);
    for (i = 0; i < 1000000; i++)
        (*dijkstra_shortest_path)(start, n);
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

int next_vertex(int n)
{
    int i, min, minPos;
    min = INF;
    minPos = -1;
    for (i = 0; i < n; i++)
    {
        if ((distance[i] < min) && !S[i])
        {
            min = distance[i];
            minPos = i;
        }
    }
    return minPos;
}

int print_step(int step)
{
    int i;
    printf("\nStep %d: S = {", step);
    for (i = 0; i < MAX_VERTICES; i++)
    {
        if (S[i] == TRUE)
            printf(" %c ", i + 65);
    }

    if (step < 3)
        printf("}\t\t");
    else
        printf("}\t");
    printf("distance: [ ");
    for (i = 0; i < MAX_VERTICES; i++)
    {
        if (distance[i] == INF)
            printf("%2c  ", '*');
        else
            printf("%2d  ", distance[i]);
    }
    printf("]");
    return ++step;
}

void dijkstra_shortest_path(int start, int n)
{
    int i, u, w, step = 0;

    for (i = 0; i < n; i++)
    {
        distance[i] = weight[start][i];
        S[i] = FALSE;
    }

    S[start] = TRUE;
    distance[start] = 0;

#ifndef __TEST__
    step = print_step(0);
#else
    step++;
#endif

    for (i = 0; i < n - 1; i++)
    {
        u = next_vertex(n);
        S[u] = TRUE;
        for (w = 0; w < n; w++)
        {
            if (!S[w] && distance[u] + weight[u][w] < distance[w])
                distance[w] = distance[u] + weight[u][w];
        }
#ifndef __TEST__
        step = print_step(step);
#else
        step++;
#endif
    }
#ifndef __TEST__
    printf("\n");
#endif
}
