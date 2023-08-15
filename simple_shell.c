#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "simple_shell.h"
#define BUFFER_SIZE 1024
#define PROMPT "#cisfun$ "
void execute_command(char *command)
{
    char *argv[2];
    argv[0] = command;
    argv[1] = NULL;
    if (strcmp(command, "\n") == 0)
    {
        return;
    }
    command[strcspn(command, "\n")] = 0;
    if (execve(argv[0], argv, NULL) == -1)
    {
        perror("./shell");
    }
}
int main()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while (1)
    {
        printf(PROMPT);
        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        pid_t pid, wpid;
        int status;
        if ((pid = fork()) == 0)
        {
            execute_command(line);
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("shell");
        }
        else
        {
            do
            {
                wpid = waitpid(pid, &status, WUNTRACED);
            }
            while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    free(line);
    return 0;
}
