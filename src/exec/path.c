#include "exec/path.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

char *extractPath(const char *PATH)
{
    static char *path_ctx = NULL;
    static char *path_buf = NULL;

    if (PATH != NULL)
    {
        free(path_buf);
        path_buf = strdup(PATH);
        if (path_buf == NULL)
            return NULL;
        path_ctx = path_buf;
    }

    if (path_ctx == NULL)
        return NULL;

    char *path_section = path_ctx;
    char *delimiter = strchr(path_ctx, ':');

    if (delimiter != NULL)
    {
        *delimiter = '\0';
        path_ctx = delimiter + 1;
    }
    else
    {
        path_ctx = NULL;
    }

    return path_section;
}

bool checkPath(const char *lexeme, bool should_print)
{
    if (lexeme == NULL || lexeme[0] == '\0')
        return false;

    // Explicit paths like ./a.out or /usr/bin/ls should not be resolved via PATH.
    if (strchr(lexeme, '/') != NULL)
    {
        if (!access(lexeme, X_OK))
        {
            if (should_print)
                printf("%s is %s\n", lexeme, lexeme);
            return true;
        }
        return false;
    }

    char *PATH = getenv("PATH");
    if (PATH == NULL)
        return false;

    char *path_section = extractPath(PATH);

    while (path_section != NULL)
    {
        char buffer[BUF_SIZE];
        const char *dir = (path_section[0] == '\0') ? "." : path_section;
        snprintf(buffer, sizeof(buffer), "%s/%s", dir, lexeme);

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
