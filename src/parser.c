#include "parser.h"
#include "tokentable.h"
#include "ast.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>

#define MAX_CMD_LENGTH 100

AstNode *parse_input();
static void match(TokKind token);
static AstCommand *parse_command_line();
static StrVec parse_command();
static RedirVec parse_redirect_list();
static Redir parse_redirect();

static Token lookahead;

static void match(TokKind token)
{
    if (lookahead.kind != token)
    {
        ; // panic?
    }
    else
    {
        lookahead = get_token();
    }
}

AstNode *parse_input()
{
    lookahead = get_token();
    if (lookahead.kind == TOK_NL)
    {
        match(TOK_NL);
        return NULL;
    }
    else if (lookahead.kind == TOK_EOF)
    {
        match(TOK_EOF);
        return NULL;
    }

    AstNode *asttree = malloc(sizeof(AstNode));
    asttree->cmd = parse_command_line();
    return asttree;
}

static AstCommand *parse_command_line()
{
    AstCommand *commandnode = malloc(sizeof(AstCommand));

    StrVec argvec = parse_command();
    commandnode->argv = argvec;

    if (isRedirect(lookahead.kind))
    {
        RedirVec redirvec = parse_redirect_list();
        commandnode->redirs = redirvec;
    }
    else
    {
        commandnode->redirs.v = NULL;
        commandnode->redirs.v = 0;
    }

    return commandnode;
}

static StrVec parse_command()
{
    StrVec argvec;
    argvec.v = malloc(sizeof(char *));

    int i = 0;
    while (lookahead.kind == TOK_WORD && i < MAX_CMD_LENGTH)
    {
        char **tmp = realloc(argvec.v, (i + 2) * sizeof(char *));
        if (!tmp)
        {
            ; // panic more?
        }
        else
        {
            argvec.v = tmp;
        }

        argvec.v[i] = strndup(lookahead.lexeme, lookahead.len);
        match(TOK_WORD);
        i++;
    }

    // NULL terminate argv
    argvec.v[i] = NULL;
    argvec.n = i;
    return argvec;
}

static RedirVec parse_redirect_list()
{
    RedirVec redirvec;
    redirvec.v = malloc(sizeof(Redir));

    int i = 0;
    while (isRedirect(lookahead.kind) && i < MAX_CMD_LENGTH)
    {
        Redir *tmp = realloc(redirvec.v, (i + 2) * sizeof(Redir));
        if (!tmp)
        {
            ; // panic more?
        }
        else
        {
            redirvec.v = tmp;
        }
        redirvec.v[i] = parse_redirect();
        i++;
    }

    redirvec.n = i;
    return redirvec;
}

static Redir parse_redirect()
{
    Redir redir;

    if (lookahead.kind == TOK_REDIR_STDOUT)
    {
        redir.kind = REDIR_STDOUT;
        match(TOK_REDIR_STDOUT);
    }
    else if (lookahead.kind == TOK_REDIR_FD1)
    {
        redir.kind = REDIR_FD1;
        match(TOK_REDIR_FD1);
    }
    else if (lookahead.kind == TOK_REDIR_FD2)
    {
        redir.kind = REDIR_FD2;
        match(TOK_REDIR_FD2);
    }
    redir.target = strndup(lookahead.lexeme, lookahead.len);
    match(TOK_WORD);

    return redir;
}