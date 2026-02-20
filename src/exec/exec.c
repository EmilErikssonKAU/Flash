#include "exec.h"

ExecResult exec(AstNode *node)
{
    ExecResult exec_result;
    if (node == NULL)
    {
        ; // cry
    }

    switch (node->nodetype)
    {
    case (CommandNode):
        exec_result = exec_simple_command(node);
        break;
    }

    return exec_result;
}
