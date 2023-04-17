// codecB: convert all chars to the 3-rd next char (adding a number of 3 to the ascii
// val).

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

extern char *encodeB(char *text)
{
    int len = strlen(text);
    char *code = malloc(len + 1);
    for (int i = 0; i < len; i++)
    {
        code[i] = (text[i] + 3) % 255;
    }
    
    return code;
}

extern char *decodeB(char *code)
{
    int len = strlen(code);
    char *originalText = malloc(len + 1);
    for (int i = 0; i < len; i++)
    {
        originalText[i] = (code[i] - 3) % 255;
    }

    return originalText;
}