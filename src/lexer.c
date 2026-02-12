#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "tokentable.h"

#define BUFSIZE 1024
#define LEXSIZE 30

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

static void next()
{
    if (lexeme_pos < LEXSIZE - 1)
    {
        lexeme_buffer[lexeme_pos] = peak();
    }
    input_pos++;
    lexeme_pos++;
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
            next();
        }
        memset(lexeme_buffer, 0, sizeof(lexeme_buffer));
        lexeme_pos = 0;
    }

    // Read digit
    // if (isdigit(peak()))
    // {
    //     while (isdigit(peak()))
    //     {
    //         next();
    //     }
    //     return number;
    // }

    // Read keyword or id

    while (!isspace(peak()) && peak() != '\0')
    {
        next();
    }
    return lex2tok(lexeme_buffer);

    // Read other characters
    // else
    // {
    //     // Special case: 'character containing two characters'
    //     if (peak() == ':' && input_buffer[input_pos + 1] == '=')
    //         next();
    //     next();

    //     tokentype token = lex2tok(lexeme_buffer);

    //     // if token comes back as id: it is undefined
    //     // else: defined special character
    //     if (token == id)
    //         return undef;
    //     else
    //         return token;
    // }
}

char *get_lexeme()
{
    if (lexeme_buffer[0] == '\0')
        return NULL;
    else
        return lexeme_buffer;
}
