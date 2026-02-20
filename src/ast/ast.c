#include "ast.h"
#include <stdlib.h>

void free_ast(AstNode *ast)
{
    if (ast == NULL)
    {
        return;
    }

    if (ast->cmd != NULL)
    {
        if (ast->cmd->argv.v != NULL)
        {
            for (size_t i = 0; i < ast->cmd->argv.n; i++)
            {
                free(ast->cmd->argv.v[i]);
            }
            free(ast->cmd->argv.v);
        }

        if (ast->cmd->redirs.v != NULL)
        {
            for (size_t i = 0; i < ast->cmd->redirs.n; i++)
            {
                free(ast->cmd->redirs.v[i].target);
            }
            free(ast->cmd->redirs.v);
        }

        free(ast->cmd);
    }

    free(ast);
}
