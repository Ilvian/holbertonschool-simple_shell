#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

char *trim_whitespace(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) str++;
    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';

    return str;
}

void execute_command(char *command) {
    pid_t pid;
    int status;
    char *args[3];

    command = trim_whitespace(command);

    if (strcmp(command, "ls") == 0) {
        clearenv();
        args[0] = "/bin/ls";
        args[1] = NULL;
    } else {
        args[0] = "/bin/";
        args[1] = command;
        args[2] = NULL;
    }

    pid = fork();

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("./hsh");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("./hsh");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE) {
            fprintf(stderr, "./hsh: 1: %s: not found\n", command);
            exit(127);
        }
    }
}

int main(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (1) {
        if (isatty(STDIN_FILENO)) {
            printf("#cisfun$ ");
        }
        
        read = getline(&line, &len, stdin);
        if (read == -1)
            break;

        execute_command(line);
    }

    free(line);
    return 0;
}

