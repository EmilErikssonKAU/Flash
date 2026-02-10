#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT_LENGTH 100
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
  setbuf(stdout, NULL); // Flush after every printf
  char buf[BUF_SIZE];

  // Read-Eval-Print Loop
  while (true)
  {
    printf("$ ");
    fgets(buf, MAX_INPUT_LENGTH, stdin);
    buf[strlen(buf) - 1] = '\0'; // Remove newline
    if (!strcmp(buf, "exit"))
      break;
    else if (!strncmp(buf, "echo", (sizeof("echo") - 1) * sizeof(char)))
    {
      printf("%s\n", buf + 5);
    }
    else
    {
      printf("%s: command not found\n", buf);
    }
  }

  return 0;
}
