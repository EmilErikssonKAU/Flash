#include "readline_custom.h"

void init_readline()
{
    rl_attempted_completion_function = readline_completion;
}
