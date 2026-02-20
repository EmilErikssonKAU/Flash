#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "exec.h"

bool execute_program(char *argv[])
{
    pid_t pid = fork();

    if (pid == 0) // child process
    {
        execvp(argv[0], argv);
        exit(1);
    }
    else // parent process
    {
        wait(NULL);
    }

    // Not correct but I have other problems right now
    return true;
}
