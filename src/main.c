#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "lexer.h"
#include "tokentable.h"
#include "parser.h"

#define MAX_INPUT_LENGTH 100
#define BUF_SIZE 1024

char *extractPath(const char *PATH)
{
  char *buf;
  if (PATH != NULL)
    buf = strdup(PATH);
  else
    buf = NULL;
  char *path_section = strtok(buf, ":");
  return path_section;
}

bool checkPath(const char *lexeme, bool should_print)
{
  char *PATH = getenv("PATH");
  char *path_section = extractPath(PATH);

  while (path_section != NULL)
  {
    char buffer[BUF_SIZE];
    snprintf(buffer, sizeof(buffer), "%s/%s", path_section, lexeme);

    if (!access(buffer, X_OK))
    {
      if (should_print)
        printf("%s is %s\n", lexeme, buffer);
      return true;
    }
    path_section = extractPath(NULL);
  }
  return false;
}

char *expand_path(const char *path)
{
  if (!path)
    return NULL;

  if (path[0] != '~')
  {
    char *expansion = malloc(strlen(path) + 1);
    memcpy(expansion, path, strlen(path) + 1);
    return expansion;
  }

  const char *home = getenv("HOME");

  size_t expanded_path_len = strlen(home) + strlen(path) + 1;
  char *expanded_path = malloc(expanded_path_len);
  snprintf(expanded_path, expanded_path_len, "%s%s", home, path + 1);
  return expanded_path;
}

void execute_program(char *argv[])
{
  pid_t pid = fork();

  if (pid == 0) // child process
  {
    execvp(argv[0], argv);
    exit(1);
  }
  else // parent process
  {
    wait(NULL);
  }
}

int main(int argc, char *argv[])
{
  setbuf(stdout, NULL); // Flush after every printf
  char buf[BUF_SIZE];

  // Read-Eval-Print Loop
  while (true)
  {
    printf("$ ");
    if (fgets(buf, MAX_INPUT_LENGTH, stdin) == NULL)
      break;
    buf[strlen(buf) - 1] = '\0'; // Remove newline

    fill_input_buffer(buf);

    AstNode *asttree = parse_input();

    if (asttree == NULL || asttree->cmd == NULL || asttree->cmd->argv.v == NULL || asttree->cmd->argv.n == 0 || asttree->cmd->argv.v[0] == NULL || asttree->cmd->argv.v[0][0] == '\0')
    {
      free_ast(asttree);
      continue;
    }

    char **cmd_argv = asttree->cmd->argv.v;
    bool should_exit = false;

    if (!strcmp(cmd_argv[0], "exit"))
    {
      should_exit = true;
    }

    else if (!strcmp(cmd_argv[0], "echo"))
    {
      int i = 1;
      while (cmd_argv[i] != NULL)
      {
        if (i > 1)
          printf(" ");
        printf("%s", cmd_argv[i]);
        i++;
      }
      printf("\n");
    }

    else if (!strcmp(cmd_argv[0], "type"))
    {
      if (cmd_argv[1] == NULL || cmd_argv[1][0] == '\0')
      {
        free_ast(asttree);
        continue;
      }

      if (is_keyword(cmd_argv[1]))
      {
        printf("%s is a shell builtin\n", cmd_argv[1]);
      }

      else if (!checkPath(cmd_argv[1], true))
      {
        printf("%s: not found\n", cmd_argv[1]);
      }
    }

    else if (!strcmp(cmd_argv[0], "pwd"))
    {
      char cwd[BUF_SIZE];
      getcwd(cwd, sizeof(cwd));
      printf("%s\n", cwd);
    }

    else if (!strcmp(cmd_argv[0], "cd"))
    {

      if (cmd_argv[1] == NULL || cmd_argv[1][0] == '\0')
      {
        free_ast(asttree);
        continue;
      }

      else
      {
        char *expanded_path = expand_path(cmd_argv[1]);

        if (chdir(expanded_path))
        {
          printf("cd: %s: No such file or directory\n", cmd_argv[1]);
        }

        free(expanded_path);
      }
    }

    else if (checkPath(cmd_argv[0], false))
    {
      execute_program(cmd_argv);
    }

    else
    {
      printf("%s: command not found\n", cmd_argv[0]);
    }

    free_ast(asttree);
    if (should_exit)
      break;
  }

  return 0;
}
