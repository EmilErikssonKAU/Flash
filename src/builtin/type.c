#include "builtin.h"
#include "exec/path.h"
#include "lexer/tokentable.h"
#include <stdio.h>

bool builtin_type(int argc, char **argv)
{
    if (argv[1] == NULL || argv[1][0] == '\0')
    {
        return false;
    }

    if (is_keyword(argv[1]))
    {
        printf("%s is a shell builtin\n", argv[1]);
    }
    else if (!checkPath(argv[1], true))
    {
        printf("%s: not found\n", argv[1]);
    }

    return false;
}
