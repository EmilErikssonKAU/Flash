#include "exec/exec.h"
#include "ast/ast.h"
#include "builtin/builtin.h"
#include "exec/path.h"
#include <stdio.h>

ExecResult exec_simple_command(AstCommand *cmd)
{
    ExecResult exec_result;
    exec_result.status = 1;
    exec_result.shouldExit = false;

    if (cmd == NULL || cmd->argv.v == NULL || cmd->argv.n == 0 || cmd->argv.v[0] == NULL)
    {
        exec_result.status = 0;
        return exec_result;
    }

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
        exec_result.status = 127;
    }

    return exec_result;
}
