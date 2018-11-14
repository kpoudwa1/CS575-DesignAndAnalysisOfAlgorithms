#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Global variables
//Capacity of Knapsack
int capacity = 0;
//No. of items
int n = 0;

//Function declarations
void bruteForce();
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

	//Brute Force function call
	bruteForce();

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
	if (fp != NULL)
	{
		//Get the number of items
		fgets(str, 100, fp);
		n = atoi(str);
		printf("Number of items %d", items);

		//Allocate the items on heap
		items = malloc(n * sizeof(struct Items));

		//Get the capacity of the knapsack
		fgets(str, 100, fp);
		capacity = atoi(str);
		printf("\nKnapsack capacity %d", capacity);

		//Get the profits
		fgets(str, 100, fp);
		//Process the profits
		for (int i = 0; i < n; i++)
		{
			if (i == 0)
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
		for (int i = 0; i < n; i++)
		{
			if (i == 0)
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
	if (fp != NULL)
		fclose(fp);
}

//Function for performing bruteforce Knapsack
void bruteForce()
{
	int selectedItemsSize, finalProfit, calculatedWeight, maxprofit, maxWeight;

	//Init variables
	selectedItemsSize = 0;
	finalProfit = 0;
	calculatedWeight = 0;
	maxprofit = 0;
	maxWeight = 0;

	int power = pow(2, n);
	int selectedItems[power];

	//Init array to -1
	for(int h = 0; h < power; h++)
	{
		selectedItems[h] = -1;
	}

	//Generating the subsets
	for(int i = 0; i < power; i++)
	{
		calculatedWeight = finalProfit = 0;
		int itemArray[n], counter = 0;

		//Looping all possible combination with number of items
		for(int j = 0; j < n; j++)
		{
			//Selecting an item, when bit matches for i i.e. a possible combination and j i.e item
			if(i & (1 << j))
			{
				int tempWeightValue = items[j].weight + calculatedWeight;
				if(capacity >= tempWeightValue)
				{
					calculatedWeight = calculatedWeight + items[j].weight;
					finalProfit = finalProfit + items[j].profit;
					itemArray[counter] = j;
					counter++;
				}
			}
		}

		//Getting maximum profit and weight value
		if(finalProfit > maxprofit)
		{
			if(maxWeight <= capacity)
			{
				//Taking max profit amount
				maxprofit = finalProfit;

				//Taking max weight
				maxWeight = calculatedWeight;

				//Assigning -1 as default value
				for(int h = 0; h < power; h++)
				{
					selectedItems[h] = -1;
				}
				for(int k = 0; k < counter; ++k)
				{
					selectedItems[k] = itemArray[k];
				}

				//Decreasing the size
				selectedItemsSize = counter - 1;
			}
		}
	}

	printf("\n\nSelected items,");
	printf("\nProfit\tWeight");
	printf("\n================");
	if (maxprofit > 0)
	{
		for(int i = 0; i <= selectedItemsSize; i++)
		{
			if (selectedItems[i] != -1)
			{
				printf("\n%d\t%d", items[selectedItems[i]].profit, items[selectedItems[i]].weight);
			}
		}
	}
	printf("\nMaximum profit %d", maxprofit);
	printf("\nMaximum weight %d", maxWeight);
}

//Function for freeing memory
void freeMemory()
{
	free(items);
}
