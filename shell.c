#include "shell.h"
#include <stdlib.h>

char **args = NULL; // Global arguments array
int num_args = 0; // number of args

int main(void)
{
  signal(SIGINT, INThandler); // handles ctrl+c signal

  char *home_dir = getenv("HOME"); // The users home directory
  char *line = NULL; // The actual command line given by the user
  char cwd[MAX_CWD_SIZE];
  int status = 0; // Status of child process 

  while(1)
  {
    if (getcwd(cwd, MAX_CWD_SIZE) == NULL) // gets the current working directory
    {
      if (errno == ERANGE)
        printf("[ERROR] cwdname length exceeds the buffer size\n");
      else
        perror("getcwd");
      exit(EXIT_FAILURE);
    }

    printf("%s\n", cwd);

    if (status == 0) // if command executed fine, then print normally, else we print a red $, to indicate that there was a problem in the last executed command.
    {
      printf("$ ");
    }

    else 
    {
      printf(ANSI_COLOR_RED "$ " ANSI_COLOR_RESET);
    }

    get_input(&line);

    if (strlen(line) == 1 || line[0] == '\n') // seeing if we got an empty line of input, is necessary, otherwise program crashes.
    {
      continue;
    }

    parse_line(&line);

    if (strcmp(args[0], "exit") == 0)
    {
      exit_shell();
    }

    if (strcmp(args[0], "cd") == 0) 
    {
      if (num_args == 1 || args[1] == NULL) // if only cd, is given then like most shells we will simply go to home directory.
      {
        status = change_cwd(&home_dir);
      }
      else 
      {
        status = change_cwd(&args[1]);
      }
    }

    else 
    {
      status = execute_command();
    }

    free(line);
    line = NULL;
    free(args);
    printf("\n");
  }
}

void get_input(char **line) // A function that gets input from user
{
  size_t bufsize = 0; // size of line

  if (getline(line, &bufsize, stdin) == -1) 
  {
    if (feof(stdin))  // checks for ctrl+d, indicating no more input
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

void parse_line(char **line) // Function that splits line into parameters.
{
  num_args = 0;
  static int args_max_size = INIT_ARGS_SIZE;
  args = malloc(sizeof(char *) * args_max_size);
  
  if (args == NULL)
  {
    printf("MEMORY ALLOCATION ERROR\n");
    exit(EXIT_FAILURE);
  }

  args[num_args] = strtok(*line, ARG_SEPERATOR); // check out man strtok, basically splits our line into different words.

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

    args[num_args] = strtok(NULL, ARG_SEPERATOR); // NULL is used because strtok has optimizations to remember the line variable that we give it for better performance and memory
  }
}

void exit_shell(void) // byeeee!
{
  printf("byeeee!\n");
  exit(EXIT_SUCCESS);
}


int change_cwd(char **destination) // cd functionality, because executing it as a normal command would mean changing directory of child process, cd has to be a shell builtin to change parent process directory.
{
  if (chdir(*destination) < 0)
  {
    printf("ERROR: The directory does not exist");
    return -1;
  }

  return 0;
}

int execute_command(void)
{
  pid_t wpid;
  int status; // status from wait
  pid_t pid = fork(); // new child!!
  
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

  // following gets the status from child, first if done to see if child process ended normally, second to see if it was because of something like kill.
  if (WIFEXITED(status))
  {
    status = WEXITSTATUS(status);
  }
  else if (WIFSIGNALED(status))
  {
    status = WTERMSIG(status);
  }

  return status;

}

void INThandler(int signal)
{
  return;
}
