#pragma once

#include <stdio.h>
#include <unistd.h>

typedef enum
{
    GREEN,
    RED,
    BLUE,
    DEFAULT
} Color;

typedef struct
{
    Color prompt_color;
    char *prompt_text;
} Settings;

Settings init_settings();