#include "settings/extract_settings.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_FIELD "PS1_COLOR"
#define PROMPT_FIELD "PS1_TEXT"

void skip_leading_space(const char **cursor)
{
    while (**cursor != '\0' && isspace(**cursor))
    {
        (*cursor)++;
    }
}

int len_without_trailing_space(int raw_len, const char *cursor)
{
    while (raw_len > 0 && isspace(cursor[raw_len - 1]))
    {
        raw_len--;
    }

    return raw_len;
}

bool has_leading_trailing_char(const char *cursor, int len, char target)
{
    if ((cursor[0] == target && cursor[len - 1] == target))
    {
        return true;
    }

    return false;
}

// Prompt length, vital for cursor.c
static int prompt_length = 0;

int get_prompt_length()
{
    return prompt_length;
}

void set_prompt_length(int len)
{
    prompt_length = len;
}

static bool color_equals_ignore_case(const char *value, const char *target)
{
    if (value == NULL)
    {
        return false;
    }

    while (*value != '\0' && *target != '\0')
    {
        if (toupper((unsigned char)*value) != toupper((unsigned char)*target))
        {
            return false;
        }
        value++;
        target++;
    }

    return *value == '\0' && *target == '\0';
}

static Color parse_color_value(const char *value)
{
    if (color_equals_ignore_case(value, "GREEN"))
    {
        return GREEN;
    }
    if (color_equals_ignore_case(value, "RED"))
    {
        return RED;
    }
    if (color_equals_ignore_case(value, "BLUE"))
    {
        return BLUE;
    }

    return DEFAULT;
}

// Supports formats: KEY=VALUE, KEY='VALUE', KEY="VALUE"
Color get_color(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    char line[1024];
    size_t field_len = strlen(COLOR_FIELD);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        const char *cursor = line;
        skip_leading_space(&cursor);

        // Verify existence of color KEY
        if (strncmp(cursor, COLOR_FIELD, field_len) != 0)
        {
            continue;
        }

        cursor += field_len;
        skip_leading_space(&cursor);

        if (*cursor != '=')
        {
            continue;
        }
        cursor++;

        skip_leading_space(&cursor);

        // including carriage return \r might be overkill
        size_t raw_len = strcspn(cursor, "\r\n");
        raw_len = len_without_trailing_space(raw_len, cursor);

        // Currently allowing quotes around VALUE
        size_t start = 0;
        size_t end = raw_len;
        if (raw_len >= 2 && (has_leading_trailing_char(cursor, raw_len, '"') || has_leading_trailing_char(cursor, raw_len, '\'')))
        {
            start = 1;
            end = raw_len - 1;
        }

        // Will change if I find extremely long color
        int max_value_len = 64;
        char value[max_value_len];
        if (end - start >= max_value_len)
        {
            continue; // user invented som bs color
        }
        memcpy(value, cursor + start, end - start);
        value[end - start] = '\0';

        fclose(file);
        return parse_color_value(value);
    }

    fclose(file);
    return DEFAULT;
}

// Supports formats: KEY=VALUE, KEY='VALUE', KEY="VALUE"
char *get_prompt(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    char line[1024];
    size_t field_len = strlen(PROMPT_FIELD);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        const char *cursor = line;
        skip_leading_space(&cursor);

        if (strncmp(cursor, PROMPT_FIELD, field_len) != 0)
        {
            continue;
        }

        cursor += field_len;
        skip_leading_space(&cursor);

        if (*cursor != '=')
        {
            continue;
        }
        cursor++;
        skip_leading_space(&cursor);

        // including carriage return \r might be overkill
        size_t raw_len = strcspn(cursor, "\r\n");

        // NOTE: Allowing trailing space in prompt

        // Currently allowing quotes around VALUE
        size_t start = 0;
        size_t end = raw_len;
        if (raw_len >= 2 && (has_leading_trailing_char(cursor, raw_len, '"') || has_leading_trailing_char(cursor, raw_len, '\'')))
        {
            start = 1;
            end = raw_len - 1;
        }

        // Currently no limit on prompt lengt, might later enforce something
        size_t out_len = end - start;
        char *prompt = malloc(out_len + 1);

        if (prompt == NULL)
        {
            fclose(file);
            return NULL;
        }

        memcpy(prompt, cursor + start, out_len);
        prompt[out_len] = '\0';

        fclose(file);

        prompt_length = out_len;

        return prompt;
    }

    fclose(file);

    // Shouldn't trigger if checking file exists before function call
    return NULL;
}
