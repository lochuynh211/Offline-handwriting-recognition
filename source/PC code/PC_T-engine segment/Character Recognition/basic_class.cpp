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
		printf("tran stack1   %d \n", src->maxSize);
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
		printf("tran stack2   %d \n", src->maxSize);
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
Image* create_Image(IplImage* src)
{
	Image* result = (Image*)malloc(sizeof(Image));
	int mHeight = src->height;
	int mWidth = src->width;
	createImage(result, mHeight, mWidth);

	unsigned char* mData = result->mData;
	uchar* ptr;
	int i, j;
	for (i = 0; i < mHeight; i++)
	{
		ptr = (uchar*)(src->imageData + i * src->widthStep);
		for (j = 0; j < mWidth; j++)
		{
			*(mData + i * mWidth + j) = *(ptr + j);
		}
	}
	
	threshold(result, NORMAL_LEVEL);
	return result;
}

void threshold(Image* input, int level)
{
	int mHeight = input->mHeight;
	int mWidth = input->mWidth;
	unsigned char* mData = input->mData;
	int i, j;
	for (i = 0; i < mHeight; i++)
		for (j = 0; j < mWidth; j++)
		{
			if (*(mData + i * mWidth + j) < level)
				*(mData + i * mWidth + j) = 1;			//black pixel
			else
				*(mData + i * mWidth + j) = 0;			//white pixel
		}
}