#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "tokentable.h"

#define BUFSIZE 1024
#define LEXSIZE 100

static char input_buffer[BUFSIZE];
static char lexeme_buffer[LEXSIZE];
static size_t input_pos = 0;  /* current index program input_buffer  */
static size_t lexeme_pos = 0; /* current index lexeme  input_buffer  */

bool fill_input_buffer(const char *input)
{
    int i = 0;
    while (i < BUFSIZE - 1 && input[i] != '\0')
    {
        input_buffer[i] = input[i];
        i++;
    }
    input_buffer[i] = '\0';
    input_pos = 0;
    return true;
}

static char peak()
{
    return input_buffer[input_pos];
}

static char double_peak()
{
    return input_buffer[input_pos + 1];
}

static char triple_peak()
{
    return input_buffer[input_pos + 2];
}

static void next()
{
    if (lexeme_pos < LEXSIZE - 1)
    {
        lexeme_buffer[lexeme_pos] = peak();
    }
    input_pos++;
    lexeme_pos++;
}

static void skip()
{
    if (input_pos < BUFSIZE - 1)
        input_pos++;
}

static bool is_operator_start()
{
    if (peak() == '>')
    {
        return true;
    }

    if ((peak() == '1' || peak() == '2') && double_peak() == '>')
    {
        return true;
    }

    return false;
}

static bool scan_operator(TokKind *kind)
{
    if ((peak() == '1' || peak() == '2') && double_peak() == '>' && triple_peak() == '>')
    {
        char fd = peak();
        next();
        next();
        next();
        *kind = (fd == '1') ? TOK_APPEND_FD1 : TOK_APPEND_FD2;
        return true;
    }

    if (peak() == '>' && double_peak() == '>')
    {
        next();
        next();
        *kind = TOK_APPEND_STDOUT;
        return true;
    }

    if ((peak() == '1' || peak() == '2') && double_peak() == '>')
    {
        char fd = peak();
        next();
        next();
        *kind = (fd == '1') ? TOK_REDIR_FD1 : TOK_REDIR_FD2;
        return true;
    }

    if (peak() == '>')
    {
        next();
        *kind = TOK_REDIR_STDOUT;
        return true;
    }

    return false;
}

static bool handle_singlequotes(void)
{
    skip();
    while (peak() != '\'' && peak() != '\0')
    {
        next();
    }
    if (peak() != '\'')
    {
        return false;
    }
    skip();
    return true;
}

static bool handle_doublequotes(void)
{
    skip();
    while (peak() != '\"' && peak() != '\0')
    {
        if (peak() == '\\')
        {
            if ((double_peak() == '\\') || (double_peak() == '"'))
            {
                skip();
            }
        }
        next();
    }
    if (peak() != '\"')
    {
        return false;
    }
    skip();
    return true;
}

const char *get_rest_of_input_buffer()
{
    return &input_buffer[input_pos + 1];
}

static Token make_token(TokKind kind)
{
    return (Token){
        .kind = kind,
        .lexeme = lexeme_buffer,
        .len = lexeme_pos,
    };
}

Token get_token(void)
{
    // TODO: Handle case when buffer is not filled

    // Empty the lexinput_buffer
    memset(lexeme_buffer, 0, sizeof(lexeme_buffer));
    lexeme_pos = 0;

    // Ignore whitespace except line feed, which is a token.
    while (peak() != '\0' && peak() != '\n' && isspace((unsigned char)peak()))
    {
        skip();
    }

    if (peak() == '\n')
    {
        skip();
        return make_token(TOK_NL);
    }

    if (peak() == '\0')
    {
        return make_token(TOK_EOF);
    }

    if (is_operator_start())
    {
        TokKind operator_kind;
        if (!scan_operator(&operator_kind))
        {
            return make_token(TOK_ERR);
        }
        return make_token(operator_kind);
    }

    // Read keyword or word
    while (!isspace(peak()) && peak() != '\0' && !is_operator_start())
    {
        if (peak() == '\\')
        {
            skip();
            next();
        }
        else if (peak() == '\"')
        {
            if (!handle_doublequotes())
                return make_token(TOK_ERR);
        }
        else if (peak() == '\'')
        {
            if (!handle_singlequotes())
                return make_token(TOK_ERR);
        }
        else
        {
            next();
        }
    }
    return make_token(TOK_WORD);
}

const char *get_lexeme(void)
{
    if (lexeme_buffer[0] == '\0')
        return NULL;
    else
        return lexeme_buffer;
}
