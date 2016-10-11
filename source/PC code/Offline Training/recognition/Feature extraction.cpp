#include "stdafx.h"
#include "Feature extraction.h"
void get_feature_extraction(IplImage* img,float left_2_right[2][21][3],float right_2_left[2][21][3],float top_2_bot[2][15][3], float bot_2_top[2][15][3])
{
	uchar* ptr;
	float LT;
	float DT;
	int height = img->height;
	int width = img->width;
	int r,c;			//row and col cua LT va DT
	
	//left_2_right
	r = 0;
	c = 0;
	for (int i=0;i<height;i++)
	{
		ptr = (uchar*)img->imageData+i*img->widthStep;
		for (int j=0;j<width;j++)
		{
			if (ptr[j] != 0 && ((j>0 && ptr[j-1] == 0) || 1))
			{
				LT = float(width - j)/float(width);
				DT = float(ptr[j])/10;
				if (c < N) 
				{
					left_2_right[0][r][c] = LT;			//new value
					left_2_right[1][r][c] = DT;
					c++;
				}
			}
		}		
		r++;//new row
		c = 0;
	}

	//right_2_left
	r = 0;
	c = 0;
	for (int i=0;i<height;i++)
	{
		ptr = (uchar*)img->imageData+i*img->widthStep;
		for (int j=width-1;j>=0;j--)
		{
			if (ptr[j] != 0 && ((j<width-1 && ptr[j+1] == 0) || 1))
			{
				LT = float(j)/float(width);
				DT = float(ptr[j])/10;
				if (c < N) 
				{
					right_2_left[0][r][c] = LT;			//new value
					right_2_left[1][r][c] = DT;
					c++;
				}
			}
		}		
		r++;//new row
		c = 0;
	}
	uchar* ptr2;
	int k = 0;		//tham so them vao
	//top_2_bot
	r = 0;
	c = 0;
	for (int j=0;j<width;j++)
	{
		for (int i=0;i<img->height;i++)
		{
			ptr = (uchar*)img->imageData + i*img->widthStep;
			if (ptr[j] != 0)
			{
				if (i > 0) 
				{
					ptr2 = (uchar*)img->imageData + (i-1)*img->widthStep;
					k = ptr2[j];
				}
				else k = 0;
				if (k == 0)
				{
					LT = float(height-i)/float(height);
					DT = float(ptr[j])/10;
					if (c < N) 
					{
						top_2_bot[0][r][c] = LT;
						top_2_bot[1][r][c] = DT;
						c++;
					}
				}
			}
		}
		r++;
		c = 0;
	}
	//bot_2_top
	r = 0;
	c = 0;
	for (int j=width-1;j>=0;j--)
	{
		for (int i=0;i<img->height;i++)
		{
			ptr = (uchar*)img->imageData + i*img->widthStep;
			if (ptr[j] != 0)
			{
				if (i < height-1) 
				{
					ptr2 = (uchar*)img->imageData + (i+1)*img->widthStep;
					k = ptr2[j];
				}
				else k = 0;
				if (k == 0)
				{
					LT = float(i)/float(height);
					DT = float(ptr[j])/10;
					if (c < N) 
					{
						bot_2_top[0][r][c] = LT;
						bot_2_top[1][r][c] = DT;
						c++;
					}
				}
			}
		}
		r++;
		c = 0;
	}
	ptr = NULL;
	ptr2 = NULL;
	delete[] ptr;
	delete[] ptr2;

}
void normalize_feature_extraction(IplImage* img, float left_2_right[2][21][3],float right_2_left[2][21][3],float top_2_bot[2][15][3], float bot_2_top[2][15][3],
									float n_left_2_right[2][7][3],float n_right_2_left[2][7][3],float n_top_2_bot[2][5][3], float n_bot_2_top[2][5][3])
{
	int width = img->width;
	int height = img->height;
	float n_LT;
	float n_DT;
	for (int i = 0;i<height;i+=3)
	{
		for (int j=0;j<N;j++)
		{
			n_LT = (left_2_right[0][i][j]+left_2_right[0][i+1][j]+left_2_right[0][i+2][j])/3;
			n_DT = (left_2_right[1][i][j]+left_2_right[1][i+1][j]+left_2_right[1][i+2][j])/3;
			n_left_2_right[0][i/3][j] = n_LT;
			n_left_2_right[1][i/3][j] = n_DT;
			n_LT = (right_2_left[0][i][j]+right_2_left[0][i+1][j]+right_2_left[0][i+2][j])/3;
			n_DT = (right_2_left[1][i][j]+right_2_left[1][i+1][j]+right_2_left[1][i+2][j])/3;
			n_right_2_left[0][i/3][j] = n_LT;
			n_right_2_left[1][i/3][j] = n_DT;
		}
	}
	for (int i = 0;i<width;i+=3)
	{
		for (int j=0;j<N;j++)
		{
			n_LT = (top_2_bot[0][i][j]+top_2_bot[0][i+1][j]+top_2_bot[0][i+2][j])/3;
			n_DT = (top_2_bot[1][i][j]+top_2_bot[1][i+1][j]+top_2_bot[1][i+2][j])/3;
			n_top_2_bot[0][i/3][j] = n_LT;
			n_top_2_bot[1][i/3][j] = n_DT;
			n_LT = (bot_2_top[0][i][j]+bot_2_top[0][i+1][j]+bot_2_top[0][i+2][j])/3;
			n_DT = (bot_2_top[1][i][j]+bot_2_top[1][i+1][j]+bot_2_top[1][i+2][j])/3;
			n_bot_2_top[0][i/3][j] = n_LT;
			n_bot_2_top[1][i/3][j] = n_DT;
		}
	}
}