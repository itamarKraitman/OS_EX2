// #include <sys/stat.h>
// #include <sys/wait.h>
// #include <fcntl.h>
// #include "stdio.h"
// #include "errno.h"
// #include "stdlib.h"
// #include "unistd.h"
// #include <string.h>

// void sigint_handler(int signum)
// {
//     printf("\n");
// }

// int main()
// {
//     int i;
//     char *argv[1024];
//     char command[1024];
//     char *token;
//     int redirect_output = 0;
//     int append_output = 0;
//     int num_tokens;
//     char *file;
//     int output_fd;
//     char *output_file;

//     signal(SIGINT, sigint_handler);

//     while (1)
//     {
//         printf("%s", "$ ");
//         fgets(command, 1024, stdin);

//         command[strcspn(command, "\n")] = '\0'; // replace \n with \0

//         if (strcmp(command, "exit") == 0)
//         { // exit shell
//             printf("Exiting stshell.\n");
//             break;
//         }

//         /* parse command line */
//         i = 0;
//         token = strtok(command, " ");
//         while (token != NULL)
//         {
//             argv[i] = token;
//             token = strtok(NULL, " ");
//             i++;
//         }
//         argv[i] = NULL;

//         /* Is command empty */
//         if (argv[0] == NULL)
//             continue;

//         for (i = 0; argv[i] != NULL; i++)
//         {
//             if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>") == 0)
//             {
//                 output_file = argv[i + 1];
//                 // argv[i] = NULL;
//                 if (strcmp(argv[i], ">") == 0)
//                 {
//                     output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//                 }
//                 else
//                 {
//                     output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
//                 }
//                 if (output_fd < 0)
//                 {
//                     perror(output_file);
//                     exit(1);
//                 }
//                 dup2(output_fd, STDOUT_FILENO);
//                 close(output_fd);
//                 break;
//             }
//         }

//         // Check for pipes
//         int num_pipes = 0;
//         int pipefd[2];
//         pid_t child_pid;
//         int args = 0;
//         for (i = 0; argv[i] != NULL; i++)
//         {
//             if (strcmp(argv[i], "|") == 0)
//             {
//                 num_pipes++;
//             }
//         }

//         if (num_pipes > 0)
//         {
//             // Initialize input pipe
//             pipe(pipefd);

//             // Execute first command and send output to input pipe
//             argv[i] = NULL;
//             child_pid = fork();
//             if (child_pid == 0)
//             {
//                 dup2(pipefd[1], STDOUT_FILENO);
//                 close(pipefd[0]);
//                 close(pipefd[1]);
//                 execvp(argv[0], argv);
//                 perror(argv[0]);
//                 exit(1);
//             }
//             else if (child_pid < 0)
//             {
//                 perror("fork");
//                 exit(1);
//             }
//             close(pipefd[1]);

//             // // Execute intermediate commands and send output to next command
//             // for (int j = 1; j < num_pipes; j++)
//             // {
//             //     args = args + i + 1;
//             //     i = 0;
//             //     pipe(pipefd);
//             // }
//         }

//         /* for commands not part of the shell command language */
//         if (fork() == 0)
//         {
//             execvp(argv[0], argv);
//             exit(1);
//         }
//         else
//         {
//             wait(NULL);
//         }
//     }
// }

#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

void sigint_handler(int signum)
{
    printf("\n");
}

int main()
{
    int i, redirect;
    char *argv[1024];
    char command[1024];
    char *token;
    int redirect_output = 0;
    int append_output = 0;
    int num_tokens;
    char *file;

    signal(SIGINT, sigint_handler);

    while (1)
    {
        printf("%s", "stshell> ");
        fgets(command, 1024, stdin);

        command[strcspn(command, "\n")] = '\0'; // replace \n with \0

        if (strcmp(command, "exit") == 0)
        { // exit shell
            printf("Exiting stshell.\n");
            break;
        }

        /* parse command line */
        i = 0;
        token = strtok(command, " ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;

        /* Is command empty */
        if (argv[0] == NULL)
            continue;

        for (i = 0; argv[i] != NULL; i++)
        {
            if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>"))
            {
                redirect = 1;
                // output_file = argv[i + 1];
                // output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR, 0644);
                // output_fd = creat(output_file, 0660);
            }
        }

        // else if (strcmp(argv[i], ">>") == 0)
        // {
        //     redirect = 1;
        //     // output_file = argv[i + 1];
        //     // output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR, 0660);
        //     // output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0660);

        // }

        if (fork() == 0)
        {

            // int redirect = 0;

            // for (i = 0; argv[i] != NULL; i++)
            // {
            //     if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>"))
            //     {
            //         redirect = 1;
            //         // output_file = argv[i + 1];
            //         // output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR, 0644);
            //         // output_fd = creat(output_file, 0660);
            //     }
            //     // else if (strcmp(argv[i], ">>") == 0)
            //     // {
            //     //     redirect = 1;
            //     //     // output_file = argv[i + 1];
            //     //     // output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR, 0660);
            //     //     // output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0660);

            //     // }
            // }
            if (redirect)
            {
                int output_fd = 0;
                char *output_file;
                if (strcmp(argv[i], ">") == 0)
                {
                    output_fd = creat(output_file, 0660);
                    close(STDOUT_FILENO);
                    dup(output_fd);
                    close(output_fd);
                }
                else
                {
                    output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0660);
                    if (output_fd < 0)
                    {
                        perror("open");
                        exit(EXIT_FAILURE);
                    }
                    close(STDOUT_FILENO);
                    dup(output_fd);
                    close(output_fd);
                }
            }
            //         // output_file = argv[i + 1];
            //         // output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR, 0644);
            //         // output_fd = creat(output_file, 0660);
            //     }
            //     // else if (strcmp(argv[i], ">>") == 0)
            //     // {
            //     //     redirect = 1;
            //     //     // output_file = argv[i + 1];
            //     //     // output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR, 0660);
            //     //     // output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0660);
            //     // }
            // if (output_fd < 0)
            // {
            //     perror(output_file);
            //     exit(1);
            // }
            // else
            // {
            //     close(STDOUT_FILENO);
            //     dup(output_fd);
            //     close(output_fd);
            //     break;
            // }
            //
            //     if (fork() == 0)
            //     {
            //         close(STDOUT_FILENO);
            // dup2(output_fd, STDOUT_FILENO);
            // close(output_fd);
            // execvp(argv[0], argv);
            //         perror("execvp");
            //         exit(1);
            //     }
            
            else 
            {
                execvp(argv[0], argv);
            }
        }
        else
        {
            wait(NULL);
        }

        /* for commands not part of the shell command language */

        if (fork() == 0)
        {
            execvp(argv[0], argv);
            exit(1);
        }
        else
        {
            wait(NULL);
        }
    }
}
