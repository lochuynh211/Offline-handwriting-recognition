#include <math.h>
#include <stdio.h>
#include <string.h>
#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <fstream>
#include <io.h>

using std::cout;
using std::ifstream;
using std::ofstream;

#define					NORMAL_LEVEL			200
#define					NORMAL_STACK_MAXSIZE	100

#if !defined(IMAGE_H)
#define IMAGE_H
typedef struct _Image{
		int mHeight;
		int mWidth;
		unsigned char* mData;
}Image;
#endif


#if !defined(STACK_H)
#define STACK_H
typedef struct _Stack{
		int maxSize;
		int* row;
		int* col;
		int n;
}Stack;
#endif

//Image
void createImage(Image* src, int height, int width);
void printImage(Image* src);
void copyImage(Image *src, Image *des);
Image* create_Image(IplImage* src);
void threshold(Image* input, int level);

//stack
void createStack(Stack* src, int max);
void pushStack(Stack* src, int pushRow, int pushCol);
void pushStack2(Stack* src, int pushRow, int pushCol);