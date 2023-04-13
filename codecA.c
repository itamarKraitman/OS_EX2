// codecA: covert all lower case chars to upper case, and all upper case to lower case.
// All other chars will remain unchanged.

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

extern char *encodeA(char *text)
{
    int len = strlen(text);
    char *code = malloc(len + 1);
    for (int i = 0; i < len; i++)
    {
        if (isupper(text[i]))
        {
            code[i] = tolower(text[i]);
        }
        else if (islower(text[i]))
        {
            code[i] = toupper(text[i]);
        }
        else
        {
            code[i] = text[i];
        }
    }
    return code;
}

extern char *decodeA(char *code)
{
    return encodeA(code);
}