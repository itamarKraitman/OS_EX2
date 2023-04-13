#include <stdio.h>
#include <string.h>
#include "codecA.h"
#include "codecB.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Wrong usage. Three arguments should be sent in the following schema: encode <codec> <message>\n");
        return 1;
    }

    char *codec = argv[1];
    char *message = argv[2];
    char *code;

    if (strcmp(codec, "codecA") == 0)
    {
        code = encodeA(message);
    }
    else if (strcmp(codec, "codecB") == 0)
    {
        code = encodeB(message);
    }
    else
    {
        printf("No such codec %c", codec);
        return 1;
    }
    return code;
}
