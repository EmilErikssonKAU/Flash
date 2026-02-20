#include "builtin.h"
#include "exec/path.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool builtin_cd(int argc, char **argv)
{
    if (argv[1] == NULL || argv[1][0] == '\0')
    {
        return false;
    }

    char *expanded_path = expand_path(argv[1]);

    if (chdir(expanded_path))
    {
        printf("cd: %s: No such file or directory\n", argv[1]);
    }

    free(expanded_path);
    return false;
}
