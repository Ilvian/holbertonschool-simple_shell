#include "simple_shell.h"
int main(void)
{
    char command[MAX_COMMAND_LENGTH];
    shell_session_t session = {0};
    while (1)
    {
        printf("#cisfun$ ");
        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
        {
            printf("\n");
            exit(0);
        }
        command[strcspn(command, "\n")] = '\0';
        if (strlen(command) == 0)
            continue;
        execute_command(command, &session);
    }
    return 0;
}
void execute_command(char *command, shell_session_t *session)
{
    pid_t pid;
    int status;
    if ((pid = fork()) == -1)
    {
        perror("Error forking");
        exit(1);
    }
    if (pid == 0)
    {
        char *argv[] = {command, NULL};
        if (execve(command, argv, environ) == -1)
        {
            perror("./shell");
            exit(1);
        }
    }
    else
    {
        wait(&status);
        session->command_count++;
    }
}
