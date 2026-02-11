#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lexer.h"
#include "tokentable.h"

#define MAX_INPUT_LENGTH 100
#define BUF_SIZE 1024

// int main(int argc, char *argv[])
// {
//   setbuf(stdout, NULL); // Flush after every printf
//   char buf[BUF_SIZE];

//   // Read-Eval-Print Loop
//   while (true)
//   {
//     printf("$ ");
//     fgets(buf, MAX_INPUT_LENGTH, stdin);
//     buf[strlen(buf) - 1] = '\0'; // Remove newline
//     if (!strcmp(buf, "exit"))
//       break;
//     else if (!strncmp(buf, "echo", (sizeof("echo") - 1) * sizeof(char)))
//     {
//       printf("%s\n", buf + 5);
//     }
//     else
//     {
//       printf("%s: command not found\n", buf);
//     }
//   }

//   return 0;
// }

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
      else
      {
        printf("invalid_command: not found\n");
      }
    }
    else
    {
      printf("%s: command not found\n", buf);
    }
  }

  return 0;
}
