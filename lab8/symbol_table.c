#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "removeComments.h"

typedef struct Symbol
{
    char identifier[50];
    int declared;
    int used[25];
    int count;
}S;

S table[100];
int count = 0;

int isKeyword(char *word)
{
    char *keywords[] = {"int", "float", "char", "if", "else", "for", "while", "return", "void"};
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < n; i++)
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Error: file name not given\n");
        return 1;
    }

    FILE *file = removeComments(argv[1]);
    if (file == NULL)
    {
        printf("Error: cannot open file\n");
        return 1;
    }

    char word[50];
    int ch, i = 0;

    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == ' ')
            continue;

        if (isalpha(ch) || ch == '_')
        {
            i = 0;
            while (isalnum(ch) || ch == '_')
            {
                word[i++] = ch;
                ch = fgetc(file);
            }
            word[i] = '\0';
            fseek(file, -1, SEEK_CUR);   

            if (isKeyword(word))
                printf("%s\t-> Keyword\n", word);
            else
                printf("%s\t-> Identifier\n", word);
        }
        else if (isdigit(ch))
        {
            i = 0;
            while (isdigit(ch))
            {
                word[i++] = ch;
                ch = fgetc(file);
            }
            word[i] = '\0';
            fseek(file, -1, SEEK_CUR);
            printf("%s\t-> Constant\n", word);
        }
        else if (ispunct(ch))
        {
            printf("%c\t-> Symbol/Operator\n", ch);
        }
    }

    fclose(file);
    return 0;
}
