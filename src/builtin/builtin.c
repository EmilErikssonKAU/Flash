#include "builtin.h"
#include "exec/redir.h"
#include <string.h>

static const BuiltinEntry builtins[] = {
    {"cd", builtin_cd},
    {"echo", builtin_echo},
    {"exit", builtin_exit},
    {"pwd", builtin_pwd},
    {"type", builtin_type},
};

const char **get_builtin_names(int *count)
{
    static const char *names[sizeof(builtins) / sizeof(builtins[0])];
    size_t n = sizeof(builtins) / sizeof(builtins[0]);
    for (size_t i = 0; i < n; i++)
        names[i] = builtins[i].name;
    *count = (int)n;
    return names;
}

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

bool executeBuiltIn(int argc, char *argv[], RedirVec *redirs)
{
    for (size_t i = 0; i < sizeof(builtins) / sizeof(builtins[0]); i++)
    {
        if (!strcmp(builtins[i].name, argv[0]))
        {
            int stored_fds[2] = {-1, -1};

            apply_redirs(redirs, stored_fds, true);
            bool status = builtins[i].funct(argc, argv);
            restore_redirs(stored_fds);

            return status;
        }
    }

    return false;
}
