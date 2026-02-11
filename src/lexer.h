#pragma once
#include <stdbool.h>

bool fill_input_buffer(char *input);
char *get_rest_of_input_buffer();
int get_token();
char *get_lexeme();
