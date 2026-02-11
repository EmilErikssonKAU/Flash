#pragma once

#define nfound -1

typedef enum tokenvalues
{
   tstart = 257,
   undef,
   id,
   number,
   exit,
   echo
} tokentype;

tokentype lex2tok(char *fplex); /* convert a lexeme  to a token    */
tokentype key2tok(char *fplex); /* convert a keyword to a token    */
char *tok2lex(tokentype ftok);  /* convert a token   to a lexeme   */
