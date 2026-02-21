#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "ast/ast.h"

typedef enum
{
    TOK_WORD,
    TOK_REDIR_STDOUT,
    TOK_REDIR_FD1,
    TOK_REDIR_FD2,
    TOK_APPEND_STDOUT,
    TOK_APPEND_FD1,
    TOK_APPEND_FD2,
    TOK_NL,
    TOK_EOF,
    TOK_ERR
} TokKind;

typedef struct
{
    TokKind kind;
    const char *lexeme;
    size_t len;
} Token;

bool is_keyword(const char *lexeme);
bool isRedirect(TokKind tok);
bool isAppend(TokKind tok);
TokKind redirKindToTok(RedirKind kind);
