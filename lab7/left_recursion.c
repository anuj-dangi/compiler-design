#include <stdio.h>
#include <string.h>

#define max 50

int n = 0;
char prod[max][max];

char outProd[max][max]={'\0'};

int ntCount = 0;
char nt[max];

char alpha[max][10][max];
char beta[max][10][max];

char temp = 'Z';
int newN = 0;

int indexOf(char c)
{
    for (int i = 0; i < ntCount; i++)
    {
        if (nt[i] == c)
        {
            return i;
        }
    }
    return -1;
}

int isNt(char c)
{
    return (c >= 'A' && c <= 'Z');
}

int isPresent(char arr[], char c)
{
    int i = 0;
    while (arr[i] != '\0')
    {
        if (arr[i] == c)
            return 1;
        i++;
    }
    return 0;
}

void add(char arr[], char c)
{
    if (!isPresent(arr, c))
    {
        int i = 0;
        while (arr[i] != '\0')
        {
            i++;
        }

        arr[i] = c;
        arr[i + 1] = '\0';
    }
}

void findAlphaBeta(int index, char c)
{
    int ak = 0, bk = 0;
    for (int i = 0; i < n; i++)
    {
        if (prod[i][0] == c)
        {
            int j = 3;

            if (prod[i][j] == c)
            {
                for (int l = j + 1; prod[i][l] != '\0'; l++)
                {
                    add(alpha[index][ak], prod[i][l]);
                }
                ak++;
            }
            else
            {
                for (int l = j; prod[i][l] != '\0'; l++)
                {
                    add(beta[index][bk], prod[i][l]);
                }
                bk++;
            }
        }
    }
}

void leftRec(int index, char c)
{
    if (alpha[index][0][0] != '\0')
    {
        int i = 0;
        do
        {
            outProd[newN][0] = c;
            outProd[newN][1] = '-';
            outProd[newN][2] = '>';

            int j = 3, k = 0;
            while (beta[index][i][k] != '\0')
            {
                outProd[newN][j] = beta[index][i][k];
                j++;
                k++;
            }
            outProd[newN][j++] = temp;
            outProd[newN][j] = '\0';
            i++;
            newN++;
        } while (beta[index][i][0] != '\0');

        i = 0;
        while (alpha[index][i][0] != '\0')
        {
            outProd[newN][0] = temp;
            outProd[newN][1] = '-';
            outProd[newN][2] = '>';

            int j = 3, k = 0;
            while (alpha[index][i][k] != '\0')
            {
                outProd[newN][j] = alpha[index][i][k];
                j++;
                k++;
            }
            outProd[newN][j++] = temp;
            outProd[newN][j] = '\0';
            i++;
            newN++;
        }

        outProd[newN][0] = temp;
        outProd[newN][1] = '-';
        outProd[newN][2] = '>';
        outProd[newN][3] = '~';
        outProd[newN][4] = '\0';
        newN++;

        temp--;
    }
    else
    {
        int i = 0;
        while (beta[index][i][0] != '\0')
        {
            outProd[newN][0] = c;
            outProd[newN][1] = '-';
            outProd[newN][2] = '>';

            int j = 3, k = 0;
            while (beta[index][i][k] != '\0')
            {
                outProd[newN][j] = beta[index][i][k];
                j++;
                k++;
            }
            outProd[newN][j] = '\0';
            i++;
            newN++;
        }
    }
}

void findAll(int index, char rhs[max][max])
{
    int k = 0;
    for (int i = 0; i < n; i++)
    {
        if (prod[i][0] == nt[index])
        {
            int j = 3;
            while (prod[i][j] != '\0')
            {
                rhs[k][j - 3] = prod[i][j];
                j++;
            }
            k++;
        }
    }
    rhs[k][0] = '\0';
}

void removeIndirect(int index, char c)
{
    for (int i = 0; i < n; i++)
    {
        if (prod[i][0] == c)
        {
            int tempc = prod[i][3];
            if (tempc != c && isNt(tempc))
            {
                for (int l = 0; l < n; l++)
                {
                    if (prod[l][0] == tempc)
                    {
                        if (prod[l][3] == prod[i][0])
                        {
                            char rhs[max][max];

                            int p = indexOf(tempc);

                            findAll(p, rhs);

                            char rRhs[max][max];

                            char str[max];
                            for (int k = 4; prod[i][k] != '\0'; k++)
                            {
                                str[k - 4] = prod[i][k];
                            }
                            str[strlen(str)] = '\0';

                            int u = 0;
                            while (rhs[u][0] != '\0')
                            {
                                int y = 0;
                                while (rhs[u][y] != '\0')
                                {
                                    rRhs[u][y] = rhs[u][y];
                                    y++;
                                }

                                int r = 0;
                                while (str[r] != '\0')
                                {
                                    rRhs[u][y] = str[r];
                                    y++;
                                    r++;
                                }
                                rRhs[u][y] = '\0';
                                u++;
                            }
                            rRhs[u][0] = '\0';

                            int y = 0;
                            while (rRhs[0][y] != '\0')
                            {
                                prod[i][y + 3] = rRhs[0][y++];
                            }
                            prod[i][y + 3] = '\0';

                            y = 1;

                            while (rRhs[y][0] != '\0')
                            {
                                prod[n][0] = prod[i][0];
                                prod[n][1] = '-';
                                prod[n][2] = '>';
                                int r = 0;
                                while (rRhs[y][r] != '\0')
                                    prod[n][r + 3] = rRhs[y][r++];
                                prod[n][r + 3] = '\0';
                                n++;
                                y++;
                            }
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("error: file name is not given\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        perror("Error: cannot open file\n");
        return 1;
    }

    int c;

    while ((c = fgetc(file)) != EOF)
    {
        fseek(file, -1, SEEK_CUR);

        fgets(prod[n], max, file);

        int len = strlen(prod[n]);
        if (len > 0 && prod[n][len - 1] == '\n')
        {
            prod[n][len - 1] = '\0';
        }

        if (indexOf(prod[n][0]) == -1)
        {
            nt[ntCount++] = prod[n][0];
        }
        n++;
    }

    for (int i = 0; i < ntCount; i++)
    {
        removeIndirect(i, nt[i]);
    }

    for (int i = 0; i < ntCount; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            alpha[i][j][0] = beta[i][j][0] = '\0';
        }
    }

    for (int i = 0; i < ntCount; i++)
    {
        findAlphaBeta(i, nt[i]);
    }

    for (int i = 0; i < ntCount; i++)
    {
        leftRec(i, nt[i]);
    }

    printf("-----Left Recursion free Grammar-----\n");
    for (int i = 0; i < newN; i++)
    {
        int j = 0;
        while (outProd[i][j] != '\0')
        {
            printf("%c", outProd[i][j]);
            j++;
        }
        printf("\n");
    }

    fclose(file);
    return 0;
}
