#include "custom_keybinds.h"
#include <readline/readline.h>

int guarded_backspace(int count, int key)
{
    if (rl_point == 0)
    {
        rl_ding(); // Optional if you hate the user
        return 0;
    }

    rl_delete_text(rl_point - 1, rl_point);
    rl_point--;
    return 1;
}