#include "tokentable.h"

#include <string.h>

bool is_keyword(const char *lexeme)
{
    static const char *keywords[] = {"exit", "echo", "type", "pwd", "cd"};
    if (lexeme == NULL)
    {
        return false;
    }

    size_t keyword_count = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < keyword_count; i++)
    {
        if (!strcmp(lexeme, keywords[i]))
        {
            return true;
        }
    }

    return false;
}

bool isRedirect(TokKind tok)
{
    if (isAppend(tok))
    {
        return true;
    }
    if (tok == TOK_REDIR_FD1 || tok == TOK_REDIR_FD2 || tok == TOK_REDIR_STDOUT)
    {
        return true;
    }

    return false;
}

// Appends are viewed as a subset of redirects
bool isAppend(TokKind tok)
{
    if (tok == TOK_APPEND_FD1 || tok == TOK_APPEND_FD2 || tok == TOK_APPEND_STDOUT)
    {
        return true;
    }

    return false;
}

TokKind redirKindToTok(RedirKind kind)
{
    switch (kind)
    {
    case REDIR_STDOUT:
        return TOK_REDIR_STDOUT;
    case REDIR_FD1:
        return TOK_REDIR_FD1;
    case REDIR_FD2:
        return TOK_REDIR_FD2;
    case APPEND_STDOUT:
        return TOK_APPEND_STDOUT;
    case APPEND_FD1:
        return TOK_APPEND_FD1;
    case APPEND_FD2:
        return TOK_APPEND_FD2;
    default:
        return TOK_ERR;
    }
}
