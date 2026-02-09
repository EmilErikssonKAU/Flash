#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 100

int main(int argc, char *argv[])
{
  // Flush after every printf
  setbuf(stdout, NULL);
  printf("$ ");

  char buf[1024];
  fgets(buf, MAX_INPUT_LENGTH, stdin);

  // Remove newline
  buf[strlen(buf) - 1] = '\0';
  printf("%s: command not found\n", buf);

  return 0;
}
