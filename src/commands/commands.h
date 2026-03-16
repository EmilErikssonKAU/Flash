#pragma once

// Returns a heap-allocated array of all known command names (builtins + $PATH executables).
// *count is set to the number of entries. Caller must free with free_commands().
char **get_all_commands(int *count);
void free_commands(char **commands, int count);
