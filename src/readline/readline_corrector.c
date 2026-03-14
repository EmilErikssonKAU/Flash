#include "readline_corrector.h"
#include "../cursor/cursor.h"

int readline_corrector()
{
    // Move down cursor one row
    move_cursor_down();

    // Print correction suggestion
    printf("New suggestion!");

    // Move cursor back up one row

    // Move cursor correct amount of steps forward
}