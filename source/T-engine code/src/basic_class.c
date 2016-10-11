#include "basic_class.h"

void createImage(Image* src, int height, int width)
{
	src->mHeight = height;
	src->mWidth = width;
	src->mData = (unsigned char*)malloc(src->mHeight * src->mWidth);
}

void printImage(Image* src)
{	
	int i,j;
	int width = src->mWidth;
	int height = src->mHeight;
	unsigned char* mData = src->mData;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			printf("%d",(int)(mData[i*width + j]));
		}
		printf("\n");
	}
}

void copyImage(Image *src, Image *des)
{
	int row,col;
	int width = src->mWidth;
	int height = src->mHeight;

	if (des->mHeight != height || des->mWidth != width) return;

	for (row = 0; row < height; row++)
	{	
		unsigned char* out = des->mData + row*width;
		unsigned char* in = src->mData + row*width;
		for (col = 0; col < width; col++) 
			out[col] = in[col];
	}
}


void createStack(Stack* src, int max)
{	
	src->row = (int*)malloc(max * sizeof(int)); //row of point
	src->col = (int*)malloc(max * sizeof(int)); // col of point
	src->n = 0;
	src->maxSize = max;
}



void pushStack(Stack* src, int pushRow, int pushCol)
{
	if (src->n > src->maxSize) 
	{
		////printf("tran stack1   %d \n", src->maxSize);
		return;
	}
	src->row[src->n] = pushRow;
	src->col[src->n] = pushCol;
	src->n++;
}


void pushStack2(Stack* src, int pushRow, int pushCol)
{
	int i;
	if (src->n > src->maxSize) 
	{
		////printf("tran stack2   %d \n", src->maxSize);
		return;
	}
	for (i = 0; i < src->n; i++)
	{
		if (src->row[i] == pushRow)
		{
			if (src->col[i] == pushCol) return;  //khong push
		}
	}
	src->row[src->n] = pushRow;
	src->col[src->n] = pushCol;
	src->n++;
}
