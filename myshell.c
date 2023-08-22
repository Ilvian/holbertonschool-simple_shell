#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT "#cisfun$ "
#define MAX_INPUT_LENGTH 1024

int main(void) {
    char input[MAX_INPUT_LENGTH];
    pid_t pid;

    while (1) {
        if (isatty(STDIN_FILENO)) {
            printf(PROMPT);
            fflush(stdout); 
        }

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            exit(0);
        }

        input[strcspn(input, "\n")] = '\0';

        pid = fork();
        if (pid == -1) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            char *args[2];
            args[0] = input;
            args[1] = NULL;
            
            if (execve(input, args, NULL) == -1) {
                printf("./shell: No such file or directory\n");
                exit(1);
            }
        } else {
            wait(NULL);
        }
    }

    return 0;
}
