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
    int i;
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
        printf("%s", "$ ");
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

        // Check for output redirection with ">"
        redirect_output = 0;
        append_output = 0;
        for (int i = 0; i < num_tokens; i++)
        {
            if (strcmp(argv[i], ">") == 0)
            {
                redirect_output = 1;
                file = argv[i + 1];
                argv[i] = NULL;
            }
            else if (strcmp(argv[i], ">>") == 0)
            {
                append_output = 1;
                file = argv[i + 1];
                argv[i] = NULL;
            }
        }

        // Check for output redirection with ">"
        redirect_output = 0;
        append_output = 0;
        for (int i = 0; i < num_tokens; i++)
        {
            if (strcmp(argv[i], ">") == 0)
            {
                redirect_output = 1;
                file = argv[i + 1];
                argv[i] = NULL;
            }
            else if (strcmp(argv[i], ">>") == 0)
            {
                append_output = 1;
                file = argv[i + 1];
                argv[i] = NULL;
            }
        }

        /* for commands not part of the shell command language */
        if (fork() == 0)
        {
            execvp(argv[0], argv);
            wait(NULL);
        }
    }
}
