#include "ast.h"
#include <stdlib.h>

static void free_command_fields(AstCommand *cmd)
{
    if (cmd == NULL)
    {
        return;
    }

    if (cmd->argv.v != NULL)
    {
        for (size_t i = 0; i < cmd->argv.n; i++)
        {
            free(cmd->argv.v[i]);
        }
        free(cmd->argv.v);
    }

    if (cmd->redirs.v != NULL)
    {
        for (size_t i = 0; i < cmd->redirs.n; i++)
        {
            free(cmd->redirs.v[i].target);
        }
        free(cmd->redirs.v);
    }
}

static void free_pipeline(AstPipeline *pipeline)
{
    if (pipeline == NULL)
    {
        return;
    }

    if (pipeline->commands != NULL)
    {
        for (size_t i = 0; i < pipeline->n; i++)
        {
            free_command_fields(&pipeline->commands[i]);
        }
        free(pipeline->commands);
    }

    free(pipeline);
}

static void free_and_or(AstAndOr *and_or)
{
    if (and_or == NULL)
    {
        return;
    }

    AstPipeline first_pipeline = and_or->first;
    AstPipeline *first_ptr = &first_pipeline;
    if (first_ptr->commands != NULL)
    {
        for (size_t i = 0; i < first_ptr->n; i++)
        {
            free_command_fields(&first_ptr->commands[i]);
        }
        free(first_ptr->commands);
    }

    if (and_or->links != NULL)
    {
        for (size_t i = 0; i < and_or->n; i++)
        {
            free_pipeline(and_or->links[i].pipeline);
        }
        free(and_or->links);
    }

    free(and_or);
}

static void free_list(AstList *list)
{
    if (list == NULL)
    {
        return;
    }

    if (list->items != NULL)
    {
        for (size_t i = 0; i < list->n; i++)
        {
            AstAndOr *item = &list->items[i];
            AstPipeline first_pipeline = item->first;
            AstPipeline *first_ptr = &first_pipeline;
            if (first_ptr->commands != NULL)
            {
                for (size_t j = 0; j < first_ptr->n; j++)
                {
                    free_command_fields(&first_ptr->commands[j]);
                }
                free(first_ptr->commands);
            }

            if (item->links != NULL)
            {
                for (size_t j = 0; j < item->n; j++)
                {
                    free_pipeline(item->links[j].pipeline);
                }
                free(item->links);
            }
        }
        free(list->items);
    }

    free(list);
}

void free_ast(AstNode *ast)
{
    if (ast == NULL)
    {
        return;
    }

    if (ast->cmd != NULL)
    {
        free_command_fields(ast->cmd);
        free(ast->cmd);
    }
    free_pipeline(ast->pipeline);
    free_and_or(ast->and_or);
    free_list(ast->list);

    free(ast);
}
