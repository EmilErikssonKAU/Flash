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
#define MAX_ARGS 20

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

void build_argv(char *lexemes[], char *argv[])
{
  int i = 0;
  while (i < MAX_ARGS - 1 && lexemes[i] != NULL)
  {
    argv[i] = lexemes[i];
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

  char *cmd_argv[MAX_ARGS];

  char *input_buffer[BUF_SIZE];
  char *output_buffer[BUF_SIZE];
  char *error_buffer[BUF_SIZE];

  char *lexemes[MAX_ARGS];
  int tokens[MAX_ARGS];

  // Read-Eval-Print Loop
  while (true)
  {
    printf("$ ");
    if (fgets(buf, MAX_INPUT_LENGTH, stdin) == NULL)
      break;
    buf[strlen(buf) - 1] = '\0'; // Remove newline

    fill_input_buffer(buf);
    int j = 0;
    tokens[j] = get_token();


    // LEXEMIZE THE INPUT
    while (j < MAX_ARGS - 2)
    {
      char *lex = get_lexeme();
      if (lex == NULL)
        break;
      lexemes[j] = strdup(lex);
      j++;
      tokens[j] = get_token();
    }
    lexemes[j] = NULL;

    // DETERMINE END OF COMMAND

    // PARSE ONE COMMAND

    // EXECUTE ON COMMAND

    // HANDLE REDIRECTS OF OUTPUT_BUFFER
    if (lexemes[0] == NULL || lexemes[0][0] == '\0')
    {
      continue;
    }

    if (!strcmp(lexemes[0], "exit"))
    {
      break;
    }

    else if (!strcmp(lexemes[0], "echo"))
    {
      int i = 1;
      while (lexemes[i] != NULL)
      {
        if (i > 1)
          printf(" ");
        printf("%s", lexemes[i]);
        i++;
      }
      printf("\n");
    }

    else if (!strcmp(lexemes[0], "type"))
    {
      if (lexemes[1] == NULL || lexemes[1][0] == '\0')
      {
        continue;
      }

      if (is_keyword(lexemes[1]))
      {
        printf("%s is a shell builtin\n", lexemes[1]);
      }

      else if (!checkPath(lexemes[1], true))
      {
        printf("%s: not found\n", lexemes[1]);
      }
    }

    else if (!strcmp(lexemes[0], "pwd"))
    {
      char cwd[BUF_SIZE];
      getcwd(cwd, sizeof(cwd));
      printf("%s\n", cwd);
    }

    else if (!strcmp(lexemes[0], "cd"))
    {

      if (lexemes[1] == NULL || lexemes[1][0] == '\0')
      {
        continue;
      }

      else
      {
        char *expanded_path = expand_path(lexemes[1]);

        if (chdir(expanded_path))
        {
          printf("cd: %s: No such file or directory\n", lexemes[1]);
        }

        free(expanded_path);
      }
    }

    else if (checkPath(lexemes[0], false))
    {
      build_argv(lexemes, cmd_argv);
      execute_program(cmd_argv);
    }

    else
    {
      printf("%s: command not found\n", buf);
    }

    // Clear lexemes, and free memory
    for (int i = 0; i < j; i++)
    {
      free(lexemes[i]);
    }
    memset(lexemes, 0, sizeof(lexemes));
  }

  return 0;
}
