#include<stdio.h>

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
    remove(argv[1]);
    rename("temp.c", argv[1]);

    return 0;
}