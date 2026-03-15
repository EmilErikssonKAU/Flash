#include "cursor.h"
#include <stdlib.h>

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
