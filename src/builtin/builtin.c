#include "builtin.h"
#include <string.h>

static const BuiltinEntry builtins[] = {
    {"cd", builtin_cd},
    {"echo", builtin_echo},
    {"exit", builtin_exit},
    {"pwd", builtin_pwd},
    {"type", builtin_type},
};

bool isBuiltIn(char *word)
{
    if (word == NULL || word[0] == '\0')
    {
        return false;
    }

    size_t builtins_count = sizeof(builtins) / sizeof(builtins[0]);
    for (size_t i = 0; i < builtins_count; i++)
    {
        if (!strcmp(word, builtins[i].name))
        {
            return true;
        }
    }

    return false;
}

bool executeBuiltIn(int argc, char *argv[])
{
    for (size_t i = 0; i < sizeof(builtins) / sizeof(builtins[0]); i++)
    {
        if (!strcmp(builtins[i].name, argv[0]))
        {
            return builtins[i].funct(argc, argv);
        }
    }

    return false;
}
