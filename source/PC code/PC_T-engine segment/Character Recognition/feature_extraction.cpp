#include "feature_extraction.h"

void getFeatureExtraction(Image* input, double* output, int group)
{
	int RESAMPLE_SIZE_HEIGHT, RESAMPLE_SIZE_WIDTH;
	if (group == 1) 
	{
		RESAMPLE_SIZE_HEIGHT = 5;
		RESAMPLE_SIZE_WIDTH = 5;
	}
	else if (group == 2 || group == 3 || group == 4 || group == 5 || group == 8 || group == 9)
	{
		RESAMPLE_SIZE_HEIGHT = 10;
		RESAMPLE_SIZE_WIDTH = 5;
	}
	else
	{
		RESAMPLE_SIZE_HEIGHT = 7;
		RESAMPLE_SIZE_WIDTH = 5;
	}

	Image* tempImage = (Image*)malloc(sizeof(Image));
	createImage(tempImage, input->mHeight, input->mWidth);
	copyImage(input, tempImage);
	int maxOfLengthLine;
	int MDF_VEC_LEN = (RESAMPLE_SIZE_HEIGHT + RESAMPLE_SIZE_WIDTH) * MAX_OF_TRANSITION * 2 * 2;
	//output = (double*)malloc(MDF_VEC_LEN * sizeof(double));

	findingStartingPoint(input, &maxOfLengthLine);
	//printImage(input);
	normalizeImageDirection(input, tempImage, maxOfLengthLine);
	getMDFFeatureVector(input, output, RESAMPLE_SIZE_HEIGHT, RESAMPLE_SIZE_WIDTH);

}
void findingStartingPoint(Image* input, int* maxOfLengthLine)
{
	int height = input->mHeight;
	int width = input->mWidth;
	unsigned char* data = input->mData;

	int countingChangeDirection = 0;
	int lengthOfPreviousDirection = 0;
	unsigned char previousDirection = 0;
	unsigned char newDirection = 0;

	*maxOfLengthLine = 0;
	int tempOfMaxOfLengthLine = 1;

	Stack* stack = (Stack*)malloc(sizeof(Stack));
	createStack(stack, NORMAL_STACK_MAXSIZE);
	
	int i, j;
	for (i = height - 1; i >= 0; i--)
		for (j = 0; j < width; j++)
		{
			if (*(data + i*width + j) == 1)
			{
				pushStack(stack, i, j);
				while(stack->n > 0)			//duyet stack
				{
					reduceStack(stack, input);
					
					if (stack->n <= 0) break;
					int startRow = *(stack->row + stack->n - 1);
					int startCol = *(stack->col + stack->n  - 1);
					stack->n--;

					*(data + startRow*width + startCol) = 8;		//assign new Segment point

					previousDirection = 8;
					lengthOfPreviousDirection = 1;
					tempOfMaxOfLengthLine = 1;
					countingChangeDirection = 0;

					int curRow = startRow;
					int curCol = startCol;
					int nextRow, nextCol;
					while (findingNextPoint(stack, input, curRow, curCol, &nextRow, &nextCol) != 0)
					{
						newDirection = convertToDirection(curRow, curCol, nextRow, nextCol);
						//unsigned char convertToDirection(int curRow, int curCol, int nextRow, int nextCol)
						curRow = nextRow;
						curCol = nextCol;
						if (previousDirection == newDirection)
						{
							lengthOfPreviousDirection++;
							tempOfMaxOfLengthLine++;
							*(data + curRow*width + curCol) = newDirection;
						}
						else		//sudden change in direction
						{
							if (previousDirection != 8) countingChangeDirection++;
							if (checkCorner(input, newDirection, curRow, curCol) != 0				//check corner success
															|| countingChangeDirection >= 3		//change > 3
															|| lengthOfPreviousDirection >= 2)	//length of direction > 3   
							{
								*(data + curRow*width + curCol) = 8;
								previousDirection = 8;
								if (tempOfMaxOfLengthLine > *maxOfLengthLine) *maxOfLengthLine = tempOfMaxOfLengthLine;
								tempOfMaxOfLengthLine = 1;

								lengthOfPreviousDirection = 1;
								countingChangeDirection = 0;
								continue;
							}
							else											//nothing happen
							{
								*(data + curRow*width + curCol) = newDirection;
								previousDirection = newDirection;
								tempOfMaxOfLengthLine++;
								lengthOfPreviousDirection = 1;
								continue;
							}
						}
					}//when 1 part of skeleton end
					if (tempOfMaxOfLengthLine > *maxOfLengthLine) *maxOfLengthLine = tempOfMaxOfLengthLine;
					tempOfMaxOfLengthLine = 1;
				}//when there is no component in stack
			}//if data of pixel == 1
		}//end for loop
	//input->printImage();
	free(stack);
}
void reduceStack(Stack* stack, Image* input)
{
	unsigned char* data = input->mData;
	int width = input->mWidth;
	int i,j;
	for (i = 0; i < stack->n; i++)
	{
		int row = *(stack->row + i);
		int col = *(stack->col + i);
		if (*(data + row*width + col) != 1)
		{
			for (j = i; j < stack->n - 1; j++)
			{
				*(stack->row + j) = *(stack->row + j + 1);
				*(stack->col + j) = *(stack->col + j + 1);
			}
			i--;
			stack->n--;
		}
	}
}
int findingNextPoint(Stack* stack, Image* input, int curRow, int curCol, int *nextRow, int *nextCol)
{
	unsigned char* data = input->mData;

	int width = input->mWidth;
	int height = input->mHeight;
	int result = 0;
	


	unsigned char* ptr = data + curRow * width;		//duyet theo con tro tung hang
	if (curCol > 0 && *(ptr + curCol - 1) == 1)		//1 diem ngay ben trai
	{
		result = 1;
		*nextRow = curRow;
		*nextCol = curCol - 1;
	}
	if (curRow > 0)
	{
		ptr = data + (curRow - 1)*width;
		if (curCol > 0)		
		{
			if (*(ptr + curCol - 1)  == 1)		//goc tren trai
			{
				if (result != 0)
				{
					pushStack(stack, curRow - 1, curCol - 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow - 1;
					*nextCol = curCol - 1;
				}
			}
		}
		if (*(ptr + curCol) == 1)    //phia tren
		{
			if (result != 0)
			{
				pushStack(stack, curRow - 1, curCol);
			}
			else
			{
				result = 1;
				*nextRow = curRow - 1;
				*nextCol = curCol;
			}
		}
		if (curCol < width - 1)    //goc tren phai
		{
			if (*(ptr + curCol + 1) == 1)
			{
				if (result != 0)
				{
					pushStack(stack, curRow - 1, curCol + 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow - 1;
					*nextCol = curCol + 1;
				}
			}
		}
	}
	ptr = data + curRow * width;
	if (curCol < width - 1 && *(ptr + curCol + 1) == 1)  //phia ben phai
	{
		if (result != 0)
		{
			pushStack(stack, curRow, curCol + 1);
		}
		else
		{
			result = 1;
			*nextRow = curRow;
			*nextCol = curCol + 1;
		}
	}
	if (curRow < height - 1)
	{
		ptr = data + (curRow + 1)*width;
		if (curCol < width - 1)  //goc duoi phai
		{
			if (*(ptr + curCol + 1) == 1)
			{
				if (result != 0)
				{
					pushStack(stack, curRow + 1, curCol + 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow + 1;
					*nextCol = curCol + 1;
				}
			}
		}
		if (*(ptr + curCol) == 1)  //phia duoi
		{
			if (result != 0)
			{
				pushStack(stack, curRow + 1, curCol);
			}
			else
			{
				result = 1;
				*nextRow = curRow + 1;
				*nextCol = curCol;
			}
		}
		if (curCol > 0)  //goc duoi trai
		{
			if (*(ptr + curCol -1) == 1) 
			{
				if (result != 0)
				{
					pushStack(stack, curRow + 1, curCol - 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow + 1;
					*nextCol = curCol - 1;
				}
			}
		}
	}
	return result;
}

unsigned char convertToDirection(int curRow, int curCol, int nextRow, int nextCol)
{
	if (((curRow - nextRow == 1) && (curCol - nextCol == 1))||((curRow - nextRow == -1) && (curCol - nextCol == -1))) return 3;
	else if (((curRow - nextRow == 1) && (curCol - nextCol == -1))||((curRow - nextRow == -1) && (curCol - nextCol == 1))) return 5;
	else if (curCol - nextCol == 0) return 2;
	else if (curRow - nextRow == 0) return 4;
	return 0;
}

int findingNextPointOfCheckCorner(Image* input, int curRow, int curCol, int *nextRow, int *nextCol)
{
	unsigned char* data = input->mData;
	int width = input->mWidth;
	int height = input->mHeight;

	unsigned char* ptr = data + curRow * width;		//duyet theo con tro tung hang	
	if (curCol > 0 && *(ptr + curCol - 1) == 1)		//1 diem ngay ben trai
	{
		*nextRow = curRow;
		*nextCol = curCol - 1;
		return 1;
	}
	if (curRow > 0)
	{
		ptr = data + (curRow - 1)*width;
		if (curCol > 0)		
		{
			if (*(ptr + curCol - 1)  == 1)		//goc tren trai
			{
				*nextRow = curRow - 1;
				*nextCol = curCol - 1;
				return 1;
			}
		}
		if (*(ptr + curCol) == 1)    //phia tren
		{
			*nextRow = curRow - 1;
			*nextCol = curCol;
			return 1;
		}
		if (curCol < width - 1)    //goc tren phai
		{
			if (*(ptr + curCol + 1) == 1)
			{
				*nextRow = curRow - 1;
				*nextCol = curCol + 1;
				return 1;
			}
		}
	}
	ptr = data + curRow * width;
	if (curCol < width - 1 && *(ptr + curCol + 1) == 1)  //phia ben phai
	{
		*nextRow = curRow;
		*nextCol = curCol + 1;
		return 1;
	}
	if (curRow < height - 1)
	{
		ptr = data + (curRow + 1)*width;
		if (curCol < width - 1)  //goc duoi phai
		{
			if (*(ptr + curCol + 1) == 1)
			{
				*nextRow = curRow + 1;
				*nextCol = curCol + 1;
				return 1;
			}
		}
		if (*(ptr + curCol) == 1)  //phia duoi
		{
			*nextRow = curRow + 1;
			*nextCol = curCol;
			return 1;
		}
		if (curCol > 0)  //goc duoi trai
		{
			if (*(ptr + curCol -1) == 1) 
			{
				*nextRow = curRow + 1;
				*nextCol = curCol - 1;
				return 1;
			}
		}
	}
	return 0;
}

int checkCorner(Image* input, unsigned char previousDirection, int curRow, int curCol)
{
	int nextRow, nextCol;
	if (findingNextPointOfCheckCorner(input, curRow, curCol, &nextRow, &nextCol) != 0)
	{
		unsigned char newDirection = convertToDirection(curRow, curCol, nextRow, nextCol);
		if (previousDirection != 8)
		{
			if (previousDirection == 3 && newDirection == 5) return 1;
			else if (previousDirection == 5 && newDirection == 3) return 1;
			else return 0;
		}
		else return 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void normalizeImageDirection(Image* input, Image* tempImage, int maxOfLengthLine)
{
	Stack* list = (Stack*)malloc(sizeof(Stack));
	createStack(list, maxOfLengthLine);
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	createStack(stack, NORMAL_STACK_MAXSIZE);
	int width = input->mWidth;
	int height = input->mHeight;
	unsigned char* data = input->mData;
	int i, j;
	for (i = height-1; i >= 0; i--)
		for (j = 0; j < width; j++)
		{
			if (*(data + i*width + j) == 8)
			{
				pushStack(stack, i, j);
				while(stack->n > 0)			//duyet stack
				{
					reduceStackForNomarlization(input, tempImage, stack);
					if (stack->n <= 0) break;
					int startRow = *(stack->row + stack->n - 1);
					int startCol = *(stack->col + stack->n  - 1);
					stack->n--;
					*(tempImage->mData + startRow*width + startCol) = 2;

					//Reset List
					list->n = 0;
					pushStack(list, startRow, startCol);
					

					int curRow = startRow;
					int curCol = startCol;
					int nextRow, nextCol;
					unsigned char valueOfDirection;

					//use to define value to modify
					unsigned char valueToModify = 8;
					int countingDirection2 = 0;
					int countingDirection3 = 0;
					int countingDirection4 = 0;
					int countingDirection5 = 0;
					
					while ((valueOfDirection = findingNextPointForNormalization(input, tempImage, stack, curRow, curCol, &nextRow, &nextCol)) != 0)
					{
						
						
						if (valueOfDirection != 8)
						{
							pushStack(list, nextRow, nextCol);						
							if (valueOfDirection == 2) countingDirection2++;
							else if (valueOfDirection == 3) countingDirection3++;
							else if (valueOfDirection == 4) countingDirection4++;
							else countingDirection5++;
						}
						else
						{
							valueToModify = findingValueToModify(countingDirection2, countingDirection3, countingDirection4,
																								countingDirection5);
							modifyDirection(input, list, valueToModify);

							//Reset list
							list->n = 0;
							pushStack(list, nextRow, nextCol);
							countingDirection2 = 0;
							countingDirection3 = 0;
							countingDirection4 = 0;
							countingDirection5 = 0;
						}
						curRow = nextRow;
						curCol = nextCol;
					}
					valueToModify = findingValueToModify(countingDirection2, countingDirection3, countingDirection4,
																								countingDirection5);
					modifyDirection(input, list, valueToModify);
				}//when there is no component in stack
			}
		}
	free(list);
	free(stack);
}
int findingNextPointForNormalization(Image* input, Image* tempImage, Stack* stack, int curRow, int curCol, int* nextRow, int* nextCol)
{
	unsigned char* data = input->mData;
	unsigned char* tempData = tempImage->mData;

	int width = input->mWidth;
	int height = input->mHeight;
	int result = 0;
	


	unsigned char* ptr = data + curRow * width;		//duyet theo con tro tung hang
	unsigned char* tempPtr = tempData + curRow * width;	
	if (curCol > 0 && *(ptr + curCol - 1) != 0 && *(tempPtr + curCol - 1) == 1)		//1 diem ngay ben trai
	{
		result = *(ptr + curCol - 1);
		*nextRow = curRow;
		*nextCol = curCol - 1;
		*(tempPtr + curCol - 1) = 2;
	}
	if (curRow > 0)
	{
		ptr = data + (curRow - 1)*width;
		tempPtr = tempData + (curRow - 1)*width;
		if (curCol > 0)		
		{
			if (*(ptr + curCol - 1)  != 0 && *(tempPtr + curCol - 1)  == 1)		//goc tren trai
			{
				if (result != 0)
				{
					if (*(ptr + curCol - 1)  == 8) 
					{
						pushStack(stack, curRow - 1, curCol - 1);
						//*(tempPtr + curCol - 1) = 2;
					}
				}
				else
				{
					result = *(ptr + curCol - 1);
					*nextRow = curRow - 1;
					*nextCol = curCol - 1;
					*(tempPtr + curCol - 1) = 2;
				}				
			}
		}
		if (*(ptr + curCol) != 0 && *(tempPtr + curCol) == 1)    //phia tren
		{
			if (result != 0)
			{
				if (*(ptr + curCol) == 8)
				{
					pushStack(stack, curRow - 1, curCol);
				}
			}
			else
			{
				result = *(ptr + curCol);
				*nextRow = curRow - 1;
				*nextCol = curCol;
				*(tempPtr + curCol) = 2;
			}
		}
		if (curCol < width - 1)    //goc tren phai
		{
			if (*(ptr + curCol + 1) != 0 && *(tempPtr + curCol + 1) == 1)
			{
				if (result != 0)
				{
					if (*(ptr + curCol + 1) == 8)
					{
						pushStack(stack, curRow - 1, curCol + 1);
						//*(tempPtr + curCol + 1) = 2;
					}
				}
				else
				{
					result = *(ptr + curCol + 1);
					*nextRow = curRow - 1;
					*nextCol = curCol + 1;
					*(tempPtr + curCol + 1) = 2;		
				}
			}
		}
	}
	ptr = data + curRow * width;
	tempPtr = tempData + curRow * width;
	if (curCol < width - 1 && *(ptr + curCol + 1) != 0 && *(tempPtr + curCol + 1) == 1)  //phia ben phai
	{
		if (result != 0)
		{
			if (*(ptr + curCol + 1) == 8)
			{
				pushStack(stack, curRow, curCol + 1);
				//*(tempPtr + curCol + 1) = 2;
			}
		}
		else
		{
			result = *(ptr + curCol + 1);
			*nextRow = curRow;
			*nextCol = curCol + 1;
			*(tempPtr + curCol + 1) = 2;
		}
		//*(tempPtr + curCol + 1) = 2;
	}
	if (curRow < height - 1)
	{
		ptr = data + (curRow + 1)*width;
		tempPtr = tempData + (curRow + 1)*width;
		if (curCol < width - 1)  //goc duoi phai
		{
			if (*(ptr + curCol + 1) != 0 && *(tempPtr + curCol + 1) == 1)
			{
				if (result != 0)
				{
					if (*(ptr + curCol + 1) == 8)
					{
						pushStack(stack, curRow + 1, curCol + 1);
						//*(tempPtr + curCol + 1) = 2;
					}
				}
				else
				{
					result = *(ptr + curCol + 1);
					*nextRow = curRow + 1;
					*nextCol = curCol + 1;
					*(tempPtr + curCol + 1) = 2;
				}
				//*(tempPtr + curCol + 1) = 2;
			}
		}
		if (*(ptr + curCol) != 0 && *(tempPtr + curCol) == 1)  //phia duoi
		{
			if (result != 0)
			{
				if (*(ptr + curCol) == 8)
				{
					pushStack(stack, curRow + 1, curCol);
					//*(tempPtr + curCol) = 2;
				}
			}
			else
			{
				result = *(ptr + curCol);
				*nextRow = curRow + 1;
				*nextCol = curCol;
				*(tempPtr + curCol) = 2;
			}
			//*(tempPtr + curCol) = 2;
		}
		if (curCol > 0)  //goc duoi trai
		{
			if (*(ptr + curCol -1) != 0 && *(tempPtr + curCol - 1) == 1) 
			{
				if (result != 0)
				{
					if (*(ptr + curCol -1) == 8)
					{
						pushStack(stack, curRow + 1, curCol - 1);
						//*(tempPtr + curCol - 1) = 2;
					}
				}
				else
				{
					result = *(ptr + curCol -1);
					*nextRow = curRow + 1;
					*nextCol = curCol - 1;
					*(tempPtr + curCol - 1) = 2;
				}
				//*(tempPtr + curCol - 1) = 2;
			}
		}
	}
	return result;
}
void modifyDirection(Image* input, Stack *list, unsigned char value)
{
	int i;
	for (i = 0; i < list->n; i++)
	{
		*(input->mData + *(list->row + i)*input->mWidth + *(list->col + i)) = value;
	}
}
unsigned char findingValueToModify(int countingDirection2, int countingDirection3, int countingDirection4, int countingDirection5)
{
	if (countingDirection2 == 0 && countingDirection3 == 0 && countingDirection4 == 0 && countingDirection5 == 0) return 8;
	if (countingDirection2 >= countingDirection3 && countingDirection2 >= countingDirection4
												 && countingDirection2 >= countingDirection5) return 2;
	else if (countingDirection3 >= countingDirection2 && countingDirection3 >= countingDirection4
												 && countingDirection3 >= countingDirection5) return 3;
	else if (countingDirection4 >= countingDirection2 && countingDirection4 >= countingDirection3
												 && countingDirection4 >= countingDirection5) return 4;
	else if (countingDirection5 >= countingDirection2 && countingDirection5 >= countingDirection3
												 && countingDirection5 >= countingDirection4) return 5;
	return 0;
}


void reduceStackForNomarlization(Image* input, Image* tempImage, Stack* stack)
{
	unsigned char* data = input->mData;
	int width = input->mWidth;
	int i,j;
	for (i = 0; i < stack->n; i++)
	{
		int row = *(stack->row + i);
		int col = *(stack->col + i);
		if (*(data + row*width + col) != 8 || *(tempImage->mData + row*width + col) == 2)
		{
			for (j = i; j < stack->n - 1; j++)
			{
				*(stack->row + j) = *(stack->row + j + 1);
				*(stack->col + j) = *(stack->col + j + 1);
			}
			i--;
			stack->n--;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void getMDFFeatureVector(Image* input, double* output, int RESAMPLE_SIZE_HEIGHT, int RESAMPLE_SIZE_WIDTH)
{
	int width = input->mWidth;
	int height = input->mHeight;
	unsigned char* data = input->mData;

	//left to right and right to left : size of vector : height*3
	//top to bottom and bottom to top : size of vector : width*3
	int sizeOfVector = (height + width)*MAX_OF_TRANSITION*2;	//2 for the reverse direction 	
	int* LT = (int*)malloc(sizeOfVector*sizeof(int));
	int* DT = (int*)malloc(sizeOfVector*sizeof(int));

	//initial LT and DT
	int i;
	for (i = 0; i < sizeOfVector; i++)
	{
		LT[i] = 0;
		DT[i] = 0;
	}
	

	/***********************************************/
	//calculate LT and DT
	int countIndex_1;
	int countTransition_1;
	int offset_1 = 0;

	int offset_2 = height*MAX_OF_TRANSITION;
	int countTransition_2;
	int countIndex_2;

	int  j;
	//left to right and right to left
	for (i = 0; i < height; i++)
	{
		countIndex_1 = MAX_OF_TRANSITION*i;
		countTransition_1 = 0;
		for (j = 0; j < width; j++)		//left to right
		{
			if (countTransition_1 >= MAX_OF_TRANSITION) break;
			else if (*(data + i*width + j) != 0 && *(data + i*width + j - 1) == 0)
			{
				LT[countIndex_1] = width - j;
				DT[countIndex_1] = *(data + i*width + j);
				countIndex_1++;
				countTransition_1++;
			}
		}
		
		countIndex_2 = offset_2 + MAX_OF_TRANSITION*i;
		countTransition_2 = 0;
		for (j = width-1; j >= 0; j--)		//right to left
		{
			if (countTransition_2 >= MAX_OF_TRANSITION) break;
			else if (*(data + i*width + j) != 0 && *(data + i*width + j + 1) == 0)
			{
				LT[countIndex_2] = j+1;
				DT[countIndex_2] = *(data + i*width + j);
				countIndex_2++;
				countTransition_2++;
			}
		}
	}
	
	offset_1 = height*MAX_OF_TRANSITION*2;
	offset_2 = offset_1 + width*MAX_OF_TRANSITION;
	countTransition_1 = 0;
	countTransition_2 = 0;
	//top to bottom and bottom to top
	for (j = 0; j < width; j++)
	{
		countTransition_1 = 0;
		countIndex_1 = offset_1 + MAX_OF_TRANSITION * j;
		for (i = 0; i < height; i++) //top to bottom
		{
			if (countTransition_1 >= MAX_OF_TRANSITION) break;
			else if (*(data + i*width + j) != 0 && *(data + (i - 1)*width + j) == 0)
			{
				LT[countIndex_1] = height - i;
				DT[countIndex_1] = *(data + i*width + j);
				countIndex_1++;
				countTransition_1++;
			}
		}
		
		countTransition_2 = 0;
		countIndex_2 = offset_2 + MAX_OF_TRANSITION * j;
		for (i = height - 1; i >= 0; i--) //bottom to top
		{
			if (countTransition_2 >= MAX_OF_TRANSITION) break;
			else if (*(data + i*width + j) != 0 && *(data + (i + 1)*width + j) == 0)
			{
				LT[countIndex_2] = i+1;
				DT[countIndex_2] = *(data + i*width + j);
				countIndex_2++;
				countTransition_2++;
			}
		}
	}


	/************************************************/
	//Nornamlize DT and LT and push into output
	//output = LT && DT

	
	int dividedHeight = height / RESAMPLE_SIZE_HEIGHT;
	int dividedWidth = width / RESAMPLE_SIZE_WIDTH;
	int modularHeight = height % RESAMPLE_SIZE_HEIGHT;
	int modularWidth = width % RESAMPLE_SIZE_WIDTH;

	countIndex_1 = 0;		//index for LT in output vector
	countIndex_2 = (RESAMPLE_SIZE_HEIGHT + RESAMPLE_SIZE_WIDTH) * MAX_OF_TRANSITION * 2;	//index for DT in ouput vector

	int index = 0;

	int* total_1 = (int*)malloc(MAX_OF_TRANSITION * sizeof(int));	//calculate total correspond to index of TRANSITION LT
	int* total_2 = (int*)malloc(MAX_OF_TRANSITION * sizeof(int)); //DT
	
	/*
	cout<<"height is : "<<height<<"\n";
	cout<<"divide height is : "<<dividedHeight<<"\n";
	cout<<"modular height is : "<<modularHeight<<"\n";
	cout<<"width is : "<<width<<"\n";
	cout<<"divide width is : "<<dividedWidth<<"\n";
	cout<<"modular width is : "<<modularWidth<<"\n";
	*/
	int h, k;
	for (i = 0; i < 2; i++)	//left to right and right to left
		for (j = 0; j < RESAMPLE_SIZE_HEIGHT; j++)
		{
			if (j < modularHeight)
			{
				//init the total 
				for (h = 0; h < MAX_OF_TRANSITION; h++)
				{
					total_1[h] = 0;
					total_2[h] = 0;
				}
				//caculate the total
				for (k = 0; k < dividedHeight + 1; k++)
				{
					for (h = 0; h < MAX_OF_TRANSITION; h++)
					{
						total_1[h] += LT[index];
						total_2[h] += DT[index];
						index++;
					}
				}
				//get the average of total and assign to ouput
				for (h = 0; h < MAX_OF_TRANSITION; h++)
				{
					output[countIndex_1] = (double)(total_1[h]) / (double)((dividedHeight + 1) * width);
					output[countIndex_2] = (double)(total_2[h]) / (double)((dividedHeight + 1) * 10);
					countIndex_1++;
					countIndex_2++;
				}
			}
			else
			{
				//init the total 
				for (h = 0; h < MAX_OF_TRANSITION; h++)
				{
					total_1[h] = 0;
					total_2[h] = 0;
				}
				//caculate the total
				for (k = 0; k < dividedHeight; k++)
				{
					for (h = 0; h < MAX_OF_TRANSITION; h++)
					{
						total_1[h] += LT[index];
						total_2[h] += DT[index];
						index++;
					}
				}
				//get the average of total and assign to ouput
				for (h = 0; h < MAX_OF_TRANSITION; h++)
				{
					output[countIndex_1] = (double)(total_1[h]) / (double)((dividedHeight) * width);
					output[countIndex_2] = (double)(total_2[h]) / (double)((dividedHeight) * 10);
					countIndex_1++;
					countIndex_2++;
				}
			}
		}
	for (i = 0; i < 2; i++)	//top to bottom and bottom to top
		for (j = 0; j < RESAMPLE_SIZE_WIDTH; j++)
		{
			if (j < modularWidth)
			{
				//init the total 
				for ( h = 0; h < MAX_OF_TRANSITION; h++)
				{
					total_1[h] = 0;
					total_2[h] = 0;
				}
				//caculate the total
				
				for (k = 0; k < dividedWidth + 1; k++)
				{
					for (h = 0; h < MAX_OF_TRANSITION; h++)
					{
						total_1[h] += LT[index];
						total_2[h] += DT[index];
						index++;
					}
				}
				//get the average of total and assign to ouput
				for (h = 0; h < MAX_OF_TRANSITION; h++)
				{
					output[countIndex_1] = (double)(total_1[h]) / (double)((dividedWidth + 1) * height);
					output[countIndex_2] = (double)(total_2[h]) / (double)((dividedWidth + 1) * 10);
					countIndex_1++;
					countIndex_2++;
				}
			}
			else
			{
				//init the total 
				for (h = 0; h < MAX_OF_TRANSITION; h++)
				{
					total_1[h] = 0;
					total_2[h] = 0;
				}
				//caculate the total
				for (k = 0; k < dividedWidth; k++)
				{
					for (h = 0; h < MAX_OF_TRANSITION; h++)
					{
						total_1[h] += LT[index];
						total_2[h] += DT[index];
						index++;
					}
				}
				//get the average of total and assign to ouput
				for (h = 0; h < MAX_OF_TRANSITION; h++)
				{
					output[countIndex_1] = (double)(total_1[h]) / (double)((dividedWidth) * height);
					output[countIndex_2] = (double)(total_2[h]) / (double)((dividedWidth) * 10);
					countIndex_1++;
					countIndex_2++;
				}
			}
		}
	
	
	free(DT);
	free(LT);
	free(total_1);
	free(total_2);	
}