#include<stdio.h>
#include<math.h>
#include<stdlib.h>

//Tromino tile board
int **board;
int counter = 0;

//Function declarations
int ** generateBoard(int holeRow, int holeColumn, int rows);
void trominoTile(int *counter, int k, int row, int column, int holeRow, int holeColumn);
void centerTromino(int *counter, int r1, int c1, int r2, int c2, int r3, int c3);
void printBoard(int boardOrder);
void freeMemory(int boardOrder);

int main(int argc, char *argv[])
{	
	//Variable declarations
	int k, rowSize, holeRow, holeColumn;
	
	//Check if the number of arguments passed is valid
	if(argc != 4)
	{
		printf("Invalid number of arguments\n");
		exit(1);
	}
	
	//Getting k
	k = atoi(argv[1]);
	//Getting the order of the matrix
	rowSize = (int) pow(2, k);
	//Getting the row location
	holeRow = atoi(argv[2]);
	holeColumn = atoi(argv[3]);	
	
	//Check if k >= 1
	if(k < 1)
	{
		printf("k must be greater than or atleast 1\n");
		exit(1);
	}
	if(holeRow >= rowSize || holeColumn >= rowSize)
	{
		printf("Please enter the proper hole location\n");
		exit(1);
	}

	//Creating a tromino board
	board = generateBoard(holeRow, holeColumn, rowSize);
	printf("A %d x %d board has been created!", rowSize, rowSize);
	printf("\nHole location, Row index: %d, Column index: %d", holeRow, holeColumn);
	
	//Executing the tromino tile function
	trominoTile(&counter, k, 0, 0, holeRow, holeColumn);

	//Displaying the contents of the board
	printf("\nThe Tromino tiling is as follows");
	printBoard(rowSize);

	//Freeing the memory allocated to the board
	freeMemory(rowSize);

	printf("\n");
	return 0;
}

//Function for generating the board
int ** generateBoard(int holeRow, int holeColumn, int rows)
{
	//Create a double pointer for the rows
	int **matrix = malloc(rows * sizeof(int *));

	//Allocate the columns
	for(int i = 0; i < rows; i++)
	{
		matrix[i] = malloc(rows * sizeof(int));
		for(int j = 0; j < rows; j++)
			matrix[i][j] = -1;
	}

	//Setting the hole to -99
	matrix[holeRow][holeColumn] = -99;

	return matrix;
}

//Function for printing the matrix
void printBoard(int boardOrder)
{
	printf("\n");
	for(int i = 0; i < boardOrder; i++)
	{
		for(int j = 0; j < boardOrder; j++)
		{
			//Printing 'X' for the hole and pretty printing
			if(board[i][j] <= 9 && board[i][j] == -99)
				printf("X  \t");
			else if(board[i][j] <= 9)
				printf("%d  \t", board[i][j]);
			else if(board[i][j] >= 10 && board[i][j] <= 99)
				printf("%d \t", board[i][j]);
			else
				printf("%d\t", board[i][j]);
		}
		printf("\n");
	}
}

//Recursive function for tromino tiling
void trominoTile(int *counter, int k, int row, int column, int holeRow, int holeColumn)
{
	//Getting the matrix order
	int matrixOrder = (int) pow(2, k);

	//For solving trivial case
	if(k == 1)
	{
		(*counter)++;
		for (int i = row; i < (row + matrixOrder); i++)
		{
			for (int j = column; j< (column + matrixOrder); j++)
			{
				if(!(i == holeRow && j == holeColumn))
				{
					board[i][j] = *counter;
				}
			}
		}
		return;
	}

	//Flag for deciding the quadrant
	int quadFlag = 0;

	//If the hole is in the first board, fill the other boards
	if(holeRow < row + (matrixOrder/2) && holeColumn < column + (matrixOrder/2))
	{
		centerTromino(counter, row + (matrixOrder/2) - 1, column + (matrixOrder/2), row + (matrixOrder/2), column + (matrixOrder/2) - 1, row + (matrixOrder/2), column + (matrixOrder/2));
		quadFlag = 1;
	}//If the hole is in the second board, fill the other boards
	else if(holeRow < row + (matrixOrder/2) && holeColumn >= column + (matrixOrder/2))
	{
		centerTromino(counter, row + (matrixOrder/2) - 1, column + (matrixOrder/2) - 1, row + (matrixOrder/2), column + (matrixOrder/2) - 1, row + (matrixOrder/2), column + (matrixOrder/2));
		quadFlag = 2;
	}//If the hole is in the third board, fill the other boards
	else if(holeRow >= row + (matrixOrder/2) && holeColumn < column + (matrixOrder/2))
	{
		centerTromino(counter, row + (matrixOrder/2) - 1, column + (matrixOrder/2) - 1, row + (matrixOrder/2) - 1, column + (matrixOrder/2), row + (matrixOrder/2), column + (matrixOrder/2));
		quadFlag = 3;
	}//If the hole is in the fourth board, fill the other boards
	else if(holeRow >= row + (matrixOrder/2) && holeColumn >= column + (matrixOrder/2))
	{
		centerTromino(counter, row + (matrixOrder/2) - 1, column + (matrixOrder/2) - 1, row + (matrixOrder/2) - 1, column + (matrixOrder/2), row + (matrixOrder/2), column + (matrixOrder/2) - 1);
		quadFlag = 4;
	}

	//Variable declarations for other holes
	int rowLeftUp, columnLeftUp, rowRightUp, columnRightUp, rowLeftLow, columnLeftLow, rowRightLow, columnRightLow;
	if(quadFlag == 1)
	{
		//Init the hole positions
		rowLeftUp = holeRow;
		columnLeftUp = holeColumn;
		rowRightUp = row + (matrixOrder/2) - 1;
		columnRightUp = column + (matrixOrder/2);
		rowLeftLow = row + (matrixOrder/2);
		columnLeftLow = column + (matrixOrder/2) - 1;
		rowRightLow = row + (matrixOrder/2);
		columnRightLow = column + (matrixOrder/2);
	}
	else if(quadFlag == 2)
	{
		//Init the hole positions
		rowLeftUp = row + (matrixOrder/2) - 1;
		columnLeftUp = column + (matrixOrder/2) - 1;
		rowRightUp = holeRow;
		columnRightUp = holeColumn;
		rowLeftLow = row + (matrixOrder/2);
		columnLeftLow = column + (matrixOrder/2) - 1;
		rowRightLow = row + (matrixOrder/2);
		columnRightLow = column + (matrixOrder/2);
	}
	else if(quadFlag == 3)
	{
		//Init the hole positions
		rowLeftUp = row + (matrixOrder/2) - 1;
		columnLeftUp = column + (matrixOrder/2) - 1;
		rowRightUp = row + (matrixOrder/2) - 1;
		columnRightUp = column + (matrixOrder/2);
		rowLeftLow = holeRow;
		columnLeftLow = holeColumn;
		rowRightLow = row + (matrixOrder/2);
		columnRightLow = column + (matrixOrder/2);
	}
	else if(quadFlag == 4)
	{
		//Init the hole positions
		rowLeftUp = row + (matrixOrder/2) - 1;
		columnLeftUp = column + (matrixOrder/2) - 1;
		rowRightUp = row + (matrixOrder/2) - 1;
		columnRightUp = column + (matrixOrder/2);
		rowLeftLow = row + (matrixOrder/2);
		columnLeftLow = column + (matrixOrder/2) - 1;
		rowRightLow = holeRow;
		columnRightLow = holeColumn;
	}

	//Recursive calls
	trominoTile(counter, k - 1, row, column, rowLeftUp, columnLeftUp);
	trominoTile(counter, k - 1, row,column + (matrixOrder/2), rowRightUp, columnRightUp);
	trominoTile(counter, k - 1,row + (matrixOrder/2),column, rowLeftLow, columnLeftLow);
	trominoTile(counter, k - 1,row + (matrixOrder/2),column + (matrixOrder/2), rowRightLow, columnRightLow);
}

//Function for putting the tromino in the center
void centerTromino(int *counter, int r1, int c1, int r2, int c2, int r3, int c3)
{
	(*counter)++;
	board[r1][c1] = *counter;
	board[r2][c2] = *counter;
	board[r3][c3] = *counter;
}

//Function for freeing the allocated memory
void freeMemory(int boardOrder)
{
	//Freeing the columns
	for(int i = 0; i < boardOrder; i++)
		free(board[i]);

	//Freeing the board
	free(board);
}