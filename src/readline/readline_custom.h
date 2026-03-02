#pragma once

#include <readline/readline.h>

void init_readline();
char **readline_completion(const char *text, int start, int end);
