#include "builtin/builtin.h"
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 1024

bool builtin_pwd(int argc, char **argv)
{
    char cwd[BUF_SIZE];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
    return false;
}
