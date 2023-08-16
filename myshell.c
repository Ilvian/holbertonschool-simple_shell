#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024

int main() {
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_INPUT_LENGTH / 2];
    FILE *input_stream = stdin;  // Default to standard input

    while (1) {
        if (isatty(fileno(input_stream))) {
            // If in interactive mode, display prompt
            printf("MyShell> ");
        }

        if (fgets(input, sizeof(input), input_stream) == NULL) {
            break; // Exit the loop on EOF (Ctrl+D)
        }
        
        input[strcspn(input, "\n")] = '\0';

        int argc = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        if (argc > 0) {
            if (strcmp(args[0], "exit") == 0) {
                exit(0);
            } else if (strcmp(args[0], "echo") == 0 && argc == 2 && strcmp(args[1], "$$") == 0) {
                printf("Shell Process ID: %d\n", getpid());
                continue;
            } else if (strcmp(args[0], "echo") == 0 && argc == 2 && strcmp(args[1], "$PATH") == 0) {
                printf("PATH: %s\n", getenv("PATH"));
                continue;
            }
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
        } else if (pid == 0) {
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        } else {
            wait(NULL);
        }
    }

    return 0;
}
