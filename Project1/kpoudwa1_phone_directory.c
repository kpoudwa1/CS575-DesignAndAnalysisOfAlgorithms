#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct PhoneBook
{
	char *firstName;
	char *lastName;
	char *number;
} PhoneBook;

//Global pointers
PhoneBook *book;
PhoneBook *pivotElement;
int arrayLength = 0;

//Function declarations
void create(char *filePath);
void mergeSort(PhoneBook *book, int first, int last);
void merge(PhoneBook *book, int first, int middle, int last);
void freeMemory(PhoneBook *book);
void deleteEntry(char *name);
void insertEntry(char *name, char *phone_number);
void searchDirectory(char *name);
void sortDirectory();
int partition(int leftIndex, int rightIndex, PhoneBook *book);
void quicksort(PhoneBook *book, int leftIndex, int rightIndex);
void displayDirectory();

int main(int argc, char *argv[])
{
	printf( "\n************************************************************");
	printf( "\nFile Processing started");
	printf( "\n************************************************************");

	int choice;
	char fname[50];
	char lname[50];
	char phoneNumInput[20];

	//Reading the file
	printf( "\nProcessing %s file", argv[1]);
	create(argv[1]);
	//Sorting the records
	sortDirectory();
	displayDirectory();

	do
	{
		printf( "\n\t\tMAIN MENU");
		printf( "\nInsert			(press 1)");
		printf( "\nSearch			(press 2)");
		printf( "\nDelete			(press 3)");
		printf( "\nShow Directory		(press 4)");
		printf( "\nExit			(press 5)");
		
		printf( "\nPlease choose an operation ");
		scanf("%d", &choice);

		switch(choice)
		{
			case 1:
			{
				printf( "\nEnter the first name ");
				scanf("%s" , &fname);

				printf( "\nEnter the last name ");
				scanf("%s", &lname);

				printf( "\nEnter Phone Number ");
				scanf("%s" , &phoneNumInput);

				strcat(fname, " ");
				strcat(fname, lname);
				insertEntry(fname, phoneNumInput);
				//Function for sorting directory after insert
				sortDirectory();
				break;
			}
			case 2:
			{
				printf( "\nEnter the first name ");
				scanf("%s" , &fname);
				
				printf( "\nEnter the last name ");
				scanf("%s", &lname);
				
				
				strcat(lname, " ");
				strcat(lname, fname);
				//Sorting the phone directory and then searching the required record
				sortDirectory();
				searchDirectory(lname);
				
				break;
			}
			case 3: 
			{
				printf( "\nEnter the first name ");
				scanf("%s" , &fname);
				
				printf( "\nEnter the last name ");
				scanf("%s", &lname);

				strcat(fname, " ");
				strcat(fname, lname);
				deleteEntry(fname);
				break;
			}
			case 4:
			{
				displayDirectory();
				break;
			}
			case 5:
			{
				freeMemory(book);
				exit(0);
			}
			default:
				printf( "\nInvalid choice\n");
		}
	}while(choice >= 1 && choice <= 5);
    
	return 0;
}

//Function for reading a file and processing all the records in the file
void create(char *filePath)
{
        char str[100];
		int recordCount = 0;
        
		//File pointer
        FILE *fp;
        fp = fopen(filePath, "r");
        
		if(fp != NULL)
        {
				//Getting the number of records in the file
                while(fgets(str, 100, fp) != NULL)
                {
					recordCount++;
                }
				
			//Printing the record count & closing the file pointer
			printf( "\nNumber of records: %d", recordCount);
			//Closing the file
			fclose(fp);
	
			//Allocating memory dynamically and setting the array length
			book = malloc(sizeof(PhoneBook) * recordCount);
			arrayLength = recordCount;
	
			//Reopening the file for processing the records
			fp = fopen(filePath, "r");
			if(fp != NULL)
			{
				int count = 0;
				char *token;
				char *token2;
				char *no;
		
				while(fgets(str, 100, fp) != NULL)
				{
					token = strtok(str, ",");
					no = strtok(NULL, ",");
					token2 = strtok(token, " ");
					token2 = strtok(NULL, " ");

					//Allocate memory for the first name and copy the first name
					book[count].firstName = malloc(strlen(token2) + 1);
					strcpy(book[count].firstName, token2);

					//Allocate memory for the last time and copy the last name
					token2 = strtok(NULL, " ");
					book[count].lastName = malloc(strlen(token2) + 1);
					strcpy(book[count].lastName, token2);

					//Allocate memory for the phone number and copy phone number
					book[count].number = malloc(strlen(no));
					no[strlen(no) - 1] = '\0';
					strcpy(book[count].number, no);
					
					count++;
				}
			}
        }
        else
        {
                printf("\nCannot open file !");
        }

        //Closing the file pointer
        fclose(fp);
}

//Function for implementing the merge sort algorithm
void mergeSort(PhoneBook *book, int first, int last)
{
	if(first < last)
	{
		int middle = (first + (last - 1)) / 2;

		//Start sorting
		mergeSort(book, first, middle);
		mergeSort(book, middle + 1, last);

		//Merge
		merge(book, first, middle, last);
	}
}

void merge(PhoneBook *book, int left, int middle, int right)
{
	int leftSize = middle - left + 1;
	int rightSize =  right - middle;

	PhoneBook *RightSubArray = malloc(sizeof(PhoneBook) * rightSize);
	PhoneBook *LeftSubArray = malloc(sizeof(PhoneBook) * leftSize);

	for (int i = 0; i < leftSize; i++)
		LeftSubArray[i] = book[left + i];

	for (int j = 0; j < rightSize; j++)
 	RightSubArray[j] = book[middle + 1+ j];

	int subLeftIndex = 0; // Initial index of first subarray
	int subRightIndex = 0; // Initial index of second subarray
	int mergedIndex = left; // Initial index of merged subarray

	while(subLeftIndex < leftSize && subRightIndex < rightSize)
	{
		char name1[100];
		strcpy(name1, LeftSubArray[subLeftIndex].lastName);
		strcat(name1, " ");
		strcat(name1, LeftSubArray[subLeftIndex].firstName);

		char name2[100];
		strcpy(name2, RightSubArray[subRightIndex].lastName);
		strcat(name2, " ");
		strcat(name2, RightSubArray[subRightIndex].firstName);

		if (strcmp(name1, name2) < 0)
		{
			book[mergedIndex] = LeftSubArray[subLeftIndex];
			subLeftIndex++;
		}
		else
		{
			book[mergedIndex] = RightSubArray[subRightIndex];
			subRightIndex++;
		}
		mergedIndex++;
	}
	while (subLeftIndex < leftSize)
	{
		book[mergedIndex] = LeftSubArray[subLeftIndex];
		subLeftIndex++;
		mergedIndex++;
	}
	while (subRightIndex < rightSize)
	{
		book[mergedIndex] = RightSubArray[subRightIndex];
		subRightIndex++;
		mergedIndex++;
	}
}

//Function for deleting an entry from the phone record book
void deleteEntry(char *name)
{
	char *fname = strtok(name, " ");
	char *lname = strtok(NULL, " ");
	int index = -1;

	//Looping for getting the index of person who appears last in the sorted order
	for(int i = 0; i <= (arrayLength - 1); i++)
	{
		if((strcmp(fname, book[i].firstName) == 0) && (strcmp(lname, book[i].lastName) == 0))
		{
			index = i;
		}
	}

	//Checking if the record exists or not
	if(index != -1)
	{
		//Array copy
		printf( "\nMatch found for the record");
		PhoneBook *temp = malloc(sizeof(PhoneBook) * (arrayLength - 1));

		//Copying the contents to the new array
		for(int i = 0; i < arrayLength; i++)
		{
			if(i < index)
			{
				//Copy data in parallel
				temp[i].firstName = malloc(strlen(book[i].firstName));
				strcpy(temp[i].firstName, book[i].firstName);

				temp[i].lastName = malloc(strlen(book[i].lastName));
				strcpy(temp[i].lastName, book[i].lastName);
				
				temp[i].number = malloc(strlen(book[i].number));
				strcpy(temp[i].number, book[i].number);
			}
			else if(i > index)
			{
                //Copy data at next index
                temp[i - 1].firstName = malloc(strlen(book[i].firstName));
				strcpy(temp[i - 1].firstName, book[i].firstName);

                temp[i - 1].lastName = malloc(strlen(book[i].lastName));
                strcpy(temp[i - 1].lastName, book[i].lastName);

                temp[i - 1].number = malloc(strlen(book[i].number));
                strcpy(temp[i - 1].number, book[i].number);
			}
		}
		
		//Free memory allocated to old array
		freeMemory(book);
		book = temp;

		//Decrease the array length
		arrayLength--;
		
		printf( "\nThe record has been successfully deleted from the phone book");
	}
	else
	{
		printf( "\nUnable to delete as entry not found");
	}
}

//Function for inserting an entry in the phone directory
void insertEntry(char *name, char *phone_number)
{
	//1 = record exists, 0 = record does not exists
	int insertRecord = 1;

	char *fname = strtok(name, " ");
	char *lname = strtok(NULL, " ");

	//Looping through the records to check if entry already exists
	for(int i = 0; i <= (arrayLength - 1); i++)
	{
		if((strcmp(fname, book[i].firstName) == 0) && (strcmp(lname, book[i].lastName) == 0) && (strcmp(phone_number, book[i].number) == 0))
		{
			printf( "\nEntry already exists in the directory");
			insertRecord = 0;
			break;
		}
	}

	if(insertRecord == 1)
	{
		PhoneBook *temp = malloc(sizeof(PhoneBook) * (arrayLength + 1));

		//Incrementing the size of array by 1
		book = realloc(book, sizeof(PhoneBook) * (arrayLength + 1));
		
		//Copying the contents to the last location in the new array
		book[arrayLength].firstName = malloc(strlen(fname));
		strcpy(book[arrayLength].firstName, fname);

		book[arrayLength].lastName = malloc(strlen(lname));
		strcpy(book[arrayLength].lastName, lname);

		book[arrayLength].number = malloc(strlen(phone_number));
		strcpy(book[arrayLength].number, phone_number);

		//Incrementing the length of the array
		arrayLength++;
	
		printf( "\nRecord added successfully to the directory");
		}
}

//Function for searching an entry in the phone directory
void searchDirectory(char *name)
{
	//Sort the directory and then searching the required record
	int leftIndex = 0;
	int rightIndex = arrayLength - 1;
	int middleIndex;
	int recordFound = 0;

	while(leftIndex <= rightIndex)
	{
		middleIndex = (leftIndex+rightIndex) / 2;
		char tempName[100];
		strcpy(tempName, book[middleIndex].lastName);
		strcat(tempName, " ");
		strcat(tempName, book[middleIndex].firstName);
		
		if(strcmp(name, tempName) == 0)
		{
			int startIndex = middleIndex;
			int endIndex = middleIndex;
			
			//Iterate to the left side for getting the start location incase of multiple records with the same name
			for(int i = middleIndex - 1; i >= 0; i--)
			{
				char matchName[100];
				strcpy(matchName, book[i].lastName);
				strcat(matchName, " ");
				strcat(matchName, book[i].firstName);
				if(strcmp(name, matchName) == 0)
				{
					startIndex = i;
				}
				else
				{
					break;
				}
			}

			//Iterate to the right side for getting the start location incase of multiple records with the same name
            for(int i = middleIndex + 1; i < arrayLength; i++)
			{
				char matchName2[100];
				strcpy(matchName2, book[i].lastName);
				strcat(matchName2, " ");
                strcat(matchName2, book[i].firstName);
				
				if(strcmp(name, matchName2) == 0)
                {
					endIndex = i;
                }
                else
                {
					break;
				}
			}

			//Printing all the names from the start index
			printf( "\nList of matched records :");
			printf( "\nSrno\tName\t\t\t\tPhone_No");
			printf( "\n=============================================================================");
			for(int i = startIndex; i <= endIndex; i++)
			{
				printf( "\n%d\t%s %s\t\t\t%s", i+1, book[i].firstName, book[i].lastName, book[i].number);
			}
			printf( "\n=============================================================================");

			//Set the record found flag to true
			recordFound = 1;
			break;

		}
		else if(strcmp(name, tempName) < 0)
		{
			rightIndex = middleIndex - 1;
		}
		else if(strcmp(name, tempName) > 0)
		{
			leftIndex =  middleIndex +1;
		}
	}
	if(recordFound == 0)
		printf( "\nName not found");
}

//Function for sorting the phone directory
void sortDirectory()
{
	//Merge sort for sort names on the basis of names
	mergeSort(book, 0, arrayLength-1);

	//For sorting records with same names on the basis of phones numbers
	int end = -1;
	for(int i = 0; i <= arrayLength-1; )
	{
		char matchName[100];
		strcpy(matchName, book[i].lastName);
		strcat(matchName, " ");
		strcat(matchName, book[i].firstName);
		for(int j = i + 1; j <= arrayLength-1; j++)
		{
			char matchName2[100];
			strcpy(matchName2, book[j].lastName);
			strcat(matchName2, " ");
			strcat(matchName2, book[j].firstName);

			if(strcmp(matchName, matchName2) == 0)
				end = j;
			else
				break;
		}
		//Execute quick sort if there are records with same names
		if(end != -1)
		{
			quicksort(book, i, end);
			i = end + 1;
			end = -1;
		}
		else
		{
			i++;
		}
	}
}

int partition(int leftIndex, int rightIndex, PhoneBook *book)
{
	PhoneBook *temp = malloc(sizeof(PhoneBook));
	int pivot = rand() % (rightIndex - leftIndex + 1);
	pivot = pivot + leftIndex;

	//Swaping pivot and leftindex
	temp->firstName = malloc(strlen(book[leftIndex].firstName));
	strcpy(temp->firstName, book[leftIndex].firstName);
	temp->lastName = malloc(strlen(book[leftIndex].lastName));
	strcpy(temp->lastName, book[leftIndex].lastName);
	temp->number = malloc(strlen(book[leftIndex].number));
	strcpy(temp->number, book[leftIndex].number);

	strcpy(book[leftIndex].firstName, book[pivot].firstName);
	strcpy(book[leftIndex].lastName, book[pivot].lastName);
	strcpy(book[leftIndex].number, book[pivot].number);
	
	strcpy(book[pivot].firstName, temp->firstName);
	strcpy(book[pivot].lastName, temp->lastName);
	strcpy(book[pivot].number, temp->number);

	free(temp->firstName);
	free(temp->lastName);
	free(temp->number);

	int partitionIndex = leftIndex + 1;
		
	for(int j = leftIndex + 1; j <= rightIndex; j++)
	{
		if(strcmp(book[j].number, book[leftIndex].number) < 0)	
		{

			temp->firstName = malloc(strlen(book[partitionIndex].firstName));
			strcpy(temp->firstName, book[partitionIndex].firstName);

			temp->lastName = malloc(strlen(book[partitionIndex].lastName));
			strcpy(temp->lastName, book[partitionIndex].lastName);
			temp->number = malloc(strlen(book[partitionIndex].number));
			strcpy(temp->number, book[partitionIndex].number);

			strcpy(book[partitionIndex].firstName, book[j].firstName);
			strcpy(book[partitionIndex].lastName, book[j].lastName);
			strcpy(book[partitionIndex].number, book[j].number);

			strcpy(book[j].firstName, temp->firstName);
			strcpy(book[j].lastName, temp->lastName);
			strcpy(book[j].number, temp->number);

			free(temp->firstName);
			free(temp->lastName);
			free(temp->number);

			partitionIndex = partitionIndex + 1;
		}
	}

	temp->firstName = malloc(strlen(book[leftIndex].firstName));
	strcpy(temp->firstName, book[leftIndex].firstName);
			
	temp->lastName = malloc(strlen(book[leftIndex].lastName));
	strcpy(temp->lastName, book[leftIndex].lastName);
			
	temp->number = malloc(strlen(book[leftIndex].number));
	strcpy(temp->number, book[leftIndex].number);

	strcpy(book[leftIndex].firstName, book[partitionIndex-1].firstName);
	strcpy(book[leftIndex].lastName, book[partitionIndex-1].lastName);
	strcpy(book[leftIndex].number, book[partitionIndex-1].number);

	strcpy(book[partitionIndex-1].firstName, temp->firstName);
	strcpy(book[partitionIndex-1].lastName, temp->lastName);
	strcpy(book[partitionIndex-1].number, temp->number);

	free(temp->firstName);
	free(temp->lastName);
	free(temp->number);

	return partitionIndex-1;
}

//Function for implementing the quicksort algorithm
void quicksort(PhoneBook *book,int leftIndex,int rightIndex)
{
	if(leftIndex<rightIndex)
	{
		int partitionIndex1 = partition(leftIndex, rightIndex, book);
		quicksort(book, leftIndex, partitionIndex1 - 1);
		quicksort(book, partitionIndex1 + 1, rightIndex);
	}
}

//Function for freeing the dynamically allocated memory
void freeMemory(PhoneBook *book)
{
	for(int i = 0; i < arrayLength; i++)
	{
		free(book[i].firstName);
		free(book[i].lastName);
		free(book[i].number);
	}
	
	if(arrayLength > 0)
		free(book);
}

//Function for displaying the contents of the phone directory
void displayDirectory()
{
	sortDirectory();
	printf( "\nContents of the phone directory are:");
	printf( "\nSrno\tName\t\t\t\tPhone_No");
	printf( "\n=============================================================================");
	for(int i =0; i < arrayLength; i++)
	{
		printf( "\n%d\t%s %s\t\t\t%s", i+1, book[i].firstName, book[i].lastName, book[i].number);
	}
	printf( "\n=============================================================================");
}