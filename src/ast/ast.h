#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    REDIR_STDOUT,
    REDIR_FD1,
    REDIR_FD2
} RedirKind;

typedef enum
{
    CommandNode
} NodeType;

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

// Only AstCommand for now.
typedef struct AstNode
{
    NodeType nodetype;
    AstCommand *cmd;
} AstNode;

void free_ast(AstNode *ast);
