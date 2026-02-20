#pragma once

#include <stdbool.h>

char *extractPath(const char *PATH);
bool checkPath(const char *lexeme, bool should_print);
char *expand_path(const char *path);
