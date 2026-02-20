#include "path.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

char *extractPath(const char *PATH)
{
    char *buf;
    if (PATH != NULL)
        buf = strdup(PATH);
    else
        buf = NULL;
    char *path_section = strtok(buf, ":");
    return path_section;
}

bool checkPath(const char *lexeme, bool should_print)
{
    char *PATH = getenv("PATH");
    char *path_section = extractPath(PATH);

    while (path_section != NULL)
    {
        char buffer[BUF_SIZE];
        snprintf(buffer, sizeof(buffer), "%s/%s", path_section, lexeme);

        if (!access(buffer, X_OK))
        {
            if (should_print)
                printf("%s is %s\n", lexeme, buffer);
            return true;
        }
        path_section = extractPath(NULL);
    }
    return false;
}

char *expand_path(const char *path)
{
    if (!path)
        return NULL;

    if (path[0] != '~')
    {
        char *expansion = malloc(strlen(path) + 1);
        memcpy(expansion, path, strlen(path) + 1);
        return expansion;
    }

    const char *home = getenv("HOME");
    size_t expanded_path_len = strlen(home) + strlen(path) + 1;
    char *expanded_path = malloc(expanded_path_len);
    snprintf(expanded_path, expanded_path_len, "%s%s", home, path + 1);
    return expanded_path;
}
