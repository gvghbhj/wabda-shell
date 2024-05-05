#include "shell.h"

char **args = NULL;
int num_args = 0;

int main(void)
{

  char *home_dir = getenv("HOME");
  char *line = NULL;
  char cwd[MAX_CWD_SIZE];

  while(1)
  {
    if (getcwd(cwd, MAX_CWD_SIZE) == NULL)
    {
      if (errno == ERANGE)
        printf("[ERROR] cwdname length exceeds the buffer size\n");
      else
        perror("getcwd");
      exit(EXIT_FAILURE);
    }
    printf("%s\n", cwd);
    printf("$ ");

    get_input(&line);
    parse_line(&line);

    if (strcmp(args[0], "exit") == 0)
    {
      exit_shell();
    }

    if (strcmp(args[0], "cd") == 0) 
    {
      if (num_args == 1 || args[1] == NULL)
      {
        change_cwd(&home_dir);
      }
      else 
      {
        change_cwd(&args[1]);
      }
    }

    else 
    {
      execute_command();
    }

    free(line);
    line = NULL;
    free(args);
    printf("\n");
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


void change_cwd(char **destination)
{
  if (chdir(*destination) < 0)
  {
    printf("ERROR: The directory does not exist");
  }
}

void execute_command(void)
{
  pid_t wpid;
  int status;
  pid_t pid = fork();
  
  if (pid == 0)
  {
    if(execvp(args[0], args) == -1)
    {
      perror("SHELL ERROR: PID");
    }
    exit(EXIT_FAILURE);
  }

  else if (pid < 0)
  {
    perror("SHELL ERROR: PID");
    exit(EXIT_FAILURE);
  }

  else 
  {
    do 
    {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
}


