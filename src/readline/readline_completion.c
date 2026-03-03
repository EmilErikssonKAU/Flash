#include "readline_custom.h"

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// Temporary placement
static const char *builtin_names[] = {
    "cd",
    "echo",
    "exit",
    "pwd",
    "type",
    NULL,
};

static char *builtin_name_generator(const char *text, int state)
{
    static size_t i = 0;
    static size_t text_len = 0;

    // Reset i upon new completion request
    if (state == 0)
    {
        i = 0;
        text_len = strlen(text);
    }

    while (builtin_names[i] != NULL)
    {
        const char *name = builtin_names[i++];
        if (!strncmp(name, text, text_len))
        {
            return strdup(name);
        }
    }

    return NULL;
}

static char *path_name_generator(const char *text, int state)
{
    static char **path_names = NULL;
    static size_t path_names_count = 0;
    static size_t idx = 0;
    static size_t text_len = 0;

    if (state == 0)
    {
        for (size_t i = 0; i < path_names_count; i++)
        {
            free(path_names[i]);
        }
        free(path_names);
        path_names = NULL;
        path_names_count = 0;
        idx = 0;
        text_len = strlen(text);

        char *path_env = getenv("PATH");
        char *path_copy = strdup(path_env);

        char *saveptr = NULL;
        // Path entries split at ':'
        char *dir_path = strtok_r(path_copy, ":", &saveptr);
        while (dir_path != NULL)
        {
            DIR *dir = opendir(dir_path);
            if (dir != NULL)
            {
                struct dirent *entry;
                while ((entry = readdir(dir)) != NULL)
                {
                    char full_path[1024];
                    int written = snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
                    if (written < 0 || written >= sizeof(full_path))
                    {
                        // String could not be written in its entirety
                        continue;
                    }

                    if (access(full_path, X_OK) != 0)
                    {
                        continue;
                    }

                    // Skip duplicates
                    bool duplicate = false;
                    for (size_t i = 0; i < path_names_count; i++)
                    {
                        if (!strcmp(path_names[i], entry->d_name))
                        {
                            duplicate = true;
                            break;
                        }
                    }
                    if (duplicate)
                    {
                        continue;
                    }

                    char **tmp = realloc(path_names, (path_names_count + 1) * sizeof(char *));
                    path_names = tmp;
                    path_names[path_names_count] = strdup(entry->d_name);
                    if (path_names[path_names_count] == NULL)
                    {
                        continue;
                    }
                    path_names_count++;
                }
                closedir(dir);
            }

            dir_path = strtok_r(NULL, ":", &saveptr);
        }

        free(path_copy);
    }

    while (idx < path_names_count)
    {
        const char *name = path_names[idx++];
        if (!strncmp(name, text, text_len))
        {
            return strdup(name);
        }
    }

    return NULL;
}

static char *custom_generator(const char *text, int state)
{
    static int builtin_state = 0;
    static int path_state = 0;
    static bool use_path_generator = false;

    if (state == 0)
    {
        builtin_state = 0;
        path_state = 0;
        use_path_generator = false;
    }

    if (!use_path_generator)
    {
        char *builtin_match = builtin_name_generator(text, builtin_state);
        builtin_state++;
        if (builtin_match != NULL)
        {
            return builtin_match;
        }
        use_path_generator = true;
    }

    char *path_match = path_name_generator(text, path_state++);
    if (path_match != NULL)
    {
        return path_match;
    }

    return NULL;
}

char **readline_completion(const char *text, int start, int end)
{
    // Only completion support for first word so far
    if (start == 0)
    {
        return rl_completion_matches(text, custom_generator);
    }

    return NULL;
}
