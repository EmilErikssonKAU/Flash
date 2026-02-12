#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "lexer.h"
#include "tokentable.h"

#define MAX_INPUT_LENGTH 100
#define BUF_SIZE 1024

char *extractPath(char *PATH)
{
  char *buf;
  if (PATH != NULL)
    buf = strdup(PATH);
  else
    buf = NULL;
  char *path_section = strtok(buf, ":");
  return path_section;
}

bool checkPath(char *lexeme)
{
  char *PATH = getenv("PATH");
  char *path_section = extractPath(PATH);
  while (path_section != NULL)
  {
    char buffer[BUF_SIZE];
    snprintf(buffer, sizeof(buffer), "%s/%s", path_section, lexeme);
    if (!access(buffer, X_OK))
    {
      printf("%s is %s\n", lexeme, buffer);
      return true;
    }
    path_section = extractPath(NULL);
  }
  return false;
}

int main(int argc, char *argv[])
{
  setbuf(stdout, NULL); // Flush after every printf
  char buf[BUF_SIZE];

  char *first_lex;
  char *second_lex;

  // Read-Eval-Print Loop
  while (true)
  {
    printf("$ ");
    fgets(buf, MAX_INPUT_LENGTH, stdin);
    buf[strlen(buf) - 1] = '\0'; // Remove newline

    fill_input_buffer(buf);

    get_token();
    first_lex = get_lexeme();

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
      if (is_keyword(second_lex))
      {
        printf("%s is a shell builtin\n", second_lex);
      }
      else if (!checkPath(second_lex))
      {
        printf("%s: not found\n", second_lex);
      }
    }
    else
    {
      printf("%s: command not found\n", buf);
    }
  }

  return 0;
}
