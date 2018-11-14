#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Global variable for number of records
int noOfRecords = 0;
//Function delcarations
int ** createGraph(char *filePath);
void freeMemory(int **graph, int records);
void calculateShortestPath(int **graph);
void display(int **graph);

int main(int argc, char *argv[])
{
	//Create the graph
	int **graph = createGraph(argv[1]);

	if(graph != NULL)
	{
		calculateShortestPath(graph);

		//Free the memory allocated to the graph
		freeMemory(graph, noOfRecords);
	}

	printf("\n");
	return 0;
}

//Function for creating dynamic graph
int ** createGraph(char *filePath)
{
	char str[100];
	int recordCount = 0;
	int **graph = NULL;

	//File pointer
	FILE *fp;
	fp = fopen(filePath, "r");

	if(fp != NULL)
	{
		//Getting the number of vertices
		while(fgets(str, 100, fp) != NULL)
			recordCount++;

		//Assign the number of vertices to global variable
		noOfRecords = recordCount;

		//Printing the number of vertices & closing the file pointer
		printf("Number of records: %d", recordCount);
		fclose(fp);

		//Create a double pointer for the rows
		graph = malloc(recordCount * sizeof(int *));

		//Allocate the columns
		for(int i = 0; i < recordCount; i++)
			graph[i] = malloc(recordCount * sizeof(int));

		//Reopening the file for processing the records
		fp = fopen(filePath, "r");
		if(fp != NULL)
		{
			char *token;
			int rowPos = 0;

			while(fgets(str, 100, fp) != NULL)
			{
				//Read the element at 0 index
				token = strtok(str, ",");
				graph[rowPos][0] = atoi(token);

				//Check if the adjacency matrix contains value greater the 500
				if(atoi(token) > 500)
				{
					printf("\nError: The adjacency matrix contains value greater then 500", (atoi(token)));
					//Free the memory allocated
					freeMemory(graph, noOfRecords);
					//Closing the file
					fclose(fp);

					return NULL;
				}

				int colPos = 1;
				//Read the data in columns
				while(colPos < recordCount)
				{
					token = strtok(NULL, ",");

					//Check if the data is present for the given column
					if(token == NULL)
					{
						printf("\nError: The adjacency matrix is missing some columns");
						//Free the memory allocated
						freeMemory(graph, noOfRecords);
						//Closing the file
						fclose(fp);

						return NULL;
					}

					graph[rowPos][colPos] = atoi(token);

					//Increment the column position
					colPos++;

					//Check if the adjacency matrix contains value greater the 500
					if(atoi(token) > 500)
					{
						printf("\nError: The adjacency matrix contains value greater then 500", (atoi(token)));
						//Free the memory allocated
						freeMemory(graph, noOfRecords);
						//Closing the file
						fclose(fp);

						return NULL;
					}
				}
				//Increment the row position
				rowPos++;
			}
		}

		printf("\nThe adjacency matrix is as follows,\n");
		display(graph);
	}
	else
	{
		printf("Error: Cannot open file !");
		graph = NULL;
	}

	//Closing the file pointer
	if(fp != NULL)
		fclose(fp);

	return graph;
}

//Function for calculating the shortest path between all the vertices
void calculateShortestPath(int **graph)
{
	int **finalGraph;
	int  i, j, k;

	//Allocating dynamic memory
	finalGraph = malloc(noOfRecords * sizeof(int *));

	//Allocate the columns
	for(int i = 0; i < noOfRecords; i++)
		finalGraph[i] = malloc(noOfRecords * sizeof(int));

	for(i = 0; i < noOfRecords; i++)
	{
		for(j = 0; j < noOfRecords; j++)
		{
			finalGraph[i][j] = graph[i][j];
		}
	}

	for (k = 0; k < noOfRecords; k++)
	{
		for (i = 0; i < noOfRecords; i++)
		{
			for (j = 0; j < noOfRecords; j++)
			{
				if((finalGraph[i][k] + finalGraph[k][j]) < finalGraph[i][j])
					finalGraph[i][j] = finalGraph[i][k] + finalGraph[k][j];
			}
		}
	}

	//Displaying the shortest paths
	printf("\nThe shortest path between every pair of vertices are as follows\n");
	display(finalGraph);
	//Freeing the memory allocated
	freeMemory(finalGraph, noOfRecords);
}

//Function for displaying the shortest path
void display(int **graph)
{
	for(int i = 0; i < noOfRecords; i++)
	{
		for(int j = 0; j < noOfRecords; j++)
		{
			//For pretty printing
			if(graph[i][j] <= 9)
				printf("%d  \t", graph[i][j]);
			else if(graph[i][j] >=10 && graph[i][j] <= 99)
				printf("%d \t", graph[i][j]);
			else
				printf("%d\t", graph[i][j]);
		}
		printf("\n");
	}
}

//Function for freeing the allocated memory
void freeMemory(int **graph, int records)
{
	//Freeing the columns
	for(int i = 0; i < records; i++)
		free(graph[i]);

	//Freeing the graph
	free(graph);
}
