all: shell

clean:
	rm -vf shell.o

# _BSD_SOURCE is needed for strsep; _POSIX_C_SOURCE >= 200809L for strndup.
CFLAGS := -Wall -Wextra -std=gnu11 -pedantic 
LDFLAGS=-lreadline

shell: shell.o
shell.o: shell.c shell.h
