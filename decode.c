#include <stdio.h>
#include <string.h>
#include "codecA.h"
#include "codecB.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Wrong usage. Three arguments should be sent in the following schema: dencode <codec> <message>\n");
        return 1;
    }

    char *decodec = argv[1];
    char *message = argv[2];
    char *originalText;

    if (strcmp(decodec, "decodecA") == 0)
    {
        originalText = decodeA(message);
    }
    else if (strcmp(decodec, "decodecB") == 0)
    {
        originalText = encodeB(message);
    }
    else
    {
        printf("No such codec %c", decodec);
        return 1;
    }
    printf("%c", originalText);
}
