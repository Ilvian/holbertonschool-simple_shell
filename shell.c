#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define PROMPT "#cisfun$ "

int main(void)
{
	char input[MAX_INPUT_SIZE];
	char *argv[2];
	int status;
	size_t len = strlen(input);
	pid_t child_pid;
	
	while (1)
	{
		printf(PROMPT);
		if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
		{
			printf("\n");
			exit(EXIT_SUCCESS);
		}
		if (input[len - 1] == '\n')
		{
			input[len - 1] = '\0';
		}
		argv[0] = input;
		argv[1] = NULL;
		if ((child_pid = fork()) == -1)
		{
			perror("Error forking");
			continue;
		}
		if (child_pid == 0)
		{
			if (execve(argv[0], argv, NULL) == -1)
			{
				perror("./shell");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			wait(&status);
		}
	}
	return 0;
}
