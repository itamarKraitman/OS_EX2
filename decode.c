#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "codecA.h"
#include "codecB.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Wrong usage. Three arguments should be sent in the following schema: dencode <codec> <message>\n");
        return 1;
    }

    // load the libraries
    void* decodecA_handle = dlopen("libcodecA.so", RTLD_NOW);
    if (decodecA_handle == NULL)
    {
        printf("Error: %s", dlerror());
        return 1;
    }
    
    void* decodecB_handle = dlopen("libcodecB.so", RTLD_NOW);
    if (decodecB_handle == NULL)
    {
        printf("Error: %s", dlerror());
        return 1;
    }

    // get function pinters
    typedef char* (*decodeA)(char*);
    decodeA dcodeA = (decodeA)dlsym(decodecA_handle, "decodeA");
    if (dcodeA == NULL)
    {
        printf("Error: %s", dlerror());
        return 1;
    }

    typedef char* (*decodeB)(char*);
    decodeB dcodeB = (decodeB)dlsym(decodecB_handle, "decodeB");
    if (dcodeB == NULL)
    {
        printf("Error: %s", dlerror());
        return 1;
    }

    char *decodec = argv[1];
    char *message = argv[2];
    char *originalText;

    if (strcmp(decodec, "decodecA") == 0)
    {
        originalText = dcodeA(message);
    }
    else if (strcmp(decodec, "decodecB") == 0)
    {
        originalText = dcodeB(message);
    }
    else
    {
        printf("No such codec %s", decodec);
        return 1;
    }
    
    printf("%s\n", originalText);


    dlclose(decodecA_handle);
    dlclose(decodecB_handle);

    return 0;
}
