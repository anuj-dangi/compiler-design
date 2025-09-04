#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max 20 // Maximum number of productions and maximum length of each production

int n = 0;             // Number of productions read
char prod[max][max];   // Array to store productions

int ntCount = 0;       // Count of non-terminals
char nt[max];          // Array to store non-terminals

int tCount = 0;        // Count of terminals
char t[max];           // Array to store terminals

char first[max][max];  // FIRST sets for each non-terminal
char follow[max][max]; // FOLLOW sets for each non-terminal

char table[max][max][max]; // Predictive parsing table

typedef struct
{
	int top;
	char items[100];
}STACK;

void initialize(STACK *s)
{
	s->top = -1;
}

int isEmpty(STACK *s)
{
	return (s->top == -1);
}

int isFull(STACK *s)
{
	return (s->top == 99);
}

void push(STACK *s, char c)
{
	if(isFull(s))
	{
		printf("Error: Stack is full!\n");
		return;
	}
	s->top++;
	s->items[s->top] = c;
}

char pop(STACK *s)
{
	if(isEmpty(s))
	{
		printf("Error: Stack is Empty!\n");
		return (char)0;
	}
	return s->items[s->top--];
}

char peek(STACK *s)
{
	if(isEmpty(s))
	{
		printf("Error: Stack is Empty!\n");
		return (char)0;
	}
	return s->items[s->top];
}

// Check if a character is a non-terminal (A-Z)
int isNt(char c)
{
	return (c >= 'A' && c <= 'Z');
}

// Get the index of a non-terminal or terminal
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

// Check if character c is present in array arr[]
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

// Add character c to array arr[] if not already present
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

// Compute FIRST set of a non-terminal at index
void findFirst(int index, char c)
{
	if (!isNt(c)) // If terminal, add directly
	{
		add(first[index], c);
		return;
	}

	for (int i = 0; i < n; i++)
	{
		if (prod[i][0] == c) // For productions of this non-terminal
		{
			int j = 3; // Skip "A->"
			if (!isNt(prod[i][j])) // If first symbol in RHS is terminal
			{
				findFirst(index, prod[i][j]);
				continue;
			}

			while (prod[i][j] != '\0')
			{
				int k = indexOf(prod[i][j]);

				findFirst(k, prod[i][j]);

				int isEps = 0;

				for (int l = 0; first[k][l] != '\0'; l++)
				{
					if (first[k][l] == '~') // Check for epsilon
					{
						isEps = 1;
						continue;
					}
					add(first[index], first[k][l]);
				}

				if (!isEps)
				{
					break;
				}
				j++;
			}

			if (prod[i][j] == '\0') // All symbols can produce epsilon
			{
				add(first[index], '~');
			}
		}
	}
}

// Compute FOLLOW set of a non-terminal at index
void findFollow(int index, char c)
{
	if (index == 0) // Start symbol always contains $
	{
		add(follow[index], '$');
	}

	for (int i = 0; i < n; i++)
	{
		int j = 3; // Skip "A->"

		while (prod[i][j] != '\0')
		{
			int p = j + 1;
			if (prod[i][j] == c)
			{
				int needFollow = 1; // Flag to check if FOLLOW of LHS needed
				while (prod[i][p] != '\0')
				{
					if (!isNt(prod[i][p])) // Terminal after non-terminal
					{
						add(follow[index], prod[i][p]);
						needFollow = 0;
						break;
					}
					else
					{
						int k = indexOf(prod[i][p]);

						int isEsp = 0;

						for (int l = 0; first[k][l] != '\0'; l++)
						{
							if (first[k][l] == '~') // Check for epsilon
							{
								isEsp = 1;
								continue;
							}
							add(follow[index], first[k][l]);
						}

						if (!isEsp)
						{
							if (prod[i][p + 1] != '\0')
							{
								needFollow = 0;
							}
							break;
						}
					}

					p++;
				}
				if (needFollow) // Add FOLLOW of LHS if needed
				{
					int k = indexOf(prod[i][0]);

					if(k != index)
					{
						findFollow(k, prod[i][0]);
					}

					for (int l = 0; follow[k][l] != '\0'; l++)
					{
						add(follow[index], follow[k][l]);
					}
				}
			}
			j++;
		}
	}
}

// Compute FIRST of a RHS string and store in firstRHS[]
void findFirstOfRHS(char rhs[], char firstRHS[])
{
    firstRHS[0] = '\0';
    int i = 0;
    while (rhs[i] != '\0')
    {
        if (!isNt(rhs[i]))
        {
            add(firstRHS, rhs[i]);
            return;
        }
        int k = indexOf(rhs[i]);
        int isEps = 0;
        for (int l = 0; first[k][l] != '\0'; l++)
        {
            if (first[k][l] == '~')
                isEps = 1;
            else
                add(firstRHS, first[k][l]);
        }
        if (!isEps)
            return;
        i++;
    }
    add(firstRHS, '~'); // All symbols can produce epsilon
}

// Construct predictive parsing table for non-terminal at index
void findTable(int index)
{
	for(int i=0;i<n;i++)
	{
		if(prod[i][0] == nt[index])
		{
			char rhs[max];
			int j = 3;
			int p = 0;
			while(prod[i][j] != '\0')
			{
				rhs[p++] = prod[i][j++];
			}
			rhs[p] = '\0';

			char firstRHS[max];

			findFirstOfRHS(rhs, firstRHS);

			for(int j=0; firstRHS[j] != '\0'; j++)
			{
				if(firstRHS[j] == '~')  continue;
				if(table[index][indexOf(firstRHS[j])][0] != '\0')
				{
					perror("Error: predective parsing is not possible for this grammar\n");
					exit(1);
				}
				for(int l=3; prod[i][l] != '\0'; l++)
				{
					table[index][indexOf(firstRHS[j])][l-3] = prod[i][l];
					table[index][indexOf(firstRHS[j])][l-2] = '\0';
				}
			}
			if(isPresent(firstRHS, '~'))
			{
				for(int j=0; follow[index][j] != '\0'; j++)
				{
					if(table[index][indexOf(follow[index][j])][0] != '\0')
					{
						perror("Error: predective parsing is not possible for this grammar\n");
						exit(1);
					}
					for(int l=3; prod[i][l] != '\0'; l++)
					{
						table[index][indexOf(follow[index][j])][l-3] = prod[i][l];
						table[index][indexOf(follow[index][j])][l-2] = '\0';
					}
				}
			}
		}
	}
}

void parse()
{
	char str[max];
	printf("Enter the string to pasrse: ");
	fgets(str, max, stdin);
	str[strlen(str)-1] = '$';

	STACK stack;
	STACK *s = &stack;
	initialize(s);
	push(s, '$');
	push(s, nt[0]);

	int i = 0;
	while(str[i] != '\0')
	{
		if((peek(s) == str[i]) && str[i] == '$')
		{
			printf("String is accepted\n");
			return;
		}
		else if(peek(s) == str[i])
		{
			pop(s);
			i++;
		}
		else if(table[indexOf(peek(s))][indexOf(str[i])][0] != '\0')
		{
			char top = pop(s);
			if(table[indexOf(top)][indexOf(str[i])][0] == '~')
			{
				continue;
			}

			int j = strlen(table[indexOf(top)][indexOf(str[i])]);
			
			for(int k=j-1; k>=0; k--)
			{
				push(s, table[indexOf(top)][indexOf(str[i])][k]);
			}
		}
		else
		{
			printf("String cannot be passed given this grammar!\n");
			return;
		}
		printf("Stack: ");
		for(int j=0;j<stack.top;j++)
		{
			printf("%c", stack.items[j]);
		}
		printf(", Input: ");
		for(int j=i;j<strlen(str);j++)
		{
			printf("%c", str[j]);
		}
		printf("\n");
	}

	if(peek(s) != '$')
	{
		printf("String cannot be passed given this grammar!\n");
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

	// Read productions from file
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

	t[tCount++] = '$'; // Add end marker

	for (int i = 0; i < ntCount; i++)
	{
		follow[i][0] = first[i][0] = '\0';
	}

	for(int i=0;i<max;i++)
	{
		for(int j=0;j<max;j++)
		{
			table[i][j][0] = '\0';
		}
	}

	for (int i = 0; i < ntCount; i++)
	{
		findFirst(i, nt[i]);
	}

	for (int i = 0; i < ntCount; i++)
	{
		findFollow(i, nt[i]);
	}

    for (int i = 0; i < ntCount; i++)
	{
		findTable(i);
	}

	// Print predictive parsing table
	printf("\n-----Table-----\n\t");
    for(int i=0; t[i] != '\0'; i++)
    {
        printf("%c\t", t[i]);
    }
    printf("\n");

	for (int i = 0; i < ntCount; i++)
	{
		printf("%c\t", nt[i]);
		for (int j = 0; j < tCount; j++)
		{
			if (table[i][j][0] != '\0')
				printf("%s\t", table[i][j]);
			else
				printf("-\t");
    	}
		printf("\n");
	}

	parse();

	fclose(file);

	return 0;
}
