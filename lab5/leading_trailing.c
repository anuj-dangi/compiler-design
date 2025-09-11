#include <stdio.h>
#include <string.h>

#define max 20 // Maximum number of productions and maximum length of each production

int n = 0;			 // Number of productions read
char prod[max][max]; // Array to store productions

int ntCount = 0; // Count of non-terminals
char nt[max];	 // Array to store non-terminals

char lead[max][max];  // Array to store LEADING sets for each non-terminal
char trail[max][max]; // Array to store TRAILING sets for each non-terminal

int visi[max]; // Visited array (used for recursion and avoiding infinite loops)

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
			return 1; // Found → return true
		i++;
	}
	return 0; // Not found
}

// Add a character to an array if it is not already present
void add(char arr[], char c)
{
	if (!isPresent(arr, c)) // Avoid duplicates
	{
		int i = 0;
		while (arr[i] != '\0') // Move to end of string
		{
			i++;
		}

		arr[i] = c;	   // Insert character
		arr[i + 1] = '\0'; // Null terminate
	}
}

// Recursive function to compute LEADING set of a non-terminal
void findLead(int index, char c)
{
	if(visi[index]) // If already processed, return
	{
		return;
	}

	for(int i=0;i<n;i++) // Scan all productions
	{
		if(prod[i][0] == c) // If LHS = current non-terminal
		{
			int j = 3; // Start from RHS (after "A->")

			if(!isNt(prod[i][j])) // If first symbol is terminal → add directly
			{
				add(lead[index], prod[i][j]);
			}
			else // If first symbol is non-terminal
			{
				if(!isNt(prod[i][j+1])) // If second symbol exists and is terminal → add it
				{
					add(lead[index], prod[i][j+1]);
				}

				int k = indexOf(prod[i][j]); // Get index of non-terminal

				if(k != index) // Avoid self recursion
					findLead(k, prod[i][j]); // Recursively compute LEAD for that non-terminal

				// Copy everything from LEAD of that non-terminal
				for(int l=0; lead[k][l] != '\0'; l++)
				{
					add(lead[index], lead[k][l]);
				}
			}
		}
	}
	visi[index] = 1; // Mark as visited
}

// Recursive function to compute TRAILING set of a non-terminal
void findTrail(int index, char c)
{
	if(visi[index]) // If already processed, return
	{
		return;
	}

	for(int i=0;i<n;i++) // Scan all productions
	{
		if(prod[i][0] == c) // If LHS = current non-terminal
		{
			int j = 0;
			while( prod[i][j] != '\0' ) // Find end of production
			{
				j++;
			}

			// If last symbol is terminal → add directly
			if(!isNt(prod[i][j-1]))
			{
				add(trail[index], prod[i][j-1]);
			}
			else // If last symbol is non-terminal
			{
				// Check previous symbol (if exists and terminal) → add it
				if( j > 4  && !isNt(prod[i][j-2]) )
				{
					add(trail[index], prod[i][j-2]);
				}

				int k = indexOf(prod[i][j-1]); // Get index of non-terminal

				if(k != index) // Avoid self recursion
					findTrail(k, prod[i][j-1]); // Recursively compute TRAIL

				// Copy everything from TRAIL of that non-terminal
				for(int l=0; trail[k][l] != '\0'; l++)
				{
					add(trail[index], trail[k][l]);
				}
			}

		}
	}
	visi[index] = 1; // Mark as visited
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

		fgets(prod[n], max, file); // Read production line

		int len = strlen(prod[n]);
		if (len > 0 && prod[n][len - 1] == '\n')
		{
			prod[n][len - 1] = '\0'; // Remove newline
		}

		// If LHS non-terminal not already stored → add to nt[]
		if (indexOf(prod[n][0]) == -1)
		{
			nt[ntCount++] = prod[n][0];
		}
		n++; // Increase production count
	}

	// Initialize Leading and Trailing sets
	for (int i = 0; i < ntCount; i++)
	{
		trail[i][0] = lead[i][0] = '\0'; // Empty sets
		visi[i] = 0; // Reset visited
	}

	// Compute LEADING sets
	for(int i=0;i<ntCount;i++)
    {
        findLead(i, nt[i]);
    }

	// Reset visited for TRAILING computation
	for(int i=0;i<ntCount;i++)
    {
        visi[i] = 0;
    }

	// Compute TRAILING sets
	for(int i=0;i<ntCount;i++)
    {
		findTrail(i, nt[i]);
    }

	// Print LEADING sets
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

	// Print TRAILING sets
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

	fclose(file); // Close input file

	return 0;
}
