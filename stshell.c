#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>


#define MAX_SIZE 1024

static char **copyArgs(char **args, int argc)
{
    char **copy_of_args = malloc(sizeof(char *) * (argc + 1));
    if (copy_of_args == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int k = 0;
    while (k < argc)
    {
        copy_of_args[k] = strdup(args[k]);
        k++;
    }
    copy_of_args[k] = 0;
    return copy_of_args;
}

void sigint_handler(int signum)
{
    printf("\n%dCtrl + C Pressed\n", signum);
}

char **parseCommand(char *command)
{
    int i = 0, bufsize = MAX_SIZE;
    char *token;
    char **tokens = malloc(bufsize * sizeof(char *));

    if (tokens == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token = strtok(command, " ");
    while (token != NULL)
    {
        tokens[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;

    return tokens;
}

int* findNumberOfPipes(char **args)
{
    int* numberOfpipes = malloc(2 * sizeof(int)); 
    numberOfpipes[0] = -1;
    numberOfpipes[1] = -1;
    int i = 1, j = 0;
    while (args[i] != NULL)
    {
        if (strcmp(args[i], "|") == 0)
        {
            numberOfpipes[j] = i;
            j++;
        }
        i++;
    }
    return numberOfpipes;
}

void redirectProcess(char **args, int k, int redirectSign)
{
    pid_t pid;
    int fd, status = 0;
    char *output = args[k + 1];

    pid = fork();
    if (pid == 0)
    {
        if (redirectSign == 1) // >
        {
            fd = open(output, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        }

        else // >> (2)
        {
            fd = open(output, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        }
        if (fd < 0)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        else
        {
            dup2(fd, redirectSign);
            close(fd);
            args[k] = NULL;
            args[k + 1] = NULL;

            if (execvp(args[0], args) == -1)
            {
                perror("excevp");
                exit(EXIT_FAILURE);
            }
        }
    }
    else if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else // parent process
    {
        waitpid(pid, &status, WUNTRACED);
    }
}

void onePipeProcess(char **args, int k)
{
    int fpipe[2], status;
    pid_t pid1, pid2;
    char **copy_args = copyArgs(args, k);

    if (pipe(fpipe) == -1) // create pipe- one input and one output
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == 0) /// child process 1
    {
        // send STDOUT to output part
        dup2(fpipe[1], STDOUT_FILENO);
        close(STDOUT_FILENO);
        close(fpipe[0]);
        close(fpipe[1]);

        // execute and pass the output as argument
        execvp(copy_args[0], copy_args);
        perror("execvp"); // if execution failed
        exit(EXIT_FAILURE);
    }
    else if (pid1 < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == 0) // child process 2
    {
        dup2(fpipe[0], STDIN_FILENO); // send STDIN to input part
        close(fpipe[1]);
        close(fpipe[0]);

        execvp(args[k + 1], args + k + 1);
        perror("execvp"); // if execution failed
        exit(EXIT_FAILURE);
    }
    else if (pid2 < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    close(fpipe[0]);
    close(fpipe[1]);

    // // wait for child processes to end
    // wait(NULL);
    // wait(NULL);
    waitpid(pid1, &status, WUNTRACED);
    waitpid(pid2, &status, WUNTRACED);
}

// void twoPipesProcess(char **args, int k)
// {
// int fpipe1[2], fpipe2[2], status;
// pid_t pid1, pid2, pid3;
// ;
// char **copy_args = copyArgs(args, k);

// if (pipe(fpipe1) == -1) // create pipe- one input and one output
// {
//     perror("pipe");
//     exit(EXIT_FAILURE);
// }

// pid1 = fork();
// if (pid1 == 0) /// child process 1
// {
//     // send STDOUT to output part
//     dup2(fpipe1[1], STDOUT_FILENO);
//     close(fpipe1[0]);
//     close(fpipe1[1]);

//     // execute and pass the output as argument
//     execvp(copy_args[0], copy_args);
//     perror("execvp"); // if execution failed
//     exit(EXIT_FAILURE);
// }
// else if (pid1 < 0)
// {
//     perror("fork");
//     exit(EXIT_FAILURE);
// }

// pid2 = fork();
// if (pid2 == 0) // child process 2
// {
//     // send STDIN to input part and close fpipe1
//     dup2(fpipe1[0], STDIN_FILENO);
//     close(fpipe1[1]);
//     close(fpipe1[0]);

//     execvp(args[k + 1], args + k + 1);
//     perror("execvp"); // if execution failed
//     exit(EXIT_FAILURE);

//     // send STDOUT to output part and close fpipe2
//     dup2(fpipe2[1], STDOUT_FILENO);
//     close(fpipe2[0]);
//     close(fpipe2[1]);
// }
// else if (pid2 < 0)
// {
//     perror("fork");
//     exit(EXIT_FAILURE);
// }

// waitpid(pid2, &status, 0); // if parent process, wait to child 2 to end

// pid3 = fork();
// if (pid3 == 0) // child process 3
// {
//     // send STDIN to input part and close fpipe2
//     dup2(fpipe2[0], STDIN_FILENO);
//     close(fpipe2[1]);
//     close(fpipe2[0]);

//     execvp(args[k + 3], args + k + 3);
//     perror("execvp"); // if execution failed
//     exit(EXIT_FAILURE);
// }

// close(fpipe2[0]);
// close(fpipe2[1]);

// waitpid(pid1, &status, 0);
// waitpid(pid3, &status, 0);

void twoPipesProcess(char **args, int k1, int k2)
{
    int fpipe[4];
    char **copy_args1, **copy_args2;

    copy_args1 = copyArgs(args, k1);
    copy_args2 = copyArgs(args + k1 + 1, k2 - k1 - 1);
    if (pipe(fpipe) == -1 || pipe(fpipe + 2) == -1)
    { // Create two pipes with two input pointers and two output pointers
        perror("pipe redirection failed");
        return;
    }

    if (fork() == 0) // child 1
    {
        dup2(fpipe[1], STDOUT_FILENO); // Redirect STDOUT to output part of first pipe
        close(fpipe[0]);               // close first pipe read
        close(fpipe[1]);               // close first pipe write
        close(fpipe[2]);               // close second pipe read
        close(fpipe[3]);               // close second pipe write

        execvp(copy_args1[0], copy_args1); // pass the truncated command line as argument
        perror("First program execution failed");
        exit(1);
    }

    if (fork() == 0) // child 2
    {
        dup2(fpipe[0], STDIN_FILENO);  // Redirect STDIN to Input part of first pipe
        dup2(fpipe[3], STDOUT_FILENO); // Redirect STDOUT to output part of second pipe
        close(fpipe[1]);               // closing first pipe write
        close(fpipe[2]);               // close second pipe read
        close(fpipe[0]);               // close first pipe read
        close(fpipe[3]);               // close second pipe write

        execvp(copy_args2[0], copy_args2); // pass the truncated command line as argument
        perror("Second program execution failed");
        exit(1);
    }

    if (fork() == 0) // child 3
    {
        dup2(fpipe[2], STDIN_FILENO); // Redirect STDIN to Input part of second pipe
        dup2(fpipe[3], STDOUT_FILENO); // Redirect STDIN to Input part of second pipe

        close(fpipe[1]);              // closing first pipe write
        close(fpipe[0]);              // close first pipe read
        close(fpipe[2]);              // close second pipe read
        close(fpipe[3]);              // close second pipe write

        execvp(args[k2 + 1], args + k2 + 1); // pass the third part of command line as argument
        perror("Third program execution failed");
        exit(1);
    }

    close(fpipe[0]);
    close(fpipe[1]);
    close(fpipe[2]);
    close(fpipe[3]);
    wait(0); // Wait for child 1 to finish
    wait(0); // Wait for child 2 to finish
    wait(0); // Wait for child 3 to finish
}

int main()
{
    char *command = malloc(MAX_SIZE * sizeof(char));
    char **args;
    int redirect, pipes, status;
    signal(SIGINT, sigint_handler); // handle ctrl+c

    while (1)
    {
        printf("%s", "stshell> ");
        fgets(command, 1024, stdin);

        command[strcspn(command, "\n")] = '\0'; // replace \n with \0

        if (strcmp(command, "exit") == 0) // exit shell
        {
            printf("Exiting stshell.\n");
            break;
        }

        args = parseCommand(command);

        // if command is empty, wait for next command
        if (args[0] == NULL)
        {
            continue;
        }

        // find if command contains ">", ">>" or "|";
        int i = 0;
        while (args[i] != NULL)
        {
            // find if command contains ">", ">>" or "|"
            if (strcmp(args[i], ">") == 0)
            {
                redirect = 1;
            }
            else if (strcmp(args[i], ">>") == 0)
            {
                redirect = 2;
            }
            else if (strcmp(args[i], "|") == 0)
            {
                pipes = 1;
            }

            // handle "special" cases
            if (redirect)
            {
                redirectProcess(args, i, redirect);
            }

            else if (pipes) // NOTE! number of pipes can be ONLY 1 or 2
            {
                int* numberOfPipes = findNumberOfPipes(args);
                if (numberOfPipes[2] == -1) // only one pipe
                {
                    onePipeProcess(args, i);
                }
                else // two pipes
                {
                    twoPipesProcess(args, numberOfPipes[0], numberOfPipes[1]);
                }
                free(numberOfPipes);
            }
            i++;
        }

        // basic cases
        pid_t pid = fork();
        if (pid == 0) // fork success. child initiated
        {
            execvp(args[0], args);
            perror("Program execution failed");
            exit(1);
        }
        waitpid(pid, &status, WUNTRACED);
    }
    free(command);
    free(args);
}
