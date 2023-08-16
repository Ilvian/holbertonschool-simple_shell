#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024

int main() {
    while (1) {
        char input[MAX_INPUT_LENGTH];
        char *args[MAX_INPUT_LENGTH / 2]; // Assuming an average of 2 arguments per command

        // Display prompt
        printf("MyShell> ");

        // Read user input
        fgets(input, sizeof(input), stdin);

        // Remove newline character from input
        input[strcspn(input, "\n")] = '\0';

        // Parse input into arguments
        int argc = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL; // Null-terminate the arguments list

        // Check for built-in commands
        if (argc > 0) {
            if (strcmp(args[0], "exit") == 0) {
                // Exit the shell
                exit(0);
            } else if (strcmp(args[0], "echo") == 0 && argc == 2 && strcmp(args[1], "$$") == 0) {
                // Print the process ID (PID) of the shell
                printf("Shell Process ID: %d\n", getpid());
                continue; // Skip the fork and execvp steps
            }
        }

        // Fork a new process
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
        } else if (pid == 0) {
            // Child process
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        } else {
            // Parent process
            wait(NULL); // Wait for the child process to complete
        }
    }

    return 0;
}
