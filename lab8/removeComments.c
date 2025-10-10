#include<stdio.h>
#include "removeComments.h"

void multi_line(FILE *file)
{
    int c;

    while((c = fgetc(file)) != EOF)
    {
        if(c == '*' && fgetc(file) == '/')
        {
            return;
        }

    }
}

FILE* removeComments(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    FILE *out = fopen("temp.c", "w");

    if(file == NULL)
    {
        printf("Error: cannot open %s file\n", fileName);
        return NULL;
    }

    if(out == NULL)
    {
        printf("Error: cannot create temp file\n");
        return NULL;
    }

    int c;

    while((c = fgetc(file)) != EOF)
    {
        if(c == '"')
        {
            fputc(c, out);
            
            while((c = fgetc(file)) != '"' && c != EOF)
            {
                fputc(c , out);
            }
        }
        else if(c == '/')
        {
            c = fgetc(file);

            if(c == '/')
            {
                while((c = fgetc(file)) != '\n' && c != EOF)
                {
                    continue;
                }
                fputc('\n', out);
            }
            else if(c == '*')
            {
                multi_line(file);
            }

            c = fgetc(file);
            
        }

        fputc(c, out);
    }

    fclose(file);
    fclose(out);

    FILE* clean = fopen("temp.c", "r");

    return clean;
}