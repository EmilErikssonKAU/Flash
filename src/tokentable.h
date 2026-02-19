#pragma once
#include <stdbool.h>

#define nfound -1

typedef enum tokenvalues
{
   tstart = 257,
   TOK_UNDEF,
   TOK_WORD,
   TOK_NUMBER,
   TOK_EXIT,
   TOK_ECHO,
   TOK_TYPE,
   TOK_PWD,
   TOK_CD,
   TOK_REDIR_STDOUT,
   TOK_REDIR_STDOUT_FD1,
   TOK_REDIR_STDERR,
} tokentype;

tokentype lex2tok(char *fplex); /* convert a lexeme  to a token    */
tokentype key2tok(char *fplex); /* convert a keyword to a token    */
char *tok2lex(tokentype ftok);  /* convert a token   to a lexeme   */
bool is_keyword(char *lexeme);
