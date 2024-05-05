#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>

#define INIT_ARGS_SIZE 8
#define ARG_SEPERATOR " \t\r\n\a"
#define MAX_CWD_SIZE 512
#define ANSI_COLOR_RED  "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

void get_input(char **line);
void parse_line(char **line);
void exit_shell(void);
int change_cwd(char **destination);
int execute_command(void);
void INThandler(int signal);

#endif
