#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_INPUT_LENGTH 1024
int main(int argc, char *argv[]) {
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_INPUT_LENGTH / 2];
    FILE *input_stream = stdin;
    int i;
    pid_t pid;
    char *token;
    (void) argc;
    while (1) {
        if (isatty(fileno(input_stream))) {
            printf("MyShell> ");
        }
        if (fgets(input, sizeof(input), input_stream) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = '\0';
        i = 0;
        token = strtok(input, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        if (i > 0) {
            if (strcmp(args[0], "exit") == 0){
                exit(0);
            } else if (strcmp(args[0], "echo") == 0 && i == 2 && strcmp(args[1], "$$") == 0) {
                printf("Shell Process ID: %d\n", getpid());
                continue;
            } else if (strcmp(args[0], "echo") == 0 && i == 2 && strcmp(args[1], "$PATH") == 0) {
                printf("PATH: %s\n", getenv("PATH"));
                continue;
            }
        }
        pid = fork();
        if (pid == -1) {
            perror("fork");
        } else if (pid == 0) {
            if(execvp(args[0], args))
            {
                        fprintf(stderr,"%s: 1: %s: not found\n", argv[0] ,args[0]);
            exit(127);}
        } else {
            wait(NULL);
        }
    }
    return 0;
}
