#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT "#cisfun$ "
#define MAX_INPUT_LENGTH 1024

char *trim_whitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

int main(void) {
    char input[MAX_INPUT_LENGTH];
    char *trimmed_input;
    char *token;
    char *args[MAX_INPUT_LENGTH / 2];
    int i;
    pid_t pid, wpid;

    while (1) {
        if (isatty(STDIN_FILENO)) {
            printf(PROMPT);
            fflush(stdout);
        }

        if (fgets(input, sizeof(input), stdin) == NULL) {
            exit(0);
        }

        input[strcspn(input, "\n")] = '\0';
        trimmed_input = trim_whitespace(input);

        if (strlen(trimmed_input) == 0) {
            continue;
        }

        i = 0;
        token = strtok(trimmed_input, " ");
        while(token != NULL) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;

        printf("DEBUG: PATH = %s\n", getenv("PATH"));

        pid = fork();
        if (pid == 0) {
            if (execvp(args[0], args) == -1) {
                fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
                exit(127);
            }
        } else {
            while ((wpid = wait(&i)) > 0);
            if (WIFEXITED(i)) {
                int exit_status = WEXITSTATUS(i);
                if (exit_status == 127) {
                    exit(127);
                }
            }
        }
    }

    return 0;
}

