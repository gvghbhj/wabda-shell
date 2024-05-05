#include "shell.h"

int main(void)
{
  char *line = NULL;

  while(1)
  {
    printf("$ ");
    get_input(&line);
    printf("%s\n", line);
    free(line);
    line = NULL;
  }
}

void get_input(char **line)
{
  size_t bufsize = 0;

  if (getline(line, &bufsize, stdin) == -1) 
  {
      if (feof(stdin)) 
      {
        exit(EXIT_SUCCESS);
      } 
      else
      {
        perror("readline");
        exit(EXIT_FAILURE);
      }
  }
}
