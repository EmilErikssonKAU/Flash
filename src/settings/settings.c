#include "settings.h"
#include "extract_settings.h"
#include "../exec/path.h"
#include <stdlib.h>
#include <string.h>

#define FILENAME ".flashrc"
#define DEFAULT_DIR "~"
#define PATH_MAX 1000
#define DEFAULT_PROMPT "$ "

static const char *color_to_ansi(Color color)
{
    switch (color)
    {
    case GREEN:
        return "\033[32m";
    case RED:
        return "\033[31m";
    case BLUE:
        return "\033[34m";
    default:
        return "\033[0m";
    }
}

static char *build_colored_prompt(const Settings *settings)
{

    if (settings->prompt_color == DEFAULT)
    {
        return strdup(settings->prompt_text);
    }

    const char *ansi = color_to_ansi(settings->prompt_color);
    const char *reset = "\033[0m";
    const char *text = settings->prompt_text;

    size_t needed = 0;
    needed += 1 + strlen(ansi) + 1; // \001 + ansi + \002
    needed += strlen(text);
    needed += 1 + strlen(reset) + 1; // \001 + reset + \002
    needed += 1;                     // NUL

    char *prompt = malloc(needed);
    if (prompt == NULL)
    {
        return NULL;
    }

    // \001 %s \002 indicates start and end of characters that takes up no physical screen space
    // Very unclear if I need this but it stays for now

    snprintf(prompt, needed, "\001%s\002%s\001%s\002", ansi, text, reset);
    return prompt;
}

static Settings default_settings(void)
{
    Settings settings;
    settings.prompt_color = DEFAULT;
    settings.prompt_text = strdup(DEFAULT_PROMPT);
    return settings;
}

Settings init_settings(void)
{
    Settings settings = default_settings();

    char flashrc_path[PATH_MAX];
    snprintf(flashrc_path, sizeof(flashrc_path), "%s/%s", DEFAULT_DIR, FILENAME);
    char *expanded_path = expand_path(flashrc_path);
    if (expanded_path == NULL)
    {
        return settings;
    }

    if (access(expanded_path, R_OK) != 0)
    {
        free(expanded_path);
        return settings;
    }

    settings.prompt_color = get_color(expanded_path);
    char *prompt = get_prompt(expanded_path);
    if (prompt != NULL)
    {
        free(settings.prompt_text);
        settings.prompt_text = prompt;
    }

    char *colored_prompt = build_colored_prompt(&settings);
    if (colored_prompt != NULL)
    {
        free(settings.prompt_text);
        settings.prompt_text = colored_prompt;
    }

    free(expanded_path);
    return settings;
}
