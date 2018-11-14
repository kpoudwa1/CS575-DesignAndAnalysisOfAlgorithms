#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Global variables
//No. of items
int n = 0;
//Capacity of Knapsack
int capacity = 0;
int *profitArray, *weightArray, *bestset, *include;
float maxprofit = 0;
int num = 0;
int newN;

//Function declarations
void readFile(char *filePath);
void Knapsack();
void knapsack(int i, float profit, int weight);
int promising(int i, int weight, float profit);
float kwf4(int i, int weight, float profit, int weightArray[], int profitArray[], int C, int n);
void sort();
void freeMemory();

int main(int argc, char *argv[])
{
	//Check if the file is passed as an argument
	if(argc != 2)
	{
		printf("Invalid number of arguments\n");
		exit(1);
	}

	//Reading the contents of file
	readFile(argv[1]);

	//Sorting the items in non-ascending order of profit/weight ratio
	sort();

	//Knapsack function call
	Knapsack();

	//Freeing the memory
	freeMemory();

	printf("\n");
	return 0;
}

//Function for reading the file
void readFile(char *filePath)
{
	char str[100];
	char *token;

	//File pointer
	FILE *fp = NULL;
	fp = fopen(filePath, "r");

	//Check if the file is open
	if(fp != NULL)
	{
		//Get the number of items
		fgets(str, 100, fp);
		n = atoi(str);
		newN = n + 1;
		printf("Number of items %d", n);

		//Get the capacity of the knapsack
		fgets(str, 100, fp);
		capacity = atoi(str);
		printf("\nKnapsack capacity %d", capacity);

		//Allocate the profit, weight, bestset and include arrays on heap
		profitArray = malloc(sizeof(int) * newN);
		weightArray = malloc(sizeof(int) * newN);
		bestset = malloc(sizeof(int) * newN);
		include = malloc(sizeof(int) * newN);

		//Get the profits
		fgets(str, 100, fp);
		//Process the profits
		for(int i = 0; i < n; i++)
		{
			if(i == 0)
			{
				token = strtok(str, ",");
				profitArray[i + 1] = atoi(token);
			}
			else
			{
				token = strtok(NULL, ",");
				profitArray[i + 1] = atoi(token);
			}
		}

		//Get the weights
		fgets(str, 100, fp);
		//Process the weights
		for(int i = 0; i < n; i++)
		{
			if(i == 0)
			{
				token = strtok(str, ",");
				weightArray[i + 1] = atoi(token);
			}
			else
			{
				token = strtok(NULL, ",");
				weightArray[i + 1] = atoi(token);
			}
		}

		//Init to 0
		profitArray[0] = 0;
		weightArray[0] = 0;
		for(int i = 0; i < newN; i++)
		{
			bestset[0] = 0;
		}   include[0] = 0;
	}
	else
	{
		printf("Error: Cannot open file !\n");
		exit(1);
	}

	//Closing the file pointer
	if(fp != NULL)
		fclose(fp);
}

//Knapsack function
void Knapsack()
{
	int num = 0;
	maxprofit = 0;

	knapsack(0, 0, 0);

	//Printing the final solution
	int wt = 0, pt = 0;
	printf("\n\nSelected items,");
	printf("\nProfit\tWeight");
	printf("\n=================");
	for (int i = 1; i <= n; i++)
	{
		if(bestset[i] == 1)
		{
			printf("\n%d\t%d ", profitArray[i], weightArray[i]);
			wt += weightArray[i];
			pt += profitArray[i];
		}
	}
	printf("\nMaximum profit %d", pt);
	printf("\nMaximum weight %d", wt);
}

//Recursive function for including items in the Knapsack
void knapsack(int i, float profit, int weight)
{
	if(weight <= capacity && profit > maxprofit)
	{
		//Save the solution
		maxprofit = profit;
		num = i;

		for(int j = 0; j <= n; j++)
		{
			bestset[j] = include[j];
		}
	}

	if(promising(i, weight, profit))
	{
		include[i + 1] = 1;
		knapsack(i + 1, profit + (profitArray[i + 1]), weight + (weightArray[i + 1]));
		include[i + 1] = 0;
		knapsack(i + 1, profit, weight);
	}
}

//Promising function
int promising(int i, int weight, float profit)
{
	if(weight >= capacity)
		return 0;

	int bound = kwf4(i+1, weight, profit, weightArray, profitArray, capacity, n);

	return bound > maxprofit;
}

//Function for calculating the upper bound
float kwf4(int i, int weight, float profit, int weightArray[], int profitArray[], int C, int n)
{
	float bound = profit;
	float *x = malloc(sizeof(float) * newN);

	for(int j = 0; j < n; j++)
		x[j] = 0;

	while(weight < C && i <= n)
	{
		if(weight + weightArray[i] <= C)
		{
			x[i] = 1;
			weight = weight + weightArray[i];
			bound = bound + profitArray[i];
		}
		else
		{
			x[i] = (C - weight) / weightArray[i];
			weight = C;
			bound = bound + profitArray[i] * x[i];
		}
		i++;
	}

	//Free memory
	free(x);

	return bound;
}

//Function for sorting the items in non-ascending order of profit/weight ratio using bubble sort
void sort()
{
	float *ratio = malloc(sizeof(float) * newN);

	for(int i = 0; i < newN; i++)
	{
		ratio[i] = (float) profitArray[i] / weightArray[i];
	}

	for(int j = 0; j < newN; j++)
	{
		for(int i = 1; i < newN; i++)
		{
			if(ratio[i - 1] < ratio[i])
			{
				int temp = ratio[i - 1];
				ratio[i - 1] = ratio[i];
				ratio[i] = temp;

				temp = profitArray[i - 1];
				profitArray[i - 1] = profitArray[i];
				profitArray[i] = temp;

				temp = weightArray[i - 1];
				weightArray[i - 1] = weightArray[i];
				weightArray[i] = temp;
			}
		}
	}

	//Free the memory
	free(ratio);
}

//Function for freeing the dynamically allocated memory
void freeMemory()
{
	free(profitArray);
	free(weightArray);
	free(bestset);
	free(include);
}