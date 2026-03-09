#include "exec.h"
#include "ast/ast.h"
#include "exec/exec.h"
#include "builtin/builtin.h"
#include "path.h"
#include <stdio.h>

ExecResult exec_simple_command(AstCommand *cmd)
{
    ExecResult exec_result;
    exec_result.shouldExit = false;
    RedirVec redirs = cmd->redirs;

    char **argv = cmd->argv.v;
    size_t argc = cmd->argv.n;

    if (isBuiltIn(argv[0]))
    {
        if (executeBuiltIn(argc, argv, &redirs))
        {
            exec_result.status = 0;
        }
        else
        {
            exec_result.status = 1;
        }
    }
    else if (checkPath(argv[0], false))
    {
        if (execute_program(argv, &redirs))
        {
            exec_result.status = 0;
        }
        else
        {
            exec_result.status = 1;
        }
    }
    else
    {
        printf("%s: command not found\n", argv[0]);
    }

    return exec_result;
}