#include "readline_corrector.h"
#include "../cursor/cursor.h"

int readline_corrector()
{
    static int test = 1;
    // Move down cursor one row
    move_cursor_down();

    // Clear line
    clear_line();

    // Print correction suggestion
    printf("\rNew suggestion!%d", test % 10);

    // Move cursor back to start of line;
    printf("\r");

    // Move cursor back up one row
    move_cursor_up();

    // Move cursor correct amount of steps forward
    move_cursor_end_of_prompt();
    test++;
}
