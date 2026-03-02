#include "readline_custom.h"

#include <stdlib.h>
#include <string.h>

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

char **readline_completion(const char *text, int start, int end)
{
    // Only completion support for first word so far
    if (start == 0)
    {
        return rl_completion_matches(text, builtin_name_generator);
    }

    return NULL;
}
