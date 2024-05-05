#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>

#define INIT_ARGS_SIZE 8
#define ARG_SEPERATOR " \t\r\n\a"
#define MAX_CWD_SIZE 512

void get_input(char **line);
void parse_line(char **line);
void exit_shell(void);
void change_cwd(char **destination);
void execute_command(void);

#endif
