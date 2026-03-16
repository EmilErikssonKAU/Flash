#include "builtin/builtin.h"

#include <stdio.h>

bool builtin_echo(int argc, char **argv)
{
    int i = 1;
    while (argv[i] != NULL)
    {
        if (i > 1)
            printf(" ");
        printf("%s", argv[i]);
        i++;
    }
    printf("\n");
    return false;
}
