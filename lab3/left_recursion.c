#include <stdio.h>
#include <string.h>

#define max 20 // Maximum number of productions and maximum length of each production

int n = 0;			 // Number of productions read
char prod[max][max]; // Array to store input productions

char outProd[max][max]; // Array to store the output (left recursion free) productions

int ntCount = 0; // Count of non-terminals
char nt[max];	 // Array to store non-terminals

// Arrays to store α (recursive part) and β (non-recursive part) of productions
char alpha[max][5][max];
char beta[max][5][max];

char temp = 'Z';  // Temporary new non-terminal names start from 'Z' backwards
int newN = 0;     // Number of new productions generated

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

// Check if a character is already present in a given array
int isPresent(char arr[], char c)
{
	int i = 0;
	while (arr[i] != '\0')
	{
		if (arr[i] == c)
			return 1; // Found
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

// Separate productions of non-terminal c into alpha (if left-recursive) and beta (otherwise)
void findAlphaBeta(int index, char c)
{
    int ak = 0, bk = 0;
    for(int i=0;i<n;i++)
    {
        if(prod[i][0] == c)
        {
            int j = 3; // Skip over "A->"

            if(prod[i][j] == c) // Left recursion detected
            {
                for(int l=j+1; prod[i][l] != '\0'; l++)
                {
                    add(alpha[index][ak], prod[i][l]);
                }
                ak++;
            }
            else // Non-left-recursive production
            {
                for(int l=j; prod[i][l] != '\0'; l++)
                {
                    add(beta[index][bk], prod[i][l]);
                }
                bk++;
            }
        }
    }
}

// Generate new productions eliminating left recursion for non-terminal c
void leftRec(int index, char c)
{
    if(alpha[index][0][0] != '\0') // Left recursion exists
    {
        // For each β, create A -> βA'
        int i=0;
        do
        {
            outProd[newN][0] = c;
            outProd[newN][1] = '-';
            outProd[newN][2] = '>';

            int j = 3, k = 0;
            while(beta[index][i][k] != '\0')
            {
                outProd[newN][j] = beta[index][i][k];
                j++;
                k++;
            }
            outProd[newN][j++] = temp; // Append new non-terminal
            outProd[newN][j] = '\0';
            i++;
            newN++;
        }
        while(beta[index][i][0] != '\0');

        // For each α, create A' -> αA'
        i=0;
        while(alpha[index][i][0] != '\0')
        {
            outProd[newN][0] = temp;
            outProd[newN][1] = '-';
            outProd[newN][2] = '>';

            int j = 3, k = 0;
            while(alpha[index][i][k] != '\0')
            {
                outProd[newN][j] = alpha[index][i][k];
                j++;
                k++;
            }
            outProd[newN][j++] = temp; // Append itself
            outProd[newN][j] = '\0';
            i++;
            newN++;
        }

        // Add ε-production: A' -> ~ (using ~ to denote epsilon here)
        outProd[newN][0] = temp;
        outProd[newN][1] = '-';
        outProd[newN][2] = '>';
        outProd[newN][3] = '~';
        outProd[newN][4] = '\0';
        newN++;

        temp--; // Move to next temporary non-terminal
    }
    else // No left recursion, copy β-productions directly
    {
        int i=0;
        while(beta[index][i][0] != '\0')
        {
            outProd[newN][0] = c;
            outProd[newN][1] = '-';
            outProd[newN][2] = '>';

            int j = 3, k = 0;
            while(beta[index][i][k] != '\0')
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

	// Initialize α and β arrays
	for (int i = 0; i < ntCount; i++)
	{
        for(int j=0; j<5;j++)
        {
            alpha[i][j][0] = beta[i][j][0] = '\0'; // Empty sets
        }
	}

	// Separate productions into α and β parts
	for(int i=0;i<ntCount;i++)
    {
        findAlphaBeta(i, nt[i]);
    }

	// Generate left-recursion-free productions
	for(int i=0;i<ntCount;i++)
    {
		leftRec(i, nt[i]);
    }

	// Print the resulting grammar
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

	fclose(file); // Close input file
	return 0;
}
