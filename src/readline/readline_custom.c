#include "readline/readline_custom.h"
#include "readline/readline_corrector.h"

void init_readline()
{
    rl_attempted_completion_function = readline_completion;
    rl_event_hook = readline_corrector;
}
