#include "commands/commands.h"
#include "builtin/builtin.h"

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define INITIAL_CAP 256

static void push(char ***arr, int *count, int *cap, char *entry)
{
    if (*count >= *cap)
    {
        *cap *= 2;
        *arr = realloc(*arr, *cap * sizeof(char *));
    }
    (*arr)[(*count)++] = entry;
}

char **get_all_commands(int *count)
{
    int cap = INITIAL_CAP;
    int n = 0;
    char **cmds = malloc(cap * sizeof(char *));

    // Add builtins
    int builtin_count = 0;
    const char **builtin_names = get_builtin_names(&builtin_count);
    for (int i = 0; i < builtin_count; i++)
        push(&cmds, &n, &cap, strdup(builtin_names[i]));

    // Scan each directory in $PATH
    char *path_env = getenv("PATH");
    if (!path_env)
    {
        *count = n;
        return cmds;
    }

    char *path_copy = strdup(path_env);
    char *dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        DIR *d = opendir(dir);
        if (d)
        {
            struct dirent *entry;
            while ((entry = readdir(d)) != NULL)
            {
                if (entry->d_name[0] == '.')
                    continue;

                // Build full path to check executability
                size_t full_len = strlen(dir) + 1 + strlen(entry->d_name) + 1;
                char *full = malloc(full_len);
                snprintf(full, full_len, "%s/%s", dir, entry->d_name);

                if (access(full, X_OK) == 0)
                    push(&cmds, &n, &cap, strdup(entry->d_name));

                free(full);
            }
            closedir(d);
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);

    *count = n;
    return cmds;
}

void free_commands(char **commands, int count)
{
    for (int i = 0; i < count; i++)
        free(commands[i]);
    free(commands);
}
