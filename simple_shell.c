#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 1024

/**
 * main - Entry point for the simple shell
 *
 * Return: 0 on success, or exit status of last command executed
 */
int main(void)
{
    char *line = NULL;
    size_t line_size = 0;
    ssize_t nread;
    char *args[2];
    pid_t child_pid;
    int status;

    while (1)
    {
        printf("#cisfun$ ");
        fflush(stdout);

        nread = getline(&line, &line_size, stdin);
        if (nread == -1)
        {
            if (line)
                free(line);
            printf("\n");
            exit(EXIT_SUCCESS);
        }

        line[strcspn(line, "\n")] = '\0';

        args[0] = line;
        args[1] = NULL;

        child_pid = fork();

        if (child_pid == -1)
        {
            perror("Error:");
            continue;
        }

        if (child_pid == 0)  
        {
            if (execve(args[0], args, NULL) == -1)
            {
                perror("./simple_shell");
            }
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(&status);
        }
    }
    return 0;
}

