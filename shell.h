#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_ARGS_SIZE 8
#define ARG_SEPERATOR " \t\r\n\a"

void get_input(char **line);
void parse_line(char **line);

#endif
