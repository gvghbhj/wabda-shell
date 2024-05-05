#include "shell.h"

char **args = NULL;
int num_args = 0;

int main(void)
{
  char *line = NULL;
  char *cwd;
  while(1)
  {
    printf("$ ");
    get_input(&line);
    parse_line(&line);
    for (int i = 0 ; i < num_args; i++)
    {
      if (strcmp(args[i], "exit") == 0)
      {
        exit_shell();
      }
      printf("%s\n", args[i]);
    }
    free(line);
    line = NULL;
    free(args);
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

void parse_line(char **line)
{
  num_args = 0;
  static int args_max_size = INIT_ARGS_SIZE;
  args = malloc(sizeof(char *) * args_max_size);
  
  if (args == NULL)
  {
    printf("MEMORY ALLOCATION ERROR\n");
    exit(EXIT_FAILURE);
  }

  args[num_args] = strtok(*line, ARG_SEPERATOR);

  while(args[num_args] != NULL)
  {
    num_args++;

    if (num_args == args_max_size) 
    {
      args_max_size += INIT_ARGS_SIZE;
      args = realloc(args, sizeof(char *) * args_max_size);
      if (args == NULL)
      {
        printf("MEMORY ALLOCATION ERROR\n");
        exit(EXIT_FAILURE);
      }
    }

    args[num_args] = strtok(NULL, ARG_SEPERATOR);
  }
}

void exit_shell(void)
{
  printf("byeeee!\n");
  exit(EXIT_SUCCESS);
}
