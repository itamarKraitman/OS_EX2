#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define BUF_SIZE 1024

bool copyFile(FILE *f1, FILE *f2);
void printVerbose(bool result);

int main(int argc, char *argv[])
{
    if (argc < 3 && argc > 5)
    {
        printf("usage: cmp <file1> <file2>, or");
        printf("usage: cmp <file1> <file2> -f, or");
        printf("usage: cmp <file1> <file2> -v, or");
        printf("usage: cmp <file1> <file2> -i -v, or");
        printf("usage: cmp <file1> <file2> -v -i");
        return 1;
    }

    char *file1 = argv[1]; // First file name
    char *file2 = argv[2]; // Second file nam
    int verbosFl = 0;      // To print success/target file exist/general failure or 1\0
    int overwriteFl = 0;   // Force copy that allows to overwrite the target file
    bool fileEx = false;

    if (argc == 4)
    {
        if (strcmp(argv[3], "-v") == 0)
        {
            verbosFl = 1;
        }
        else if (strcmp(argv[3], "-f") == 0)
        {
            overwriteFl = 1;
        }
        else
        {
            printf("usage: cmp <file1> <file2>, or");
            printf("usage: cmp <file1> <file2> -i, or");
            printf("usage: cmp <file1> <file2> -v, or");
            printf("usage: cmp <file1> <file2> -i -v, or");
            printf("usage: cmp <file1> <file2> -v -i");
            return 1;
        }
    }
    if (argc == 5)
    {
        if ((strcmp(argv[3], "-v") == 0 && strcmp(argv[4], "-f") == 0) ||
            (strcmp(argv[3], "-f") == 0 && strcmp(argv[4], "-v") == 0))
        {
            verbosFl = 1;
            overwriteFl = 1;
        }
        else
        {
            printf("usage: cmp <file1> <file2>, or");
            printf("usage: cmp <file1> <file2> -i, or");
            printf("usage: cmp <file1> <file2> -v, or");
            printf("usage: cmp <file1> <file2> -i -v, or");
            printf("usage: cmp <file1> <file2> -v -i");
            return 1;
        }
    }

    // If the source exists
    FILE *sourceFile = fopen(file1, "r");
    if (sourceFile == NULL)
    {
        if (verbosFl == 1)
            printf("general failure\n");
        fclose(sourceFile);
        return 1;
    }

    if (overwriteFl == 0)
    {
        FILE *destFile = fopen(file2, "rb");
        if (destFile != NULL)
        {
            if (verbosFl == 1)
            printf("target file exist. Use -f to overwrite\n");
            fclose(destFile);
            fclose(sourceFile);
            return 1;
        }
    }
    FILE *destFile = fopen(file2, "wb");
    bool copy = copyFile(sourceFile, destFile);

    if (verbosFl)
    {
        printVerbose(copy);
    }

    fclose(sourceFile);
    fclose(destFile);

    return copy ? 0 : 1;
}

bool copyFile(FILE *sourceFile, FILE *destFile)
{
    char buf1[BUF_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buf1, 1, sizeof(buf1), sourceFile)) > 0)
    {
        fwrite(buf1, 1, bytes_read, destFile);
    }
    return true;
}

void printVerbose(bool copy)
{
    if (copy)
    {
        printf("success”\n");
    }
    else
        printf("general failure\n");
}