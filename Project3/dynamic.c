#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Global variables
//Capacity of Knapsack
int capacity = 0;
//No. of items
int n = 0;

//Function declarations
void dynamic();
void readFile(char *filePath);
void freeMemory();

//Struct for knapsack items
typedef struct Items
{
	int weight;
	int profit;
} Items;
Items *items;

int main(int argc, char *argv[])
{
	//Check if the file is passed as an argument
	if(argc != 2)
	{
		printf("Invalid number of arguments\n");
		exit(1);
	}

	//Reading the file
	readFile(argv[1]);

	//Dynamic programming function call
	dynamic();

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
		printf("Number of items %d", n);

		//Allocate the items on heap
		items = malloc(n * sizeof(struct Items));

		//Get the capacity of the knapsack
		fgets(str, 100, fp);
		capacity = atoi(str);
		printf("\nKnapsack capacity %d", capacity);

		//Get the profits
		fgets(str, 100, fp);
		//Process the profits
		for(int i = 0; i < n; i++)
		{
			if(i == 0)
			{
				token = strtok(str, ",");
				items[i].profit = atoi(token);
			}
			else
			{
				token = strtok(NULL, ",");
				items[i].profit = atoi(token);
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
				items[i].weight = atoi(token);
			}
			else
			{
				token = strtok(NULL, ",");
				items[i].weight = atoi(token);
			}
		}
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

//Function for performing dynamic Knapsack
void dynamic()
{
	int finalWeight = 0;
	int finalProfit = 0;
	int selectedItemsArray[n + 1][capacity + 1];

	//Initializing first row and column to 0
	for(int i = 0;i <= n; i++)
	{
		for(int j = 0; j <= capacity; j++)
		{
			selectedItemsArray[i][0] = 0;
			selectedItemsArray[0][j] = 0;
		}
	}

	for(int k = 1; k <= n; k++)
	{
		for(int w = 1; w <= capacity; w++)
		{
			if(items[k - 1].weight <= w && (selectedItemsArray[k - 1][w - items[k - 1].weight] + items[k - 1].profit > selectedItemsArray[k - 1][w]))
			{
				selectedItemsArray[k][w] = selectedItemsArray[k - 1][w - items[k - 1].weight] + items[k - 1].profit;
			}
			else
				selectedItemsArray[k][w] = selectedItemsArray[k - 1][w];
		}
	}

	printf("\n\nSelected items are");
	printf("\nProfit\tWeight");
	printf("\n===================");
	for(int i = n ; i >0 ; i--)
	{
		if(selectedItemsArray[i][capacity] !=selectedItemsArray[i - 1][capacity])
		{
			printf("\n%d\t%d", items[i - 1].profit, items[i - 1].weight);

			//Calculating final weight
			finalWeight += items[i - 1].weight;
			capacity = capacity-items[i - 1].weight;

			//Calculating final profit
			finalProfit += items[i - 1].profit;
		}
	}
	printf("\nMaximum weight %d", finalWeight);
	printf("\nMaximum profit %d", finalProfit);
}

//Function for freeing memory
void freeMemory()
{
	free(items);
}
