#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 50 

int n = 0;			 
char prod[max][max]; 

int ntCount = 0; 
char nt[max];	 

int tCount = 0;
char t[max];

char lead[max][max];  
char trail[max][max]; 
int visi[max];

char table[max][max][1] = {'\0'};      //row->terminal count, column->terminal count

int isNt(char c)
{
	return (c >= 'A' && c <= 'Z');
}

int indexOf(char c)
{
    if(isNt(c))
    {
        for (int i = 0; i < ntCount; i++)
        {
            if (nt[i] == c)
            {
                return i;
            }
        }
    }
	else
    {
        for (int i = 0; i < tCount; i++)
        {
            if (t[i] == c)
            {
                return i;
            }
        }
    }

	return -1;
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

void findLead(int index, char c)
{
	if(visi[index]) 
	{
		return;
	}

	for(int i=0;i<n;i++) 
	{
		if(prod[i][0] == c) 
		{
			int j = 3; 

			if(!isNt(prod[i][j])) 
			{
				add(lead[index], prod[i][j]);
			}
			else 
			{
				if(!isNt(prod[i][j+1])) 
				{
					add(lead[index], prod[i][j+1]);
				}

				int k = indexOf(prod[i][j]); 

				if(k != index) 
					findLead(k, prod[i][j]); 

				
				for(int l=0; lead[k][l] != '\0'; l++)
				{
					add(lead[index], lead[k][l]);
				}
			}
		}
	}
	visi[index] = 1; 
}

void findTrail(int index, char c)
{
	if(visi[index]) 
	{
		return;
	}

	for(int i=0;i<n;i++) 
	{
		if(prod[i][0] == c) 
		{
			int j = 0;
			while( prod[i][j] != '\0' ) 
			{
				j++;
			}

			
			if(!isNt(prod[i][j-1]))
			{
				add(trail[index], prod[i][j-1]);
			}
			else 
			{
				
				if( j > 4  && !isNt(prod[i][j-2]) )
				{
					add(trail[index], prod[i][j-2]);
				}

				int k = indexOf(prod[i][j-1]); 

				if(k != index) 
					findTrail(k, prod[i][j-1]); 

				
				for(int l=0; trail[k][l] != '\0'; l++)
				{
					add(trail[index], trail[k][l]);
				}
			}

		}
	}
	visi[index] = 1; 
}

void findTable()
{
    for(int i=0;i<n;i++)
    {
        int j=3;
        char a, b;
        while((a=prod[i][j]) != '\0' && (b=prod[i][j+1]) != '\0')
        {
            if(!isNt(a) && !isNt(b))
            {
                if(table[indexOf(a)][indexOf(b)][0] != '\0')
                {
                    printf("Error: two entry at same position\nOperator table is not possible\n");
                    exit(1);
                }
                table[indexOf(a)][indexOf(b)][0] = '=';
            }
            else if(!isNt(a) && isNt(b))
            {
                int index=indexOf(a);
                for(int k=0;k<strlen(lead[indexOf(b)]);k++)
                {
					if(table[index][indexOf(lead[indexOf(b)][k])][0] != '\0')
					{
						printf("Error: two entry at same position\nOperator table is not possible\n");
						exit(1);
					}
                    table[index][indexOf(lead[indexOf(b)][k])][0] = '<';
                }

				if(prod[i][j+2] != '\0' && !isNt(prod[i][j+2]))
                {
					if(table[index][indexOf(prod[i][j+2])][0] != '\0')
					{
						printf("Error: two entry at same position\nOperator table is not possible\n");
						exit(1);
					}
                    table[index][indexOf(prod[i][j+2])][0] = '=';
                }
            }
            else
            {
                int index=indexOf(b);
                for(int k=0;k<strlen(trail[indexOf(a)]);k++)
                {
					if(table[indexOf(trail[indexOf(a)][k])][index][0] != '\0')
					{
						printf("%c", table[indexOf(trail[indexOf(b)][k])][index][0]);
						printf("Error: two entry at same position\nOperator table is not possible\n");
						exit(1);
					}
                    table[indexOf(trail[indexOf(a)][k])][index][0] = '>';
                }
            }
            j++;
        }
    }

    int index=indexOf('$');
    for(int k=0;k<strlen(lead[indexOf(nt[0])]);k++)
    {
		if(table[index][indexOf(lead[indexOf(nt[0])][k])][0] != '\0')
		{
			printf("Error: two entry at same position\nOperator table is not possible\n");
			exit(1);
		}
        table[index][indexOf(lead[indexOf(nt[0])][k])][0] = '<';
    }
    for(int k=0;k<strlen(trail[indexOf(nt[0])]);k++)
    {
		if(table[indexOf(trail[indexOf(nt[0])][k])][index][0] != '\0')
		{
			printf("Error: two entry at same position\nOperator table is not possible\n");
			exit(1);
		}
        table[indexOf(trail[indexOf(nt[0])][k])][index][0] = '>';
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

        for(int i=3;i<len-1;i++)
        {
            if(prod[n][i] == '~')   continue;
            if(!isNt(prod[n][i]) && indexOf(prod[n][i]) == -1)
            {
                t[tCount++] = prod[n][i];
            }
        }

		if (indexOf(prod[n][0]) == -1)
		{
			nt[ntCount++] = prod[n][0];
		}
		n++; 
	}

	for (int i = 0; i < ntCount; i++)
	{
		trail[i][0] = lead[i][0] = '\0'; 
		visi[i] = 0; 
	}

	for(int i=0;i<ntCount;i++)
    {
        findLead(i, nt[i]);
    }

	for(int i=0;i<ntCount;i++)
    {
        visi[i] = 0;
    }

	for(int i=0;i<ntCount;i++)
    {
		findTrail(i, nt[i]);
    }

    t[tCount++] = '$';

    findTable();
	table[tCount-1][tCount-1][0] = 'A';

	printf("-----Leading Sets-----\n");
	for (int i = 0; i < ntCount; i++)
	{
		printf("LEADING(%c) : { ", nt[i]);
		int j = 0;
		while (lead[i][j] != '\0')
		{
			printf("%c ", lead[i][j]);
			j++;
		}
		printf("}\n");
	}

	printf("\n-----Trailing Sets-----\n");
	for (int i = 0; i < ntCount; i++)
	{
		printf("TRAILING(%c) : { ", nt[i]);
		int j = 0;
		while (trail[i][j] != '\0')
		{
			printf("%c ", trail[i][j]);
			j++;
		}
		printf("}\n");
	}
	
    printf("\n-----Operator Precedence Table-----\n\t");
    for(int k=0;k<tCount;k++)
    {
        printf("%c\t", t[k]);
    }
    printf("\n");
	for (int i = 0; i < tCount; i++)
	{
        printf("%c\t", t[i]);
        for(int k=0;k<tCount;k++)
        {
            if(table[i][k][0] == '\0')
            {
                printf("-\t");
            }
            else
                printf("%c\t", table[i][k][0]);
        }
        printf("\n");
	}

	fclose(file); 

	return 0;
}
