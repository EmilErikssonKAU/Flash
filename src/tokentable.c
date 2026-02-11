#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "tokentable.h"

typedef struct tab
{
    char *text;
    int token;
} tab;

static tab tokentab[] = {
    {"id", id},
    {"undef", undef},
    {"TERROR", nfound}};

static tab keywordtab[] = {
    {"exit", exit_tok},
    {"echo", echo},
    {"type", type},
    {"KERROR", nfound}};

tokentype lex2tok(char *fplex)
{
    // Special case
    if (!strcmp(fplex, "number"))
    {
        return id;
    }

    // Search through tokentab
    int i = 0;
    while (tokentab[i].token != nfound)
    {
        if (!strcmp(fplex, tokentab[i].text))
            return tokentab[i].token;
        i++;
    }

    return key2tok(fplex);
}

tokentype key2tok(char *fplex)
{
    // Search through keyword tab
    int i = 0;
    while (keywordtab[i].token != nfound)
    {
        if (!strcmp(fplex, keywordtab[i].text))
            return keywordtab[i].token;
        i++;
    }

    return id;
}

char *tok2lex(tokentype ftok)
{
    char *lexeme = tokentab[1].text;

    // iterate  through all tokens
    for (int i = 0; i < 21; i++)
    {
        if (tokentab[i].token == ftok)
        {
            lexeme = tokentab[i].text;
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (keywordtab[i].token == ftok)
        {
            lexeme = keywordtab[i].text;
        }
    }

    return lexeme;
}

bool is_keyword(char *lexeme)
{
    for (size_t i = 0; i < sizeof(keywordtab) / sizeof(tab); i++)
    {
        if (!strcmp(lexeme, keywordtab[i].text))
        {
            return true;
        }
    }

    return false;
}
