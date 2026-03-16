#pragma once
#include <stdbool.h>
#include "lexer/tokentable.h"

bool fill_input_buffer(const char *input);
const char *get_rest_of_input_buffer();
Token get_token(void);
const char *get_lexeme(void);
