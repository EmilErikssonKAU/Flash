#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "tokentable.h"

typedef struct tab
{
    const char *text;
    int token;
} tab;

static tab keywordtab[] = {
    {"exit", TOK_EXIT},
    {"echo", TOK_ECHO},
    {"type", TOK_TYPE},
    {"pwd", TOK_PWD},
    {"cd", TOK_CD},
    {"KERROR", nfound}};

static tab operatortab[] = {
    {"1>", TOK_REDIR_STDOUT_FD1},
    {"2>", TOK_REDIR_STDERR},
    {">", TOK_REDIR_STDOUT},
    {"OERROR", nfound}};

static tab tokentab[] = {
    {"undef", TOK_UNDEF},
    {"WORD", TOK_WORD},
    {"NUMBER", TOK_NUMBER},
    {"exit", TOK_EXIT},
    {"echo", TOK_ECHO},
    {"type", TOK_TYPE},
    {"pwd", TOK_PWD},
    {"cd", TOK_CD},
    {"1>", TOK_REDIR_STDOUT_FD1},
    {"2>", TOK_REDIR_STDERR},
    {">", TOK_REDIR_STDOUT},
    {"TERROR", nfound}};

static bool is_number_lexeme(const char *fplex)
{
    if (fplex == NULL || fplex[0] == '\0')
    {
        return false;
    }

    for (size_t i = 0; fplex[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)fplex[i]))
        {
            return false;
        }
    }

    return true;
}

tokentype lex2tok(char *fplex)
{
    // Match explicit operator lexemes first.
    for (int i = 0; operatortab[i].token != nfound; i++)
    {
        if (!strcmp(fplex, operatortab[i].text))
        {
            return operatortab[i].token;
        }
    }

    // Match builtins/keywords next.
    tokentype keyword_token = key2tok(fplex);
    if (keyword_token != TOK_WORD)
    {
        return keyword_token;
    }

    if (is_number_lexeme(fplex))
    {
        return TOK_NUMBER;
    }

    return TOK_WORD;
}

tokentype key2tok(char *fplex)
{
    for (int i = 0; keywordtab[i].token != nfound; i++)
    {
        if (!strcmp(fplex, keywordtab[i].text))
        {
            return keywordtab[i].token;
        }
    }

    return TOK_WORD;
}

char *tok2lex(tokentype ftok)
{
    for (int i = 0; tokentab[i].token != nfound; i++)
    {
        if (tokentab[i].token == ftok)
        {
            return (char *)tokentab[i].text;
        }
    }

    return (char *)"undef";
}

bool is_keyword(char *lexeme)
{
    for (int i = 0; keywordtab[i].token != nfound; i++)
    {
        if (!strcmp(lexeme, keywordtab[i].text))
        {
            return true;
        }
    }

    return false;
}
