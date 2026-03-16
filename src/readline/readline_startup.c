#include "readline_startup.h"
#include "custom_keybinds.h"

int readline_startup(void)
{
    // Probably exists better solution
    rl_bind_key('\b', guarded_backspace);
    rl_bind_key(127, guarded_backspace);
    return 0;
}