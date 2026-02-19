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
static bool hasRead = false;

bool fill_input_buffer(char *input)
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

static size_t scan_operator()
{
    if ((peak() == '1' || peak() == '2') && double_peak() == '>')
    {
        next();
        next();
        return 2;
    }

    if (peak() == '>')
    {
        next();
        return 1;
    }

    return 0;
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

char *get_rest_of_input_buffer()
{
    return &input_buffer[input_pos + 1];
}

int get_token()
{
    // TODO: Handle case when buffer is not filled

    // Empty the lexinput_buffer
    memset(lexeme_buffer, 0, sizeof(lexeme_buffer));
    lexeme_pos = 0;

    // Ignore whitespace
    if (isspace(peak()))
    {
        while (isspace(peak()))
        {
            skip();
        }
    }

    if (peak() == '\0')
    {
        return TOK_UNDEF;
    }

    if (is_operator_start())
    {
        if (scan_operator() == 0)
        {
            return TOK_UNDEF;
        }
        return lex2tok(lexeme_buffer);
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
                return TOK_UNDEF;
        }
        else if (peak() == '\'')
        {
            if (!handle_singlequotes())
                return TOK_UNDEF;
        }
        else
        {
            next();
        }
    }
    return lex2tok(lexeme_buffer);
}

char *get_lexeme()
{
    if (lexeme_buffer[0] == '\0')
        return NULL;
    else
        return lexeme_buffer;
}
