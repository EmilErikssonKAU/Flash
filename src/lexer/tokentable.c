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
    if (tok == TOK_REDIR_FD1 || tok == TOK_REDIR_FD2 || tok == TOK_REDIR_STDOUT)
    {
        return true;
    }

    return false;
}
