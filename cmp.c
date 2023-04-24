#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define BUF_SIZE 1024

bool isEqual(FILE* f1, FILE* f2, int ignoreCaseFl);
void printVerbose(bool result);


// argc - argument count, helps to indicate validate of the input.
// argv - vector of arguments input

int main(int argc, char *argv[])
{
    if (argc < 3 && argc > 5)
    {
        printf("usage: cmp <file1> <file2>, or");
        printf("usage: cmp <file1> <file2> -i, or");
        printf("usage: cmp <file1> <file2> -v, or");
        printf("usage: cmp <file1> <file2> -i -v, or");
        printf("usage: cmp <file1> <file2> -v -i");
        return 1;
    }

    char *file1 = argv[1]; // First file name
    char *file2 = argv[2]; // Second file nam
    int verbosFl = 0;      // To print equal/distinct or 1\0
    int ignoreCaseFl = 0;  // Ignore upper case or not
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");

    // If the file are good
    if (f1 == NULL || f2 == NULL)
    {
        printf("One or both files are unreachable");
        return 1;
    }

    // If the flag are valid and the flags
    if (argc == 4)
    {
        if (strcmp(argv[3], "-v") == 0)
        {
            verbosFl = 1;
        }
        else if (strcmp(argv[3], "-i") == 0)
        {
            ignoreCaseFl = 1;
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
        if ((strcmp(argv[3], "-v") == 0 && strcmp(argv[4], "-i") == 0) ||
            (strcmp(argv[3], "-i") == 0 && strcmp(argv[4], "-v") == 0))
        {
            verbosFl = 1;
            ignoreCaseFl = 1;
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

    bool equal = isEqual(f1, f2, ignoreCaseFl);
    if (verbosFl)
    {
        printVerbose(equal);
    }

    fclose(f1);
    fclose(f2);

    return equal ? 0 : 1;
}

bool isEqual(FILE *f1, FILE *f2, int ignoreCase)
{
    char buf1[BUF_SIZE];
    char buf2[BUF_SIZE];
    int len1, len2;

    while (true)
    {
        len1 = fread(buf1, 1, BUF_SIZE, f1);
        len2 = fread(buf2, 1, BUF_SIZE, f2);

        if (len1 != len2)
        {
            return 0;
        }

        if (feof(f1) || feof(f2))
        {
            break;
        }

        if (ignoreCase == 1)
        {
            for (int i = 0; i < len1; i++)
            {
                buf1[i] = tolower(buf1[i]);
                buf2[i] = tolower(buf2[i]);
            }
        }

        if (memcmp(buf1, buf2, len1) != 0)
        {
            return false;
        }
    }
    return true;
}

void printVerbose(bool filesEqual)
{
    if (filesEqual)
    {
        printf("equal\n");
    }
    else
    {
        printf("distinct\n");
    }
}


