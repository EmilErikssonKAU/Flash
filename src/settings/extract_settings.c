#include "extract_settings.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_FIELD "PS1_COLOR"
#define PROMPT_FIELD "PS1_TEXT"

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

Color get_color(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    char line[1024];
    size_t field_len = strlen(COLOR_FIELD);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        const char *cursor = line;
        while (*cursor != '\0' && isspace((unsigned char)*cursor))
        {
            cursor++;
        }

        if (strncmp(cursor, COLOR_FIELD, field_len) != 0)
        {
            continue;
        }

        cursor += field_len;
        while (*cursor != '\0' && isspace((unsigned char)*cursor))
        {
            cursor++;
        }

        if (*cursor != '=')
        {
            continue;
        }
        cursor++;

        while (*cursor != '\0' && isspace((unsigned char)*cursor))
        {
            cursor++;
        }

        size_t raw_len = strcspn(cursor, "\r\n");
        while (raw_len > 0 && isspace((unsigned char)cursor[raw_len - 1]))
        {
            raw_len--;
        }

        size_t start = 0;
        size_t end = raw_len;
        if (raw_len >= 2 &&
            ((cursor[0] == '"' && cursor[raw_len - 1] == '"') ||
             (cursor[0] == '\'' && cursor[raw_len - 1] == '\'')))
        {
            start = 1;
            end = raw_len - 1;
        }

        char value[64];
        size_t value_len = 0;
        for (size_t i = start; i < end; i++)
        {
            if (value_len + 1 < sizeof(value))
            {
                value[value_len++] = cursor[i];
            }
        }
        value[value_len] = '\0';

        fclose(file);
        return parse_color_value(value);
    }

    fclose(file);
    return DEFAULT;
}

char *get_prompt(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    char line[1024];
    size_t field_len = strlen(PROMPT_FIELD);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        const char *cursor = line;
        while (*cursor != '\0' && isspace((unsigned char)*cursor))
        {
            cursor++;
        }

        if (strncmp(cursor, PROMPT_FIELD, field_len) != 0)
        {
            continue;
        }

        cursor += field_len;
        while (*cursor != '\0' && isspace((unsigned char)*cursor))
        {
            cursor++;
        }

        if (*cursor != '=')
        {
            continue;
        }
        cursor++;
        while (*cursor != '\0' && isspace((unsigned char)*cursor))
        {
            cursor++;
        }

        size_t text_len = strcspn(cursor, "\r\n");
        size_t start = 0;
        size_t end = text_len;

        if (text_len >= 2 && cursor[0] == '"' && cursor[text_len - 1] == '"')
        {
            start = 1;
            end = text_len - 1;
        }

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
        return prompt;
    }

    fclose(file);

    // Return "$ " by default
    return strdup("$ ");
}
