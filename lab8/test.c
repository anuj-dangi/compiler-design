#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "removeComments.h"

typedef struct Symbol
{
    char identifier[50];
    int declared;       // line number where declared
    int used[25];       // lines where used
    int count;          // number of times used
} S;

S table[100];
int symbolCount = 0;

char *keywords[] = {"int", "float", "char", "if", "else", "for", "while", "return", "void"};
int numKeywords = 9;

int isKeyword(char *word)
{
    for (int i = 0; i < numKeywords; i++)
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    return 0;
}

// Find symbol index in table, -1 if not found
int findSymbol(char *name)
{
    for (int i = 0; i < symbolCount; i++)
        if (strcmp(table[i].identifier, name) == 0)
            return i;
    return -1;
}

// Add identifier to table
void addIdentifier(char *name, int line, int isDeclaration)
{
    int idx = findSymbol(name);
    if (idx == -1)
    {
        strcpy(table[symbolCount].identifier, name);
        table[symbolCount].declared = isDeclaration ? line : -1;
        table[symbolCount].count = isDeclaration ? 0 : 1;
        if (!isDeclaration)
            table[symbolCount].used[0] = line;
        symbolCount++;
    }
    else
    {
        if (!isDeclaration)
            table[idx].used[table[idx].count++] = line;
    }
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

    char word[50], prevWord[50] = "";
    int ch, i = 0, line = 1;

    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            line++;
            continue;
        }

        if (isspace(ch))
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
            fseek(file, -1, SEEK_CUR); // Step back after reading word

            if (!isKeyword(word))
            {
                int nextCh = fgetc(file);
                fseek(file, -1, SEEK_CUR); // Step back

                // Ignore function calls like printf(...) 
                if (nextCh != '(')
                {
                    // If previous word is keyword -> declaration
                    addIdentifier(word, line, isKeyword(prevWord));
                }
            }


            strcpy(prevWord, word);
        }
        else if (isdigit(ch))
        {
            // Skip numeric constants
            while (isdigit(ch))
                ch = fgetc(file);
            fseek(file, -1, SEEK_CUR);
        }
    }

    fclose(file);

    // Print symbol table
    printf("\nIdentifier\tDeclared\tUsed lines\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < symbolCount; i++)
    {
        printf("%s\t\t", table[i].identifier);
        if (table[i].declared != -1)
            printf("%d\t\t", table[i].declared);
        else
            printf("-\t\t");

        for (int j = 0; j < table[i].count; j++)
            printf("%d ", table[i].used[j]);
        printf("\n");
    }

    return 0;
}
