#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_COMMAND_LENGTH 256
extern char **environ;
typedef struct shell_session {
    int command_count;
} shell_session_t;
void execute_command(char *command, shell_session_t *session);
#endif
