#include <stdio.h>
#include <string.h>

#define max 20 // Maximum number of productions and maximum length of each production

int n = 0;			 // Number of productions read
char prod[max][max]; // Array to store productions

int ntCount = 0; // Count of non-terminals
char nt[max];	 // Array to store non-terminals

char first[max][max];  // Array to store FIRST sets for each non-terminal
char follow[max][max]; // Array to store FOLLOW sets for each non-terminal

// Returns the index of a non-terminal in the nt array
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

// Check if a character is a non-terminal (A-Z)
int isNt(char c)
{
	return (c >= 'A' && c <= 'Z');
}

// Check if a character is already present in a given array
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

// Add a character to an array if it is not already present
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

// Recursive function to compute FIRST set for a given non-terminal
void findFirst(int index, char c)
{
	// If it's a terminal, add directly to FIRST set
	if (!isNt(c))
	{
		add(first[index], c);
		return;
	}

	// Loop through all productions
	for (int i = 0; i < n; i++)
	{
		if (prod[i][0] == c) // If production's LHS matches
		{
			int j = 3; // Skip "A->" part
			if (!isNt(prod[i][j]))
			{
				findFirst(index, prod[i][j]);
				continue;
			}

			while (prod[i][j] != '\0')
			{
				int k = indexOf(prod[i][j]); // Get index of next non-terminal

				findFirst(k, prod[i][j]); // Recursive call for next non-terminal

				int isEps = 0;

				// Add all symbols from FIRST of next non-terminal to current FIRST
				for (int l = 0; first[k][l] != '\0'; l++)
				{
					if (first[k][l] == '~') // Check for epsilon
					{
						isEps = 1;
						continue;
					}
					add(first[index], first[k][l]);
				}

				if (!isEps) // Stop if epsilon not present
				{
					break;
				}
				j++;
			}

			if (prod[i][j] == '\0') // If all symbols can produce epsilon
			{
				add(first[index], '~');
			}
		}
	}
}

// Recursive function to compute FOLLOW set for a given non-terminal
void findFollow(int index, char c)
{
	if (index == 0) // Start symbol always contains $
	{
		add(follow[index], '$');
	}

	// Loop through all productions
	for (int i = 0; i < n; i++)
	{
		int j = 3; // Skip "A->" part

		while (prod[i][j] != '\0')
		{
			int t = j + 1;
			if (prod[i][j] == c) // If non-terminal found in RHS
			{
				int needFollow = 1; // Flag to determine if FOLLOW of LHS needed
				while (prod[i][t] != '\0')
				{
					if (!isNt(prod[i][t])) // Terminal after non-terminal
					{
						add(follow[index], prod[i][t]);
						needFollow = 0;
						break;
					}
					else
					{
						int k = indexOf(prod[i][t]); // Index of next non-terminal

						int isEsp = 0;

						// Add FIRST of next non-terminal
						for (int l = 0; first[k][l] != '\0'; l++)
						{
							if (first[k][l] == '~')
							{
								isEsp = 1;
								continue;
							}
							add(follow[index], first[k][l]);
						}

						if (!isEsp)
						{
							if (prod[i][t + 1] != '\0')
							{
								needFollow = 0;
							}
							break;
						}
					}

					t++;
				}
				if (needFollow) // If epsilon or end of production, add FOLLOW of LHS
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

int main(int argc, char *argv[])
{
	if (argc != 2) // Check for input file argument
	{
		perror("error: file name is not given\n");
		return 1;
	}

	FILE *file = fopen(argv[1], "r");

	if (file == NULL) // Check if file can be opened
	{
		perror("Error: cannot open file\n");
		return 1;
	}

	int c;

	// Read productions from file
	while ((c = fgetc(file)) != EOF)
	{
		fseek(file, -1, SEEK_CUR); // Move back one character

		fgets(prod[n], max, file);

		int len = strlen(prod[n]);
		if (len > 0 && prod[n][len - 1] == '\n')
		{
			prod[n][len - 1] = '\0'; // Remove newline
		}

		if (indexOf(prod[n][0]) == -1) // Add non-terminal if new
		{
			nt[ntCount++] = prod[n][0];
		}
		n++;
	}

	// Initialize FIRST and FOLLOW sets
	for (int i = 0; i < ntCount; i++)
	{
		follow[i][0] = first[i][0] = '\0';
	}

	// Compute FIRST sets
	for (int i = 0; i < ntCount; i++)
	{
		findFirst(i, nt[i]);
	}

	// Compute FOLLOW sets
	for (int i = 0; i < ntCount; i++)
	{
		findFollow(i, nt[i]);
	}

	// Print FIRST sets
	printf("-----First Sets-----\n");
	for (int i = 0; i < ntCount; i++)
	{
		printf("FIRST(%c) : { ", nt[i]);
		int j = 0;
		while (first[i][j] != '\0')
		{
			printf("%c ", first[i][j]);
			j++;
		}
		printf("}\n");
	}

	// Print FOLLOW sets
	printf("\n-----Follow Sets-----\n");
	for (int i = 0; i < ntCount; i++)
	{
		printf("FOLLOW(%c) : { ", nt[i]);
		int j = 0;
		while (follow[i][j] != '\0')
		{
			printf("%c ", follow[i][j]);
			j++;
		}
		printf("}\n");
	}

	fclose(file);

	return 0;
}
