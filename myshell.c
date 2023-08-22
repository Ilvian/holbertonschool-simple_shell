#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

char *trim_whitespace(char *str);
void execute_command(char *command);

int main(void) {
    char *input = NULL;
    size_t len = 0;
    
    while (true) {
        printf("#cisfun$ ");
        getline(&input, &len, stdin);

        if (feof(stdin)) {
            printf("\n");
            free(input);
            exit(0);
        }

        input[strcspn(input, "\n")] = 0;
        execute_command(input);
    }

    free(input);
    return 0;
}

char *trim_whitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0) 
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

void execute_command(char *command) {
    pid_t pid, wpid;
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
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE) {
            fprintf(stderr, "./hsh: 1: %s: not found\n", command);
            exit(127);
        }
    }
}

