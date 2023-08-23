# C - Simple Shell
Project to be done in teams of 4 people (your team: Arlind, Ilvian, Donald Meerdanaj, Ersida Karaj)
# Objectives
-	Learn **Unix Shell**.
-	Learn Unix **Parent** and **Child Processes**.
-	Learn Unix **Input** and **Output redirections**.
-	Learn Unix **Pipe**.
-	Learn Unix **fork()**, **exec()**, **wait()**, **open()**, **close()**, **dup()**, and **pipe() system calls**.
-	Implement a **Simple Shell Application** using Unix system calls.

# Project 1 — Simple Shell
This is the first project of Operating System course

This project consists of designing a C program to serve as a shell interface that accepts user commands
and then executes each command in a separate process. This implementation will support input and
output redirection, as well as pipes as a form of IPC between a pair of commands. Completing this project
will involve using the UNIX fork(), exec(), wait(), dup2(), and pipe() system calls and can be completed
on Linux system.
# Description
hsh is a simple UNIX command language interpreter that reads commands from either a file or standard input and executes them.

# How hsh works
Prints a prompt and waits for a command from the user
Creates a child process in which the command is checked
Checks for built-ins, aliases in the PATH, and local executable programs
The child process is replaced by the command, which accepts arguments
When the command is done, the program returns to the parent process and prints the prompt
The program is ready to receive a new command
To exit: press Ctrl-D or enter "exit" (with or without a status)
Works also in non interactive mode
# Headers at the top.
-## include <sys/wait.h>
-waitpid() and associated macros
-## include <unistd.h>
- chdir()
- fork()
- exec()
- pid_t
- ## include <stdlib.h>
- malloc()
- realloc()
- free()
- exit()
- execvp()
- EXIT_SUCCESS, EXIT_FAILURE
- ## include <stdio.h>
- fprintf()
- printf()
- stderr
- getchar()
- perror()
- ## include <string.h>
-strcmp()
-strtok()
# Basic loop of a shell

- Read: Read the command from standard input.
- Parse: Separate the command string into a program and arguments.
- Execute: Run the parsed command.

## Compilation
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o myshell

## Invocation
Usage:  myshell [filename]

To invoke myshell, compile all .c files in the repository and run the resulting executable.

can be invoked both interactively and non-interactively. If hsh is invoked with standard input not connected to a terminal, it reads and executes received commands in order.

<a>
  <img src="https://img.icons8.com/color/48/000000/ubuntu--v1.png"/>
  
</a>

 <a>
<img src="./assets/images/zorin.png" width=48>
 </a>
 
<a>
  <img  src="https://img.icons8.com/color/48/000000/windows-10.png"/>
</a>



## I. Overview

One technique for implementing a shell interface is to have the parent process first read what the user
enters on the command line (in this case, cat prog.c) and then create a separate child process that performs
the command. Unless otherwise specified, the parent process waits for the child to exit before continuing.
However, UNIX shells typically also allow the child process to run in the background, or concurrently.
To accomplish this, we add an ampersand (&) at the end of the command. Thus, if we rewrite the above
command as


```c
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT "#cisfun$ "
#define MAX_INPUT_LENGTH 1024

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

    /**
    * After reading user input, the steps are:
    * (1) fork a child process using fork()
    * (2) the child process will invoke execvp()
    * (3) parent will invoke wait() unless command included &
    */
  }
  return 0;
}
```
The main() function presents the prompt osh-> and outlines the steps to be taken after input from the
user has been read. The main() function continually loops as long as should run equals 1; when the user
enters exit at the prompt, your program will set should run to 0 and terminate.
This project is organized into several parts:
1. Creating the child process and executing the command in the child
2. Providing a history feature
3. Adding support of input and output redirection
4. Allowing the parent and child processes to communicate via a pipe

## II. Executing Command in a Child Process
The first task is to modify the main() function so that a child process is forked and executes the command
specified by the user. This will require parsing what the user has entered into separate tokens and storing
the tokens in an array of character strings. For example, if the user enters the command ps -ael at the
osh> prompt, the values stored in the args array are:
```
args[1] = NULL

```
This args array will be passed to the execvp() function, which has the following prototype:
```
(execvp(args[0], args) == -1) {
                fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
```
Here, command represents the command to be performed and params stores the parameters to this
command. For this project, the execvp() function should be invoked as execvp(args[0], args). Be sure to
check whether the user included & to determine whether or not the parent process is to wait for the child
to exit.

## III. Creating a History Feature
The next task is to modify the shell interface program so that it provides a history feature to allow a user
to execute the most recent command by entering !!. For example, if a user enters the command ls -l, she
can then execute that command again by entering !! at the prompt. Any command executed in this fashion
should be echoed on the user’s screen, and the command should also be placed in the history buffer as
the next command. Your program should also manage basic error handling. If there is no recent command
in the history, entering !! should result in a message “No commands in history.”


## IV. Redirecting Input and Output
Your shell should then be modified to support the ‘>’ and ‘<’ redirection operators, where ‘>’ redirects
the output of a command to a file and ‘<’ redirects the input to a command from a file. For example, if a
user enters
```
ls > ./a.out
```
the output from the ls command will be redirected to the file ./a.out 
 Similarly, input can be redirected as well. For example, if the user enters
```
sort < ./a.out
```
the sort < ./a.out will serve as input to the sort command.
```
a.o      cat                  ls                  out.txt
a.out    generate-authors.sh  man_1_simple_shell  README.md
Authors  hsh                  myshell.c           shell
```
## V. Communication via a Pipe
The final modification to your shell is to allow the output of one command to serve as input to another
using a pipe. For example, the following command sequence
```
ls -l | less
```
```
total 76
-rw-r--r-- 1 root root   102 Aug 23 00:48 a.o
-rwxr-xr-x 1 root root     0 Aug 23 00:53 a.out
-rw-r--r-- 1 root root   313 Aug 22 12:18 Authors
-rw-r--r-- 1 root root     0 Aug 23 00:23 cat
-rwxr-xr-x 1 root root   390 Aug 22 12:14 generate-authors.sh
-rwxr-xr-x 1 root root 17376 Aug 22 15:13 hsh
-rw-r--r-- 1 root root     0 Aug 23 00:27 ls
-rw-r--r-- 1 root root  6902 Aug 22 11:25 man_1_simple_shell
-rw-r--r-- 1 root root  1673 Aug 23 00:45 myshell.c
-rw-r--r-- 1 root root     0 Aug 23 00:26 out.txt
-rw-r--r-- 1 root root 11797 Aug 22 16:03 README.md
-rwxr-xr-x 1 root root 17377 Aug 22 15:22 shell
```
has the output of the command ls -l serve as the input to the less command. Both the ls and less commands
will run as separate processes and will communicate using the UNIX pipe() function. Perhaps the easiest way to create these separate processes is to have the parent process create the child process (which will
execute ls -l). This child will also create another child process (which will execute less) and will establish
a pipe between itself and the child process it creates. Implementing pipe functionality will also require
using the dup2() function as described in the previous section. Finally, although several commands can
be chained together using multiple pipes, you can assume that commands will contain only one pipe
character and will not be combined with any redirection operators.

## 📘 Task
| # | Requirement content                                 | Status 🔨 |
| --- | ---------------------------------------- | ------------ |
| 0   |  **  README, man, AUTHORS** - Write a README - Write a man for your shell. - You should have an AUTHORS file at the root of your repository, listing all individuals having contributed content to the repository. Format, see Docker            | Done   |
| 1   | ** Betty would be proud ** - Write a beautiful code that passes the Betty checks  **         | Done   |
| 2   | ** Simple shell** .- Write a UNIX command line interpreter.Usage: simple_shell   **         | Done  |
| 3   | ** Simple shell** . - Handle command lines with arguments            | Done   |
| 4   | ** Simple shell** . -Simple shell 0.2 - +Handle the PATH.- fork must not be called if the command doesn’t exist         | Done   |
| 5   | ** Simple shell** . -Simple shell 0.3 + Implement the exit built-in, that exits the shell. -Usage: exit.- You don’t have to handle any argument to the built-in exit | Done   |
| 6   |  ** Simple shell** .   Simple shell 0.4 .- +Implement the env built-in, that prints the current environment         | Done   |

## 📘 Testing

### Operating System

<a>
  <img src="./assets/images/gnu.jpg" width=100>
</a>

### Executing Command in a Child Process

![Executing Command in a Child Process]![carbon](https://github.com/Ilvian/holbertonschool-simple_shell/assets/135588172/1ec1d89f-b61d-4ebf-8c7c-8ac013330073)

Figure 01: Executing Command in a Child Process



## Command Execution
After receiving a command, hsh tokenizes it into words using " " as a delimiter. The first word is considered the command and all remaining words are considered arguments to that command. hsh then proceeds with the following actions:

If the first character of the command is neither a slash (\) nor dot (.), the shell searches for it in the list of shell builtins. If there exists a builtin by that name, the builtin is invoked.
If the first character of the command is none of a slash (\), dot (.), nor builtin, hsh searches each element of the PATH environmental variable for a directory containing an executable file by that name.
If the first character of the command is a slash (\) or dot (.) or either of the above searches was successful, the shell executes the named program with any remaining given arguments in a separate execution environment.
## Exit Status
hsh returns the exit status of the last command executed, with zero indicating success and non-zero indicating failure.

If a command is not found, the return status is 127; if a command is found but is not executable, the return status is 126.

All builtins return zero on success and one or two on incorrect usage (indicated by a corresponding error message).


## What we learned:
How a shell works and finds commands
Creating, forking and working with processes
Executing a program from another program
Handling dynamic memory allocation in a large program
Pair programming and team work
Building a test suite to check our own code

## Authors
👤 **ARLIND**
👤 **ILVIAN DIMCO**
👤 **DONALD MERDANAJ**
👤 **ERSIDA KARAJ**

