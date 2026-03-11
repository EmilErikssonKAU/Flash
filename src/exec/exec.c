#include "exec.h"

ExecResult exec(AstNode *node)
{
    ExecResult exec_result = {.status = 1, .shouldExit = false};
    if (node == NULL)
    {
        return exec_result;
    }

    switch (node->nodetype)
    {
    case (CommandNode):
        exec_result = exec_simple_command(node->cmd);
        break;
    case (PipelineNode):
        exec_result = exec_pipeline(node->pipeline);
        break;
    case (AndOrNode):
        exec_result = exec_and_or(node->and_or);
        break;
    case (ListNode):
        exec_result = exec_list(node->list);
        break;
    default:
        break;
    }

    return exec_result;
}
