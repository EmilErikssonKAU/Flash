#include "builtin.h"

bool builtin_exit(int argc, char **argv)
{
    exit(1);
    return true;
}
