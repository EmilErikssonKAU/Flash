#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    REDIR_STDOUT,
    REDIR_FD1,
    REDIR_FD2
} RedirKind;

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

typedef struct AstNode
{
    AstCommand *cmd;
} AstNode;
