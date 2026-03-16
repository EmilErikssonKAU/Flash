#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "lexer/lexer.h"
#include "lexer/tokentable.h"
#include "parser/parser.h"
#include "exec/exec.h"
#include "exec/path.h"
#include "settings/settings.h"
#include "readline/readline_custom.h"
#include <readline/readline.h>

#define MAX_INPUT_LENGTH 100
#define BUF_SIZE 1024

static const char *clear_screen = "\033[2J\033[H";
static const char *clear_suggestion_line = "\033[2K";

Settings init()
{
  Settings settings = init_settings();
  init_readline();
  printf("%s", clear_screen); // Clear screen
  return settings;
}

int main(int argc, char *argv[])
{
  Settings settings = init();
  setbuf(stdout, NULL); // Flush after every printf
  char *buf;

  // Read-Eval-Print Loop
  while (true)
  {
    buf = readline(settings.prompt_text);

    if (buf == NULL)
    {
      break;
    }

    printf("%s", clear_suggestion_line); // Clear the suggestion line the cursor landed on after Enter

    fill_input_buffer(buf);
    free(buf);

    AstNode *asttree = parse_input();

    if (asttree == NULL)
    {
      free_ast(asttree);
      continue;
    }

    ExecResult exec_result = exec(asttree);

    if (exec_result.shouldExit)
    {
      free_ast(asttree);
      free(settings.prompt_text);
      exit(1);
    }

    free_ast(asttree);
  }

  free(settings.prompt_text);
  return 0;
}
