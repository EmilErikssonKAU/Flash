#include "cursor.h"
#include <stdlib.h>
#include "../settings/extract_settings.h"
#include <readline/readline.h>

void move_cursor_down()
{
    const char *ansi_move_down = "\033[1B";
    printf(ansi_move_down);
}

void move_cursor_up()
{
    const char *ansi_move_up = "\033[1A";
    printf(ansi_move_up);
}

void clear_line()
{
    const char *clear_line = "\033[2K";
    printf(clear_line);
}

void move_cursor_end_of_prompt()
{
    size_t prompt_length = get_prompt_length();
    size_t readline_length = strlen(rl_line_buffer);
    size_t total_length = prompt_length + readline_length;
    printf("\033[%zuC", total_length);
}
