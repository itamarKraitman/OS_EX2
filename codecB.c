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
    for (size_t i = 0; i < len; i++)
    {
        // if (isupper(text[i]))
        // {
        //     code[i] = (text[i] + 3 - 'A') % 26 + 'A';
        // }
        // else // small letter for sure
        // {
        //     code[i] = (text[i] + 3 - 'a') % 26 + 'a';
        // }

        code[i] = (text[i] + 3) % 255; //https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.quora.com%2FWhat-are-extended-ASCII-characters&psig=AOvVaw34jngRYohO2dxBLH1mCDet&ust=1681454680319000&source=images&cd=vfe&ved=0CBEQjRxqFwoTCNDWlfqgpv4CFQAAAAAdAAAAABAu

    }

    return code;
}

extern char *decodeB(char *code)
{
    return encodeB(code);
}