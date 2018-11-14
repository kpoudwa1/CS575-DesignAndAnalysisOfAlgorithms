#include <stdio.h>
#include<stdlib.h>
#include<math.h>

//Variable declarations
int **root;
int bstArray[100];
int **matrix;
int treeDepth = -1;

//Function delcarations
int ** generateRoot(int rows);
int createArrayRep(int i, int j, int n, int depth);
int optimalSearchTree(int n, float prob[]);
void displayOBST();
void freeMemory(int rows);

int main(int argc, char *argv[])
{
	//Check if the number of arguments passed is valid
	if(argc < 3)
	{
		printf("Invalid number of arguments\n");
		exit(1);
	}

    int n = atoi(argv[1]);
    int size = argc;
    float *prob;
    prob = malloc(sizeof(float) * n);
    if(prob == NULL)
    {
        exit(1);
    }
    root = generateRoot(n + 3);
    if (n != size - 2)
	{
        printf("Insufficient probabilities\n");
        exit(1);
    }
    //Take all cost values in prob array
    for(int i = 2; i < size; i++)
	{
        prob[i - 2] = atof(argv[i]);
    }
	
    optimalSearchTree(n, prob);
	
	//Displaying the data
	displayOBST();
	
    freeMemory(n + 3);
    free(prob);
    return 0;
}

//Function for creating OBST
int optimalSearchTree(int n, float prob[])
{
    int i, k, addValue, row = 1, column = 2;
    float cost[n + 2][n + 1];
    float probValue = 0, tempValue = 0;
    //Adding 0 to diagonal elements
    for (i = 1; i <= n + 1; i++)
        cost[i][i - 1] = 0;
	
    //Mapping probabilities/cost to their respective keys
    for(i = 1; i <= n; i++)
	{
        cost[i][i] = prob[i - 1];
        root[i][i] = i;
    }

	//Iteration for calculation
    addValue = 0;
    cost[1][n] = 0;
    while(cost[1][n] == 0)
	{
        column = column + addValue;
        while (row < (n - addValue))
		{
            //Diagonal solving start
            int counter = 0; // for storing cost 1st time.
            //Calculate prob value
            probValue = 0;
            for(int l = row; l <= column; l++)
                probValue = probValue + prob[l - 1];

            for(k = row; k <= column; k++)
			{
                counter++;
                tempValue = cost[row][k - 1] + cost[k + 1][column] + probValue;
                if(tempValue <= cost[row][column] || counter == 1)
				{
                    cost[row][column] = tempValue;
                    root[row][column] = k;
                }
            }
            column++;
            row++;
            //Diagonal solving end
        }
        addValue++;
        column = 2;
        row = 1;
    }
	
	//Function call for creating array representation of BST
    treeDepth = 0;
    createArrayRep(1, n, 0, 0);
}

//Recusive function to create array representation of the BST
int createArrayRep(int i, int j, int n, int depth)
{
    if(i <= j)
	{
		//Update the depth of the tree
        if (depth > treeDepth)
            treeDepth = depth;

        int value = root[i][j];
        bstArray[n] = value;
        bstArray[2 * n + 1] = createArrayRep(i, value - 1, 2 * n + 1, depth + 1);
        bstArray[2 * n + 2] = createArrayRep(value + 1, j, 2 * n + 2, depth + 1);
        return value;
    }
	else
	{
        return 0;
    }
}

//Function to display the OBST
void displayOBST()
{
	int start = 0;
	printf("The Optimal Binary Search Tree is a follows,\n");
    //Code to print the depth values
    for(int i = 0; i <= treeDepth; i++)
	{
        int end = (int) pow(2, i) + start;
        printf("depth %d: ", i);
        for(int j = start; j < end; j++)
		{
            if(bstArray[j] != -1)
			{
                printf("%d, ", bstArray[j]);
                start = j + 1;
            }
        }
    }
    printf("\n");
}

//Function to generate root Matrix
int ** generateRoot(int rows)
{
	//Allocate the rows
    matrix = malloc(rows * sizeof(int *));

    //Allocate the columns
    for (int i = 0; i < rows; i++)
	{
        matrix[i] = malloc((rows) * sizeof(int));
        for (int j = 0; j < rows; j++)
            matrix[i][j] = 0;
    }
	
	//Initializing the BST array representation
	for (int i = 0; i < 100; i++)
	{
        bstArray[i] = -1;
    }
	
    return matrix;
}

//Function for freeing the allocated memory
void freeMemory(int rows)
{
	//Freeing the columns
	for(int i = 0; i < rows; i++)
		free(root[i]);

	//Freeing the rows
	free(root);
}