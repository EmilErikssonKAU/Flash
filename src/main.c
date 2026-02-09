#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT_LENGTH 100

int main(int argc, char *argv[])
{
  setbuf(stdout, NULL); // Flush after every printf
  char buf[1024];

  // Read-Eval-Print Loop
  while (true)
  {
    printf("$ ");
    fgets(buf, MAX_INPUT_LENGTH, stdin);
    buf[strlen(buf) - 1] = '\0'; // Remove newline
    printf("%s: command not found\n", buf);
  }

  return 0;
}
