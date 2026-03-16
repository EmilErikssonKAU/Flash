#include "readline_corrector.h"
#include "../cursor/cursor.h"
#include "../commands/commands.h"
#include "../levenshtein/levenshtein_cluster.h"

#include <readline/readline.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char **all_commands = NULL;
static int command_count = 0;
static char last_input[256] = {0};

static void ensure_commands_loaded()
{
    if (all_commands == NULL)
        all_commands = get_all_commands(&command_count);
}

int readline_corrector()
{
    ensure_commands_loaded();

    // Extract the first word from the current input (the command being typed)
    char first_word[256] = {0};
    const char *buf = rl_line_buffer;
    int len = 0;
    while (buf[len] && buf[len] != ' ' && len < (int)sizeof(first_word) - 1)
    {
        first_word[len] = buf[len];
        len++;
    }
    first_word[len] = '\0';

    if (strcmp(buf, last_input) == 0)
        return 0;
    strncpy(last_input, buf, sizeof(last_input) - 1);
    last_input[sizeof(last_input) - 1] = '\0';

    move_cursor_down();
    clear_line();

    if (len > 0)
    {
        // Build words array: words[0] = query, words[1..n] = candidates
        char **words = malloc((command_count + 1) * sizeof(char *));
        words[0] = first_word;
        for (int i = 0; i < command_count; i++)
            words[i + 1] = all_commands[i];

        int idx = find_most_levenshtein(words, command_count + 1);
        if (idx >= 0)
            printf("\rDid you mean: %s", all_commands[idx - 1]);

        free(words);
    }

    printf("\r");
    move_cursor_up();
    move_cursor_end_of_prompt();
    return 0;
}
