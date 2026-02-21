#include "builtin.h"
#include <stdlib.h>

bool builtin_exit(int argc, char **argv)
{
    exit(0);
    return true;
}
