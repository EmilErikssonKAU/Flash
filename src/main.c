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
#include "readline/readline.h"

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
    buf = readline("$");
    fill_input_buffer(buf);

    AstNode *asttree = parse_input();

    if (asttree == NULL || asttree->cmd == NULL || asttree->cmd->argv.v == NULL || asttree->cmd->argv.n == 0 || asttree->cmd->argv.v[0] == NULL || asttree->cmd->argv.v[0][0] == '\0')
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
