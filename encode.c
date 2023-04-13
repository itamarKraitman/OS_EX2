#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "codecA.h"
#include "codecB.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Wrong usage. Three arguments should be sent in the following schema: encode <codec> <message>\n");
        return 1;
    }

    // load the libraries
    void* codecA_handle = dlopen("libcodecA.so", RTLD_NOW);
    if (codecA_handle == NULL)
    {
        printf("Error has occured while opening codecA_handler");
        return 1;
    }
    
    void* codecB_handle = dlopen("libcodecB.so", RTLD_NOW);
    if (codecB_handle == NULL)
    {
        printf("Error has occured while opening codecB_handler");
        return 1;
    }

    // get function pinters
    typedef char* (*encodeA)(char*);
    encodeA codeA = (encodeA)dlsym(codecA_handle, "codeA");
    if (codeA == NULL)
    {
        printf("Error has occured while creating pinter to encodeA");
        return 1;
    }

    typedef char* (*encodeB)(char*);
    encodeB codeB = (encodeB)dlsym(codecB_handle, "codeA");
    if (codeA == NULL)
    {
        printf("Error has occured while creating pinter to encodeA");
        return 1;
    }


    char *codec = argv[1];
    char *message = argv[2];
    char *code;

    if (strcmp(codec, "codecA") == 0)
    {
        code = codeA(message);
    }
    else if (strcmp(codec, "codecB") == 0)
    {
        code = codeB(message);
    }
    else
    {
        printf("No such codec %c", codec);
        return 1;
    }
    printf("%c", code);

    dlclose(codecA_handle);
    dlclose(codecB_handle);
    return 0;
}
