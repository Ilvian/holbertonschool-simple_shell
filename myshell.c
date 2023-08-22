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
    pid_t pid;

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

        pid = fork();
        if (pid == 0) {
            char *args[2];
            args[0] = trimmed_input;
            args[1] = NULL;
            
            if (execve(trimmed_input, args, NULL) == -1) {
                printf("./shell: No such file or directory\n");
                exit(1);
            }
        } else {
            wait(NULL);
        }
    }

    return 0;
}

