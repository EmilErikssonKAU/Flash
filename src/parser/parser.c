#include "parser.h"
#include "lexer/tokentable.h"
#include "ast/ast.h"
#include "lexer/lexer.h"
#include <stdlib.h>
#include <string.h>

#define MAX_CMD_LENGTH 100

AstNode *parse_input();
static void match(TokKind token);
static bool is_semicolon_token(const Token *tok);
static bool is_and_operator_token(const Token *tok);
static bool is_or_operator_token(const Token *tok);
static AstCommand *parse_command_line();
static StrVec parse_command();
static RedirVec parse_redirect_list();
static Redir parse_redirect();
static AstList *parse_list();
static AstAndOr *parse_and_or();
static AstPipeline *parse_pipeline();

static Token lookahead;

static bool is_semicolon_token(const Token *tok)
{
    return tok->kind == TOK_WORD && tok->len == 1 && tok->lexeme[0] == ';';
}

static bool is_and_operator_token(const Token *tok)
{
    return tok->kind == TOK_WORD && tok->len == 2 &&
           tok->lexeme[0] == '&' && tok->lexeme[1] == '&';
}

static bool is_or_operator_token(const Token *tok)
{
    return tok->kind == TOK_WORD && tok->len == 2 &&
           tok->lexeme[0] == '|' && tok->lexeme[1] == '|';
}

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
    asttree->pipeline = parse_pipeline();
    asttree->nodetype = CommandNode;
    return asttree;
}

static AstList *parse_list()
{
    AstList *list = malloc(sizeof(AstList));
    list->items = NULL;
    list->n = 0;

    AstAndOr *first = parse_and_or();
    if (first != NULL)
    {
        list->items = malloc(sizeof(AstAndOr));
        if (list->items != NULL)
        {
            list->items[0] = *first;
            list->n = 1;
        }
        free(first);
    }

    while (is_semicolon_token(&lookahead))
    {
        match(TOK_WORD); // ';' currently tokenized as WORD by lexer.

        AstAndOr *next = parse_and_or();
        if (next == NULL)
        {
            break;
        }

        AstAndOr *tmp = realloc(list->items, (list->n + 1) * sizeof(AstAndOr));
        if (tmp != NULL)
        {
            list->items = tmp;
            list->items[list->n] = *next;
            list->n++;
        }
        free(next);
    }

    return list;
}

static AstAndOr *parse_and_or()
{
    AstAndOr *and_or = malloc(sizeof(AstAndOr));
    and_or->links = NULL;
    and_or->n = 0;

    AstPipeline *first = parse_pipeline();
    // Careful
    if (first != NULL)
    {
        and_or->first = *first;
        free(first);
    }
    // Seldom invoked
    else
    {
        and_or->first.commands = NULL;
        and_or->first.n = 0;
    }

    while (is_and_operator_token(&lookahead) || is_or_operator_token(&lookahead))
    {
        LogicOp op = is_and_operator_token(&lookahead) ? LOGIC_AND : LOGIC_OR;
        match(TOK_WORD); // '&&' / '||' currently tokenized as WORD by lexer.

        AstPipeline *next_pipeline = parse_pipeline();

        // no command
        if (next_pipeline == NULL)
        {
            break;
        }

        AstAndOrLink *tmp = realloc(and_or->links, (and_or->n + 1) * sizeof(AstAndOrLink));
        if (tmp != NULL)
        {
            and_or->links = tmp;
            and_or->links[and_or->n].op = op;
            and_or->links[and_or->n].pipeline = next_pipeline;
            and_or->n++;
        }
    }

    return and_or;
}

static AstPipeline *parse_pipeline()
{
    AstPipeline *pipeline = malloc(sizeof(AstPipeline));
    pipeline->commands = NULL;
    pipeline->n = 0;

    AstCommand *commandnode = parse_command_line();
    if (commandnode != NULL)
    {
        pipeline->commands = malloc(sizeof(AstCommand));
        if (pipeline->commands != NULL)
        {
            pipeline->commands[0] = *commandnode;
            pipeline->n = 1;
        }
        free(commandnode);
    }

    while (isPipe(lookahead.kind))
    {
        match(TOK_PIPELINE);

        AstCommand *next = parse_command_line();
        if (next == NULL)
        {
            break;
        }

        AstCommand *tmp = realloc(pipeline->commands, (pipeline->n + 1) * sizeof(AstCommand));
        if (tmp != NULL)
        {
            pipeline->commands = tmp;
            pipeline->commands[pipeline->n] = *next;
            pipeline->n++;
        }
        free(next);
    }

    return pipeline;
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
        commandnode->redirs.n = 0;
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
    else if (lookahead.kind == TOK_APPEND_STDOUT)
    {
        redir.kind = APPEND_STDOUT;
        match(TOK_APPEND_STDOUT);
    }
    else if (lookahead.kind == TOK_APPEND_FD1)
    {
        redir.kind = APPEND_FD1;
        match(TOK_APPEND_FD1);
    }
    else if (lookahead.kind == TOK_APPEND_FD2)
    {
        redir.kind = APPEND_FD2;
        match(TOK_APPEND_FD2);
    }
    redir.target = strndup(lookahead.lexeme, lookahead.len);
    match(TOK_WORD);

    return redir;
}
