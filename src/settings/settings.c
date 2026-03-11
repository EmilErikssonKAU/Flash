#include "settings.h"
#include "extract_settings.h"
#include "../exec/path.h"
#include <stdlib.h>
#include <string.h>

#define FILENAME ".flashrc"
#define DEFAULT_DIR "~"
#define PATH_MAX 1000
#define DEFAULT_PROMPT "$ "

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

    free(expanded_path);
    return settings;
}
