#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Symbol {
    char identifier[20];
    int declare;
    int used[20];
    int count;
} S;

S table[50];
int sym_count = 0;
int curr_line = 1;

int alphaNum(char word[])
{
    char* keywords[] = {"int", "float", "void", "double", "return", "char"};
    int n = 6;

    for (int i = 0; i < n; i++)
        if (strcmp(keywords[i], word) == 0)
            return 1;

    for (int j = 0; j < sym_count; j++)
    {
        if (strcmp(table[j].identifier, word) == 0)
        {
            table[j].used[table[j].count++] = curr_line;
            return 0;
        }
    }

    return 2;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Error: no file name given\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Error: cannot open file\n");
        return 1;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n' || ch == '\r')
        {
            curr_line++;
            continue;
        }

        if (isalpha(ch) || ch == '_')
        {
            char word[20];
            int i = 0;
            while (isalnum(ch) || ch == '_')
            {
                word[i++] = ch;
                ch = fgetc(file);
            }
            word[i] = '\0';
            fseek(file, -1, SEEK_CUR);

            int result = alphaNum(word);

            if (result == 1)
            {
                if (!strcmp(word, "int") || !strcmp(word, "float") ||
                    !strcmp(word, "char") || !strcmp(word, "double"))
                {
                    while ((ch = fgetc(file)) != EOF && !isalpha(ch) && ch != '_');
                    if (ch == EOF) break;

                    char id[20];
                    int j = 0;
                    while (isalnum(ch) || ch == '_')
                    {
                        id[j++] = ch;
                        ch = fgetc(file);
                    }
                    id[j] = '\0';
                    fseek(file, -1, SEEK_CUR);

                    strcpy(table[sym_count].identifier, id);
                    table[sym_count].declare = curr_line;
                    table[sym_count].count = 0;
                    sym_count++;
                }
            }
        }
    }

    fclose(file);

    printf("\nIdentifier\tDeclared\tUsed lines\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < sym_count; i++)
    {
        printf("%s\t\t", table[i].identifier);
        if (table[i].declare != -1)
            printf("%d\t\t", table[i].declare);
        else
            printf("-\t\t");

        for (int j = 0; j < table[i].count; j++)
            printf("%d ", table[i].used[j]);
        printf("\n");
    }

    return 0;
}
