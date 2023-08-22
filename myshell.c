#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
    args[0] = "/bin/";
    args[1] = command;
    args[2] = NULL;

    pid = fork();

    if (pid == 0) {
        if (execv(args[0], args) == -1) {
            perror("./hsh");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("./hsh");
    } else {
        while (!WIFEXITED(status) && !WIFSIGNALED(status)) {
            waitpid(pid, &status, WUNTRACED);
        }

        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE) {
            fprintf(stderr, "./hsh: 1: %s: not found\n", command);
            exit(127);
        }
    }
}

int main(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while (1) {
        printf("#cisfun$ ");
        nread = getline(&line, &len, stdin);

        if (nread == -1) {
            free(line);
            break;
        }

        line[nread - 1] = '\0';
        execute_command(line);
    }

    return 0;
}

