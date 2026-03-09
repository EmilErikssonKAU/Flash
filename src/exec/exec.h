#pragma once
#include "ast/ast.h"

typedef struct
{
    int status;
    bool shouldExit;
} ExecResult;

ExecResult exec(AstNode *node);
ExecResult exec_simple_command(AstCommand *command);
ExecResult exec_pipeline(AstPipeline *pipeline);
ExecResult exec_and_or(AstAndOr *and_or);
ExecResult exec_list(AstList *list);
bool execute_program(char *argv[], RedirVec *redir);