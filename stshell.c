#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

void redirect_process(int redirect_sign, int outfd, char *outfile)
{
    if (redirect_sign == 1) // >
    {
        outfd = creat(outfile, 0660);
        // close(STDOUT_FILENO);
        dup(outfd);
        close(outfd);
    }
    else // >>
    {
        outfd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0660);
        if (outfd < 0)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        // close(STDOUT_FILENO);
        dup(outfd);
        close(outfd);
    }
}

void sigint_handler(int signum)
{
    printf("Ctrl + C Pressed\n");
}

int main()
{
    int i, redirect, pipe_cmd = 0;
    char *argv[1024];
    char command[1024];
    char *token;
    char *redirect_sign;

    signal(SIGINT, sigint_handler);

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

        // parse command line
        i = 0;
        token = strtok(command, " ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;

        // Is command empty- go to next iteration
        if (argv[0] == NULL)
            continue;

        for (i = 0; argv[i] != NULL; i++) // find wether the user used redirect or pipe
        {
            if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>") == 0)
            {
                redirect = 1;
                redirect_sign = argv[i];
            }
            else if (strcmp(argv[i], "|"))
            {
                pipe_cmd++; // count how many pipe signs are
            }
        }

        if (redirect)
        {
            if (fork() == 0)
            {
                int output_fd = 0;
                char *output_file;
                if (strcmp(redirect_sign, ">") == 0)
                {
                    redirect_process(1, output_fd, output_file);
                }
                else
                {
                    redirect_process(2, output_fd, output_file);
                }
                // execvp(argv[0], argv);
            }

            else // parent
            {
                wait(0);
            }
        }
        if (pipe_cmd > 0)
        {
            // char **argsForPipe = splitArgsToPipe(pipe_cmd, i);
            // int pipefd[pipe_cmd] - 1;

            // if (pipe(pipefd) == -1)
            // {
            //     perror("pipe");
            //     exit(EXIT_FAILURE);
            // }

            // int k = 0;
            // while (k < pipe_cmd)
            // {
            //     pid_t first_child = fork();
            //     if (first_child == -1)
            //     {
            //         perror("fork");
            //         exit(EXIT_FAILURE);
            //     }
            //     else if (first_child == 0) // child process
            //     {
            //         dup2(pipefd[1], STDOUT_FILENO);
            //         close(pipefd[0]);
            //         close(pipefd[1]);
            //         execvp(copy)
            //     }
            // }
            // pipe_func(pipe_cmd, , )
            // int file_d[3];
            // int last = STDIN_FILENO;
            // for (int j = 0; j < pipe_cmd; j++)
            // {
            //     if (j != pipe_cmd - 1)
            //     {
            //         if (pipe(file_d) == -1)
            //         {
            //             perror("pipe");
            //             exit(EXIT_FAILURE);
            //         }
            //     }
            //     pid_t pid = fork();
            //     if (pid == -1)
            //     {
            //         perror("fork");
            //     }
            //     else if (pid == 0)
            //     {
            //         if (last != STDIN_FILENO)
            //         {
            //             if (dup2(last, STDIN_FILENO))
            //             {
            //                 perror("dup2");
            //                 exit(EXIT_FAILURE);
            //             }
            //             close(last);
            //         }
            //         if (j != pipe_cmd - 1)
            //         {
            //             if (dup2(file_d[1], STDOUT_FILENO) == -1)
            //             {
            //                 perror("dup2");
            //                 exit(EXIT_FAILURE);
            //             }
            //             close(file_d[1]);
            //         }
            //         char **cargs;
            //         int num_args = sizeof(argv) / sizeof(argv[0]);;
            //         cargs = malloc((num_args + 1) * sizeof(char *));
            //         if (cargs == NULL)
            //         {
            //             perror("malloc");
            //             exit(EXIT_FAILURE);
            //         }
            //         for (int j = 0; j < num_args; j++)
            //         {
            //             cargs[j] = malloc((strlen(argv[j]) + 1) * sizeof(char));
            //             if (cargs[j] == NULL)
            //             {
            //                 perror("malloc");
            //                 exit(EXIT_FAILURE);
            //             }
            //             strcpy(cargs[j], argv[j].c_str());
            //         }
            //         cargs[num_args] = NULL;
            //         execvp(cargs[0], cargs);
            //         perror("execvp failed");
            //         exit(EXIT_FAILURE);
            //     }
            //     else
            //     {
            //         {
            //             // Close input of previous command (if not first command)
            //             if (last != STDIN_FILENO)
            //             {
            //                 close(last);
            //             }
            //             // Close output of this command (if not last command)
            //             if (j != pipe_cmd - 1)
            //             {
            //                 close(file_d[1]);
            //                 last = file_d[0];
            //             }
            //             // Wait for command to finish
            //             wait(NULL);
            //         }
            //     }
            // }
            // continue;
            int pipefd1[2], pipefd2[2];
            pid_t pid1, pid2, pid3;
            int status;

            if (pipe(pipefd1) == -1) // first pipe
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            pid1 = fork(); // first child
            if (pid1 == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pid1 == 0) // first child process
            {
                if (dup2(pipefd1[1], STDOUT_FILENO) == -1) // redirect std output to pipefd1 write end
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                
                // close pipedf1
                close(pipefd1[0]);
                close(pipefd1[1]);
                
                // execute command
                execvp(argv[0], argv);
                perror("execvp");
                exit(EXIT_FAILURE);
            }

            if (pipe(pipefd2) == -1) // second pipe
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            pid2 = fork(); // second child
            if (pid2 == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pid2 == 0) // second child process
            {
                if (dup2(pipefd2[0], STDIN_FILENO) == -1) // redirect std output to pipefd1 read end
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                
                // close pipefd1 read and write
                close(pipefd1[1]);
                close(pipefd1[1]);

                if (dup2(pipefd2[1], STDOUT_FILENO) == -1) // redirect std output to pipefd2 write end
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }

                // close pipedf2
                close(pipefd2[0]);
                close(pipefd2[1]);

                // execute command
                execvp(argv[pipe_cmd / 2], &argv[pipe_cmd / 2]);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            
            // parent process
            close(pipefd1[0]);
            close(pipefd1[1]);

            waitpid(pid2, &status, 0); // wait for child 2 proces to end

            pid3 = fork();
            if (pid3 == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pid3 == 0)
            {
                // child 3

                if (dup2(pipefd2[0], STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }

                close(pipefd2[0]);
                close(pipefd2[1]);

                execvp(argv[pipe_cmd / 2 + 1], &argv[pipe_cmd / 2 + 1]);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            
            close(pipefd2[0]);
            close(pipefd2[2]);
            
            waitpid(pid1, &status, 0);

            waitpid(pid3, &status, 0);
        }

        /* for commands not part of the shell command language */

        if (fork() == 0)
        {
            execvp(argv[0], argv);
            exit(1);
        }
        else
        {
            wait(0);
        }
    }
}
