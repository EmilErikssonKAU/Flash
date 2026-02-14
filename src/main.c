#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "lexer.h"
#include "tokentable.h"

#define MAX_INPUT_LENGTH 100
#define BUF_SIZE 1024
#define MAX_ARGS 10

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

void build_argv(char *prog_name, char *argv[])
{

  char *arg = prog_name;
  int i = 0;
  while (arg != NULL && i < MAX_ARGS - 2)
  {
    argv[i] = strdup(arg);
    get_token();
    arg = get_lexeme();
    i++;
  }
  argv[i] = NULL;
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

  char *first_lex;
  char *second_lex;
  char *cmd_argv[MAX_ARGS];

  // Read-Eval-Print Loop
  while (true)
  {
    printf("$ ");
    if (fgets(buf, MAX_INPUT_LENGTH, stdin) == NULL)
      break;
    buf[strlen(buf) - 1] = '\0'; // Remove newline

    fill_input_buffer(buf);

    get_token();
    first_lex = get_lexeme();

    if (first_lex == NULL || first_lex[0] == '\0')
    {
      continue;
    }

    if (!strcmp(first_lex, "exit"))
    {
      break;
    }
    else if (!strcmp(first_lex, "echo"))
    {
      printf("%s\n", get_rest_of_input_buffer());
    }
    else if (!strcmp(first_lex, "type"))
    {
      get_token();
      second_lex = get_lexeme();

      if (second_lex == NULL || second_lex[0] == '\0')
      {
        continue;
      }

      if (is_keyword(second_lex))
      {
        printf("%s is a shell builtin\n", second_lex);
      }
      else if (!checkPath(second_lex, true))
      {
        printf("%s: not found\n", second_lex);
      }
    }
    else if (!strcmp(first_lex, "pwd"))
    {
      char cwd[BUF_SIZE];
      getcwd(cwd, sizeof(cwd));
      printf("%s\n", cwd);
    }
    else if (!strcmp(first_lex, "cd"))
    {
      get_token();
      second_lex = get_lexeme();

      if (second_lex == NULL || second_lex[0] == '\0')
      {
        continue;
      }
      else
      {
        char *expanded_path = expand_path(second_lex);
        if (chdir(expanded_path))
        {
          printf("cd: %s: No such file or directory\n", second_lex);
        }
        free(expanded_path);
      }
    }
    else if (checkPath(first_lex, false))
    {
      build_argv(first_lex, cmd_argv);
      execute_program(cmd_argv);
    }
    else
    {
      printf("%s: command not found\n", buf);
    }
  }

  return 0;
}
