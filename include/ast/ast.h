#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    REDIR_STDOUT,
    REDIR_FD1,
    REDIR_FD2,
    APPEND_STDOUT,
    APPEND_FD1,
    APPEND_FD2
} RedirKind;

typedef enum
{
    CommandNode,
    PipelineNode,
    AndOrNode,
    ListNode
} NodeType;

typedef enum
{
    LOGIC_AND,
    LOGIC_OR
} LogicOp;

typedef struct
{
    RedirKind kind;
    char *target;
} Redir;

typedef struct
{
    char **v;
    size_t n;
} StrVec;

typedef struct
{
    Redir *v;
    size_t n;
} RedirVec;

typedef struct
{
    StrVec argv;
    RedirVec redirs;
} AstCommand;

typedef struct
{
    AstCommand *commands;
    size_t n;
} AstPipeline;

typedef struct
{
    LogicOp op;
    AstPipeline *pipeline;
} AstAndOrLink;

typedef struct
{
    AstPipeline first;
    AstAndOrLink *links;
    size_t n;
} AstAndOr;

typedef struct
{
    AstAndOr *items;
    size_t n;
} AstList;

typedef struct
{
    NodeType nodetype;
    AstCommand *cmd;
    AstPipeline *pipeline;
    AstAndOr *and_or;
    AstList *list;
} AstNode;

void free_ast(AstNode *ast);
