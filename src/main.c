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
#include "readline/readline_custom.h"
#include <readline/readline.h>

#define MAX_INPUT_LENGTH 100
#define BUF_SIZE 1024

void init()
{
  init_readline();
}

int main(int argc, char *argv[])
{
  init();
  setbuf(stdout, NULL); // Flush after every printf
  char *buf;

  // Read-Eval-Print Loop
  while (true)
  {
    buf = readline("$ ");
    if (buf == NULL)
    {
      break;
    }

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
      exit(1);
    }

    free_ast(asttree);
  }

  return 0;
}
