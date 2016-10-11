#include "pre_processing.h"


void cutImage(Image* input, Image* output, int group)
{
	int RESAMPLE_SIZE_HEIGHT;
	int RESAMPLE_SIZE_WIDTH;
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
	int left = -1;
	int right = -1;
	int top = -1;
	int bottom = -1;

	int height = input->mHeight;
	int width = input->mWidth;

	int row, col;
	//find left boundary to cut here
	for (col = 0; col < width && left == -1; col++)
	{
		for (row = 0 ; row < height; row++)
		{

			if (input->mData[width * row + col] == 1) {
				left = col;
				break;
			}
		}
	}
	//find right boundary to cut here
	for (col = width - 1; col >= 0 && right == -1; col--)
	{
		for (row = 0; row < height; row++)
		{
			if (input->mData[width * row + col] == 1) {
				right = col;
				break;
			}
		}
	}
	//find top boundary to cut here
	for (row = 0; row < height && top == -1; row++)
	{
		for (col = 0; col<width; col++)
		{
			if (input->mData[width * row + col] == 1)
			{
				top = row;
				break;
			}
		}
	}
	//find bottom boundary to cut here
	for (row = height-1; row >= 0 && bottom == -1; row--)
	{
		for (col = 0; col < width; col++)
		{
			if (input->mData[width * row + col] == 1)
			{
				bottom = row;
				break;
			}
		}
	}

	//cut function here
	int outWidth = right - left + 1 ;
	int outHeight = bottom - top + 1 ;

	outWidth += EXPAND_WIDTH;  // cong nhieu pixel ngang cho chu  co be ngang nho
	outHeight += EXPAND_HEIGHT;

	if (outHeight < RESAMPLE_SIZE_HEIGHT) outHeight = RESAMPLE_SIZE_HEIGHT;
	if (outWidth < RESAMPLE_SIZE_WIDTH) outWidth = RESAMPLE_SIZE_WIDTH;

	createImage(output, outHeight, outWidth);

	int i, j;
	for (i = EXPAND_HEIGHT / 2; i < outHeight - EXPAND_HEIGHT / 2; i++)
	{
		unsigned char* out = output->mData + i * outWidth;
		unsigned char* in = input->mData + width * (i - EXPAND_HEIGHT/2 + top);
		for (j = EXPAND_WIDTH/2; j < outWidth - EXPAND_WIDTH/2; j++)
		{
			out[j] = in[j - EXPAND_WIDTH/2 + left];
		}
	}
	for (i = 0; i < EXPAND_HEIGHT/2; i++)
	{
		unsigned char* outLeft = output->mData + i*outWidth;
		unsigned char* outRight = output->mData + (outHeight - 1 - i)*outWidth;
		for (j = 0; j < outWidth; j++)
		{
			outLeft[j] = 0;
			outRight[j] = 0;
		}
	}
	for (i = 0; i < outHeight; i++)
	{
		unsigned char* out = output->mData + i*outWidth;
		for (j = 0; j < EXPAND_WIDTH/2; j++)
		{
			out[j] = 0;
			out[outWidth - 1- j] = 0;
		}
	}
	output = output;
}

int checkCond1(Image* input, Image* output, int row, int col)
{
	int p2,p3,p4,p5,p6,p7,p8,p9;
	p2 = p3 = p4 = p5 = p6 = p7 = p8 = p9 = 0;
	
	if (input->mData[row*input->mWidth + col] == 0) return 0;
	if (col > 0) p8 = input->mData[row*input->mWidth + col - 1];
	if (col < input->mWidth - 1) p4 = input->mData[row*input->mWidth + col + 1];
	if (row > 0) 
	{
		if (col > 0) p9 = input->mData[(row - 1)*input->mWidth + col - 1];
		if (col < input->mWidth - 1) p3 = input->mData[(row - 1)*input->mWidth + col + 1];
		p2 = input->mData[(row - 1)*input->mWidth + col];
	}
	if (row < input->mHeight - 1) 
	{
		if (col > 0) p7 = input->mData[(row + 1)*input->mWidth + col - 1];
		if (col < input->mWidth - 1) p5 = input->mData[(row + 1)*input->mWidth + col + 1];
		p6 = input->mData[(row + 1)*input->mWidth + col];
	}
	int BP = 0;
	int AP = 0;

	//caculate BP 
	BP = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

	if (BP < 2 || BP > 6) return 0;

	//caculate AP
	if (p2 == 0 && p3 == 1) AP++;		//p2 p3
	if (p3 == 0 && p4 == 1) AP++;		//p3 p4
	if (p4 == 0 && p5 == 1) AP++;		//p4 p5
	if (p5 == 0 && p6 == 1) AP++;		//p5 p6
	if (p6 == 0 && p7 == 1) AP++;		//p6 p7
	if (p7 == 0 && p8 == 1) AP++;		//p7 p8
	if (p8 == 0 && p9 == 1) AP++;		//p8 p9
	if (p9 == 0 && p2 == 1) AP++;		//p9 p2
	
	if (AP != 1) return 0;
	
	//another condition
	if (p4 == 0 || p6 == 0 || (p2 == 0 && p8 == 0))	//p4 p6 p2&&p8
	{
		output->mData[row*output->mWidth + col] = 0;
		return 1;
	}
	return 0;
}

int checkCond2(Image* input, Image* output, int row, int col)
{
	int p2,p3,p4,p5,p6,p7,p8,p9;
	p2 = p3 = p4 = p5 = p6 = p7 = p8 = p9 = 0;
	
	if (input->mData[row*input->mWidth + col] == 0) return 0;
	if (col > 0) p8 = input->mData[row*input->mWidth + col - 1];
	if (col < input->mWidth - 1) p4 = input->mData[row*input->mWidth + col + 1];
	if (row > 0) 
	{
		if (col > 0) p9 = input->mData[(row - 1)*input->mWidth + col - 1];
		if (col < input->mWidth - 1) p3 = input->mData[(row - 1)*input->mWidth + col + 1];
		p2 = input->mData[(row - 1)*input->mWidth + col];
	}
	if (row < input->mHeight - 1) 
	{
		if (col > 0) p7 = input->mData[(row + 1)*input->mWidth + col - 1];
		if (col < input->mWidth - 1) p5 = input->mData[(row + 1)*input->mWidth + col + 1];
		p6 = input->mData[(row + 1)*input->mWidth + col];
	}
	int BP = 0;
	int AP = 0;

	//caculate BP 
	BP = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

	if (BP < 2 || BP > 6) return 0;

	//caculate AP
	if (p2 == 0 && p3 == 1) AP++;		//p2 p3
	if (p3 == 0 && p4 == 1) AP++;		//p3 p4
	if (p4 == 0 && p5 == 1) AP++;		//p4 p5
	if (p5 == 0 && p6 == 1) AP++;		//p5 p6
	if (p6 == 0 && p7 == 1) AP++;		//p6 p7
	if (p7 == 0 && p8 == 1) AP++;		//p7 p8
	if (p8 == 0 && p9 == 1) AP++;		//p8 p9
	if (p9 == 0 && p2 == 1) AP++;		//p9 p2
	
	if (AP != 1) return 0;
	
	//another condition
	if (p2 == 0 || p8 == 0 || (p4 == 0 && p6 == 0 ))	//p4 p6 p2&&p8
	{
		output->mData[row*output->mWidth + col] = 0;
		return 1;
	}
	return 0;
}
void thinning(Image* input, Image* output)
{
	
	int width = input->mWidth;
	int height = input->mHeight;
	createImage(output, height, width);	//tao output moi

	copyImage(input, output);

	int i, j;
	int terminal = 1;				//ket thuc vong lap
	while (terminal)
	{
		terminal = 0;
		for (i = 0;i < height; i++) //gia tri xoa duoc luu vao dst
			for (j = 0; j < width; j++)
			{
				if (checkCond1(input, output, i, j)) terminal = 1; //kiem tra 4 dkien a,b,c,d
			}	
		copyImage(output, input);  //cap nhat vo src
		
		if (!terminal) break;
		terminal = 0;
		for (i = 0; i < height; i++)
			for (j = 0; j < width; j++)
			{
				if (checkCond2(input, output, i, j)) terminal = 1; //kiem tra a,b,c2,d2
			}
		copyImage(output, input);  //cap nhat vo src
	}
	////cout<<"\n";
	//output->printImage();
	//our improvement
	int row, col;
	for (row = 0; row < height; row++)
	{
		for (col = 0; col < width; col++)
		{
			if (*(output->mData + row*width + col) == 1)
			{
				int p2,p3,p4,p5,p6,p7,p8,p9;
				p2 = p3 = p4 = p5 = p6 = p7 = p8 = p9 = 0;
					
				if (col > 0) p8 = *(output->mData + row*input->mWidth + col - 1);
				if (col < input->mWidth - 1) p4 =  *(output->mData + row*input->mWidth + col + 1);
				if (row > 0) 
				{
					if (col > 0) p9 = *(output->mData + (row - 1)*input->mWidth + col - 1);
					if (col < input->mWidth - 1) p3 = *(output->mData + (row - 1)*input->mWidth + col + 1);
					p2 = *(output->mData + (row - 1)*input->mWidth + col);
				}
				if (row < input->mHeight - 1) 
				{
					if (col > 0) p7 = *(output->mData + (row + 1)*input->mWidth + col - 1);
					if (col < output->mWidth - 1) p5 = *(input->mData + (row + 1)*input->mWidth + col + 1);
					p6 =*(output->mData + (row + 1)*input->mWidth + col);
				}

				if (p2 == 1 && p4 == 1 && p7 == 0) 
				{	
					*(output->mData + row*width + col) = 0;
				}
				else if (p4 == 1 && p6 == 1 && p9 == 0)
				{	
					*(output->mData + row*width + col) = 0;
				}
				else if	(p6 == 1 && p8 == 1 && p3 == 0)
				{	
					*(output->mData + row*width + col) = 0;
				}
				else if	(p8 == 1 && p2 == 1 && p5 == 0)
				{	
					*(output->mData + row*width + col) = 0;
				}
			}
		}
	}
}

void preProcessAll(Image* input, Image* output, int group)
{
	Image* tmp1 = (Image*)malloc(sizeof(Image));
	cutImage(input, tmp1, group);
	Image* tmp2 = (Image*)malloc(sizeof(Image));
	thinning(tmp1, tmp2);
	cutImage(tmp2, output, group);
	free(tmp1);
	free(tmp2);
}