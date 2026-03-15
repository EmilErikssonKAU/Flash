#include "readline_corrector.h"
#include "../cursor/cursor.h"

int readline_corrector()
{
    // Move down cursor one row
    move_cursor_down();

    // // // Clear line
    clear_line();

    // // // Print correction suggestion
    printf("\rNew suggestion!");

    // // Move cursor back up one row
    move_cursor_up();

    // Move cursor correct amount of steps forward
    move_cursor_end_of_prompt();
}