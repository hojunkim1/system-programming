/* shell.c : shell program by Hojun Kim. meringyee@gmail.com */
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXARGS 128

extern char **environ;

int parseln(char *buff, char **argv);
void execute(char **argv, int bg, char *cmdline);
int corecmd(char **argv);
void unix_error(char *msg);

int main()
{
    char cmdline[MAXARGS]; /* Command line */
    char buff[MAXARGS];
    char *argv[MAXARGS];
    int bg;

    while (1)
    {
        /* Read */
        printf("> ");
        fgets(cmdline, MAXARGS, stdin);
        if (feof(stdin))
            exit(0);

        /* Evaluate */
        strcpy(buff, cmdline);
        bg = parseln(buff, argv);

        /* Ignore empty lines */
        if (argv[0] == NULL)
            continue;

        /* Execute */
        if (!corecmd(argv))
            execute(argv, bg, cmdline);
    }
}

int parseln(char *buff, char **argv)
{
    char *delim; /* Points to first space delimiter */
    int argc;
    int bg;

    /* Replace trailing '\n' with space */
    buff[strlen(buff) - 1] = ' ';

    /* Ignore leading spaces */
    while (*buff && (*buff == ' '))
        buff++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buff, ' ')))
    {
        argv[argc++] = buff;
        *delim = '\0';
        buff = delim + 1;
        while (*buff && (*buff == ' ')) /* Ignore spaces */
            buff++;
    }
    argv[argc] = NULL;

    /* Ignore blank line */
    if (argc == 0)
        return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc - 1] == '&')) != 0)
        argv[--argc] = NULL;

    return bg;
}

void execute(char **argv, int bg, char *cmdline)
{
    pid_t pid;

    /* Fork process */
    if ((pid = fork()) < 0)
        unix_error("Fork error");

    /* Child runs user job */
    if (pid == 0)
    {
        if (execve(argv[0], argv, environ) < 0)
        {
            printf("%s: Command not found.\n", argv[0]);
            exit(0);
        }
    }

    /* Parent waits for foregrround job to terminate */
    if (!bg)
    {
        int status;
        if (waitpid(pid, &status, 0) < 0)
            unix_error("waitfg: waitpid error");
    }
    else
        printf("%d %s", pid, cmdline);
}

int corecmd(char **argv)
{
    if (!strcmp(argv[0], "quit"))
        exit(0);
    if (!strcmp(argv[0], "&"))
        return 1;
    return 0;
}

void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
