#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Struct for Huffman tree
typedef struct Node
{
	char data;
	int frequency;
	struct Node *left, *right;
}Node;

//Struct for printing Huffman tree
typedef struct PrintNode
{
	char data;
	int frequency;
	int arraySize;
	int *code;
}PrintNode;

int noOfRecords = 0;
int currentSize = 0;
PrintNode *printArray;

//Function declarations
Node ** createArray(char *filePath);
Node * newNode(char c, int frequency);
void swap(Node **heap, int pos, int j);
Node * extractMin(Node **heap);
void insertParent(Node ** heap, Node *node);
void postorderPrint(Node *currentNode, int index, int tempArray[]);
void createHuffmanTree(Node ** heap);
void display();
void freeMemory(Node **heap);
void postorderFree(Node *node);
void bubbleSort(Node **heap);

int main(int argc, char *argv[])
{
	//Check if the file is passed as an argument
	if(argc != 2)
	{
		printf("Invalid number of arguments\n");
		exit(1);
	}

	//Create an array using the file
	Node ** heap = createArray(argv[1]);

	//Sorting
	bubbleSort(heap);

	//Building the Huffman tree
	createHuffmanTree(heap);

	//Code for printing
	int tempArray[20];
	int index = 0;
	postorderPrint(heap[0], index, tempArray);

	//Display the Huffman codes
	display();

	//Freeing the memory allocated for heap
	freeMemory(heap);

	printf("\n");
	return 0;
}

//Function for creating a dynamic array from the list of characters
Node ** createArray(char *filePath)
{
	char str[100];
	char line1[100];
	char line2[100];
	int recordCount = 1;
	Node **heap = NULL;

	//File pointer
	FILE *fp;
	fp = fopen(filePath, "r");

	//Check if the file is open
	if(fp != NULL)
	{
		//Get the first line
		fgets(str, 100, fp);
		fgets(line2, 100, fp);

		//Copy the contents of the first line
		strcpy(line1, str);

		char *token;
		//Read the element at 0 index
		token = strtok(str, ",");

		//Getting the number of characters
		while((token = strtok(NULL, ",")) != NULL)
			recordCount++;

		//Assign to global variables
		noOfRecords = recordCount;
		currentSize = recordCount;

		printf("\nNo of records %d", recordCount);

		//Allocate the arrays on heap
		heap = malloc(recordCount * sizeof(Node *));
		printArray = malloc(recordCount * sizeof(PrintNode));

		//For processing the characters
		for(int i = 0; i < recordCount; i++)
		{
			if(i == 0)
			{
				token = strtok(line1, ",");
				heap[i] = newNode(*token, 0);
			}
			else
			{
				token = strtok(NULL, ",");

				//Handling '\n'
				if(token[1] == '\n')
					token[1] = '\0';

				heap[i] = newNode(*token, 0);
			}

			//Update the print array
			printArray[i].data = *token;
		}

		//For processing the frequencies
		for(int i = 0; i < recordCount; i++)
		{
			if(i == 0)
			{
				token = strtok(line2, ",");
				heap[i]->frequency = atoi(token);
			}
			else
			{
				token = strtok(NULL, ",");
				heap[i]->frequency = atoi(token);
			}

			//Update the print array
			printArray[i].frequency = atoi(token);
		}
	}
	else
	{
		printf("Error: Cannot open file !");
		exit(1);
	}

	//Closing the file pointer
	if(fp != NULL)
		fclose(fp);

	return heap;
}

//Function for dynamically creating a new node on heap
Node * newNode(char c, int frequency)
{
	//Allocate the node on the heap
	Node *tempNode = malloc(sizeof(Node));

	//Assign the left and right pointers to NULL
	tempNode->left = NULL;
	tempNode->right = NULL;

	//Set the data and frequency
	tempNode->data = c;
	tempNode->frequency = frequency;

	return tempNode;
}

//Function for swapping
void swap(Node **heap, int pos, int j)
{
	Node *temp = heap[pos];
	heap[pos] = heap[j];
	heap[j] = temp;
}

//Function for extracting the min value from heap
Node * extractMin(Node **heap)
{
	//Assign the priority value
	Node *temp = heap[0];

	for(int i = 1; i < currentSize; i++)
	{
		heap[i - 1] = heap[i];
	}
	heap[currentSize - 1] = NULL;

	//Decrement the current size of the heap
	currentSize--;

	return temp;
}

//For inserting parent node in the min heap
void insertParent(Node ** heap, Node *node)
{
	heap[currentSize] = node;
		currentSize++;

	bubbleSort(heap);
}

//Print nodes
void postorderPrint(Node *currentNode, int index, int tempArray[])
{
	//Iterate the left sub tree
	if(currentNode->left != NULL)
	{
		tempArray[index] = 0;
		postorderPrint(currentNode->left, index + 1, tempArray);
	}

	//Iterate the right sub tree
	if(currentNode->right != NULL)
	{
		tempArray[index] = 1;
		postorderPrint(currentNode->right, index + 1, tempArray);
	}

	//Print the current node
	if(currentNode->left == NULL && currentNode->right == NULL)
	{
		for(int i = 0; i < noOfRecords; i++)
		{
			if(printArray[i].data == currentNode->data)
			{
				//Allocate the print array
				printArray[i].code = malloc(sizeof(int) * (index + 1));
				printArray[i].arraySize = index;

				//Copy the huffman code
				for(int j = 0; j <= index; j++)
				{
					printArray[i].code[j] = tempArray[j];
				}
			}
		}
	}
}

//Function for building Huffman Tree
void createHuffmanTree(Node ** heap)
{
	//Temp char for new nodes created
	char temp = '-';

	for(int i = 1; i < noOfRecords; i++)
	{
		//Extracting 2 nodes
		Node *xNode = extractMin(heap);
		Node *yNode = extractMin(heap);

		//Creating a parent node
		Node *parent = newNode(temp, xNode->frequency + yNode->frequency);
		parent->left = xNode;
		parent->right = yNode;

		//Inserting the parent node in min heap
		insertParent(heap, parent);
	}
}

//Function for display
void display()
{
	printf("\n\nChar\tFrequency\tHuffman Code");
	printf("\n======================================");
	for(int i = 0; i < noOfRecords; i++)
	{
		printf("\n%c\t%d\t\t", printArray[i].data, printArray[i].frequency);
		for(int j = 0; j < printArray[i].arraySize; j++)
			printf("%d", printArray[i].code[j]);
	}

	//Freeing the memory allocated for codes
	for(int i = 0; i < noOfRecords; i++)
	{
		free(printArray[i].code);
	}

	//Freeing the memory allocated for the array
	free(printArray);
}

//Function for freeing the memory for Huffman tree
void freeMemory(Node **heap)
{
	//Recursive function for freeing the nodes
	postorderFree(heap[0]);

	//Freeing the heap
	free(heap);
}

void postorderFree(Node *node)
{
	if(node != NULL)
	{
		//Traversing the left sub tree
		postorderFree(node->left);

		//Traversing the right sub tree
		postorderFree(node->right);

		//Free the node
		free(node);
	}
}

//Function for sorting the priority queue
void bubbleSort(Node **heap)
{
	for(int i = 0; i < currentSize; i++)
	{
		for(int j = 1; j < currentSize; j++)
		{
			if(heap[j - 1]->frequency > heap[j]->frequency)
				swap(heap, j - 1, j);
		}
	}
}
