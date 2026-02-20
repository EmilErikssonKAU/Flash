#pragma once
#include "ast/ast.h"

typedef struct
{
    int status;
    bool shouldExit;
} ExecResult;

ExecResult exec(AstNode *node);
ExecResult exec_simple_command(AstNode *node);
bool execute_program(char *argv[]);