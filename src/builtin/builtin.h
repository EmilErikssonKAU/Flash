#pragma once

#include <stdbool.h>

typedef bool (*BuiltinFn)(int argc, char **argv);

typedef struct
{
    const char *name;
    BuiltinFn funct;
} BuiltinEntry;

bool builtin_cd(int argc, char **argv);
bool builtin_echo(int argc, char **argv);
bool builtin_exit(int argc, char **argv);
bool builtin_pwd(int argc, char **argv);
bool builtin_type(int argc, char **argv);

bool isBuiltIn(char *word);
bool executeBuiltIn(int argc, char **argv);
