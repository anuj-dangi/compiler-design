#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 50 

int n = 1;			 
char prod[max][max]; 

int ntCount = 0; 
char nt[max];	 

int tCount = 0;
char t[max];

typedef struct
{
	char prod[max][max];
	int count;
	int already;
	char sym;
	int transition; 
}Item;

Item items[max];
int item_count = 0;

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

void addNewProd(Item* temp, char c)
{
    for(int i=0;i<n;i++)
    {
        if(prod[i][0] == c)
        {
            int k = 0;
            temp->prod[temp->count][k++] = prod[i][0];  
            temp->prod[temp->count][k++] = prod[i][1];  
            temp->prod[temp->count][k++] = prod[i][2];  
            temp->prod[temp->count][k++] = '.';         
            
            for(int j=3; prod[i][j] != '\0'; j++)
            {
                temp->prod[temp->count][k++] = prod[i][j];
            }
            temp->prod[temp->count][k] = '\0';
            temp->count++;
        }
    }
}

void addOldProd(Item *a, Item *b, char c, int source_item_index)
{
    b->count = 0;
    b->transition = source_item_index;  
    b->sym = c;
    b->already = 0;  

    for (int i = 0; i < a->count; i++)
    {
        char *prod_str = a->prod[i];
        int len = strlen(prod_str);

        int dot_pos = -1;
        for (int j = 0; j < len; j++)
        {
            if (prod_str[j] == '.')
            {
                dot_pos = j;
                break;
            }
        }

        if (dot_pos != -1 && dot_pos + 1 < len && prod_str[dot_pos + 1] == c)
        {
            char arr[max] = {0};
            int k = 0;

            for (int j = 0; j < len; j++)
            {
                if (j == dot_pos)
                {
                    arr[k++] = prod_str[j + 1];  
                    arr[k++] = '.';             
                    j++;                        
                }
                else
                {
                    arr[k++] = prod_str[j];
                }
            }

            arr[k] = '\0';
            strcpy(b->prod[b->count++], arr);
        }
    }
}

int checkSame(Item* a)
{
    for(int i = 0; i < item_count; i++)
    {
        Item* temp = &items[i];

        if(temp->count != a->count) continue;

        int match = 1;
        for(int j = 0; j < a->count; j++)
        {
            if(strcmp(a->prod[j], temp->prod[j]) != 0)
            {
                match = 0;
                break;
            }
        }

        if(match)
        {
            a->already = i;
            return 1;
        }
    }
    return 0;
}

void computeClosure(Item* a)
{
   
    int prev_count;
    do 
	{
        prev_count = a->count;
        
        for(int i = 0; i < a->count; i++)
        {
            int dot_pos = -1;
            for(int j = 0; a->prod[i][j] != '\0'; j++)
            {
                if(a->prod[i][j] == '.')
                {
                    dot_pos = j;
                    break;
                }
            }
            
            if(dot_pos != -1 && a->prod[i][dot_pos + 1] != '\0' && isNt(a->prod[i][dot_pos + 1]))
            {
                char nextNt = a->prod[i][dot_pos + 1];
                
                for(int k = 0; k < n; k++)
                {
                    if(prod[k][0] == nextNt)
                    {
                        char newProd[max];
                        int idx = 0;
                        newProd[idx++] = prod[k][0];
                        newProd[idx++] = prod[k][1];
                        newProd[idx++] = prod[k][2];
                        newProd[idx++] = '.';
                        for(int j = 3; prod[k][j] != '\0'; j++)
                        {
                            newProd[idx++] = prod[k][j];
                        }
                        newProd[idx] = '\0';
                        
                        int found = 0;
                        for(int m = 0; m < a->count; m++)
                        {
                            if(strcmp(a->prod[m], newProd) == 0)
                            {
                                found = 1;
                                break;
                            }
                        }
                        
                        if(!found)
                        {
                            strcpy(a->prod[a->count++], newProd);
                        }
                    }
                }
            }
        }
    } while(a->count != prev_count); 
}

void closure()
{
    int k = 0;
    while (k < item_count)
    {
        Item* a = &items[k];

        char symbols[max];
        int sym_count = 0;
        
        for (int i = 0; i < a->count; i++)
        {
            for (int j = 0; a->prod[i][j] != '\0'; j++)
            {
                if (a->prod[i][j] == '.' && a->prod[i][j + 1] != '\0')
                {
                    char next_sym = a->prod[i][j + 1];
                    
                    int found = 0;
                    for(int m = 0; m < sym_count; m++)
                    {
                        if(symbols[m] == next_sym)
                        {
                            found = 1;
                            break;
                        }
                    }
                    if(!found)
                    {
                        symbols[sym_count++] = next_sym;
                    }
                    break;
                }
            }
        }
        
        for(int s = 0; s < sym_count; s++)
        {
            char sym = symbols[s];
            Item b = {0};
            
            addOldProd(a, &b, sym, k);
            
            if(b.count > 0)  
            {
                computeClosure(&b);

                if (!checkSame(&b))
                {
                    items[item_count++] = b;

                    printf("I%d", item_count-1);
                    if(b.transition != -1)
                    {
                        printf(" : goto (I%d, %c)\n",  b.transition, b.sym);
                    }
                    else
                    {
                        printf("\n");
                    }

                    for(int j=0;j<b.count;j++)
                    {
                        printf("%s\n", b.prod[j]);
                    }
                    printf("\n");
                }
                else
                {
                    printf("goto (I%d, %c)\n",  b.transition, b.sym);

                    printf("same as I%d\n\n", b.already);
                }
            }
        }

        k++;
    }
}

int main(int argc, char *argv[])
{
	if (argc != 2) 
	{
		fprintf(stderr, "error: file name is not given\n");
		return 1;
	}

	FILE *file = fopen(argv[1], "r");

	if (file == NULL) 
	{
		perror("Error: cannot open file");
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
			len--;
		}

        for(int i=3;i<len;i++)
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

	t[tCount] = '\0';
	nt[ntCount] = '\0';

	prod[0][0] = 'Z';
	prod[0][1] = '-';
	prod[0][2] = '>';
	prod[0][3] = prod[1][0];
	prod[0][4] = '\0';

	Item temp = {0};
	temp.transition = -1;
	temp.already = 0;
	addNewProd(&temp, 'Z');
	computeClosure(&temp);
	
	items[item_count++] = temp;

    printf("I%d\n",0);

    for(int j=0;j<items[0].count;j++)
    {
        printf("%s\n", items[0].prod[j]);
    }
    printf("\n");
	closure();

	fclose(file); 

	return 0;
}