#include<stdio.h>

int main(int argc, char *argv [])
{
    if(argc != 2)
    {
        printf("Error: No file given\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    FILE *out = fopen("temp.c", "w");

    if(file == NULL)
    {
        printf("Error: cannot open %s file\n", argv[1]);
        return 1;
    }

    if(out == NULL)
    {
        printf("Error: cannot create temp file\n");
        return 1;
    }

    char *symbol[] = {"int", "float", "double", "long", "short", "switch", "return"};

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
        else if(c == ' ')
        {
            continue;
        }
        else if(c == '\t')
        {
            continue;
        }
        else if(c == '\n')
        {
            continue;
        }

        fputc(c, out);
    }

    fclose(file);
    fclose(out);
    rename("temp.c", argv[1]);

    return 0;
}