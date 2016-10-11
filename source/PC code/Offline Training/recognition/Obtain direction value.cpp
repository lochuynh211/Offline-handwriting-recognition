#include "stdafx.h"
#include "Obtain direction value.h"
#include "thinning.h"
brand_point stack;
void init_starting_point(IplImage* img, int* r, int *c)
{
	uchar* ptr;
	for (int i=img->height-1;i>=0;i--)
	{
		ptr = (uchar*)img->imageData + i*img->widthStep;
		for (int j = 0; j < img->width;j++)
		{
			if (ptr[j] == 1) 
			{
				*r = i;
				*c = j;
				return;
			}
		}
	}
}
bool check_corner(int pre_r, int pre_c, int cur_r, int cur_c, int pos_r, int pos_c)
{
	//(up right || down left)  && (down right || up left)
	if ((((pre_r == cur_r - 1) && (pre_c == cur_c - 1)) || ((pre_c == cur_r + 1) && (pre_c == cur_c + 1))) && (((pos_r = cur_r + 1) && (pos_c = cur_c - 1)) || ((pos_r == cur_r - 1) && (pos_c == cur_c + 1)))) return true; 
	//(down right || up left) && (up right || down left)
	else if ((((pos_r == cur_r - 1) && (pos_c == cur_c - 1)) || ((pos_c == cur_r + 1) && (pos_c == cur_c + 1))) && (((pre_r = cur_r + 1) && (pre_c = cur_c - 1)) || ((pre_r == cur_r - 1) && (pre_c == cur_c + 1)))) return true;
	else return false;
}
void reduce_stack(IplImage* img)
{
	uchar* ptr;
	for (int i=0;i<stack.n;i++)
	{
		ptr = (uchar*)img->imageData + stack.row[i]*img->widthStep;
		if (ptr[stack.col[i]] != 1) 
		{
			for (int j=i;j<stack.n-1;j++)
			{
				stack.row[j] = stack.row[j+1];
				stack.col[j] = stack.col[j+1];
			}
			i--;
			stack.n--;
		}
	}
}
void find_starting_point(IplImage* img)
{
	uchar* ptr;
	for (int i=img->height-1;i>=0;i--)
	{
		ptr = (uchar*)img->imageData+i*img->widthStep;
		for (int j=0;j<img->width;j++)
		{
			if (ptr[j] == 1)
			{
				if (i==1 && j == 4) 
					printf("");
				int start_r, start_c;
				stack.n = -1;
				init_starting_point(img, &start_r, &start_c);
				travelling(img,start_r,start_c);
				while (stack.n > 0)
				{
					reduce_stack(img);
					stack.n--;
					if (stack.n < 0) break;
					travelling(img,stack.row[stack.n],stack.col[stack.n]);	
				}
			}
		}
	}
	ptr = NULL;
	delete[] ptr;
}
struct remember_new_point
{
	int row[10];
	int col[10];
	int val[10];
	int n;
};
void travelling(IplImage* img, int start_r, int start_c)
{
	uchar* ptr;
	int cur_r, cur_c;
	int pos_r, pos_c;
	int old_direction;
	int new_direction;

	cur_r = start_r;
	cur_c = start_c;
	//khoi dong gia tri cho diem bat dau
	ptr = (uchar*)img->imageData + start_r*img->widthStep;
	ptr[start_c] = 8;
	find_next_point(img, cur_r, cur_c, &pos_r, &pos_c);
	
	//khoi tao gia tri khac

	int remember_row = -1;
	int remember_col = -1;
	int remember_length_line = 1;
	
	int one_point = to_number(cur_r,cur_c,pos_r,pos_c);
	old_direction = to_number(cur_r,cur_c,pos_r,pos_c);
	//cur_r = pos_r;
	//cur_c = pos_c;
	
	while (find_next_point(img, cur_r, cur_c, &pos_r, &pos_c))
	{
		new_direction = to_number(cur_r,cur_c,pos_r,pos_c);
		if (new_direction != old_direction)
		{
				remember_row = cur_r;
				remember_col = cur_c;
				remember_length_line = 1;
		}
		else
		{
			remember_length_line ++;
			if (remember_length_line >=2) 
			{
				if (remember_row != -1)
				{
					ptr = (uchar*)img->imageData + remember_row*img->widthStep;
					ptr[remember_col] = 8;
					remember_row = -1;
					remember_col = -1;
				}
			}
		}
		ptr = (uchar*)img->imageData + cur_r*img->widthStep;
		old_direction = new_direction;
		if (ptr[cur_c] != 8) ptr[cur_c] = new_direction;
		cur_c = pos_c;
		cur_r = pos_r;
	}
	if (cur_c != -1 && one_point)
	{
		ptr = (uchar*)img->imageData + cur_r*img->widthStep;
		ptr[cur_c] = old_direction;
	}
	//if (!one_point)
	
}
int find_next_point(IplImage* img, int r, int c, int *next_r, int *next_c)
{
	*next_r = -1;
	*next_c = -1;
	uchar* ptr;
	ptr = (uchar*)img->imageData + r*img->widthStep;
	if (c > 0 && ptr[c-1] == 1)		//1
	{
		*next_r = r;
		*next_c = c - 1;
	}
	if (r > 0)
	{
		ptr = (uchar*)img->imageData + (r-1)*img->widthStep;
		if (c > 0)		//2
		{
			if (ptr[c-1] == 1)
			{
				if (*next_r != -1)
				{
					stack.row[stack.n] = r-1;
					stack.col[stack.n] = c-1;
					stack.n++;
				}
				else
				{
					*next_r = r-1;
					*next_c = c-1;
				}
			}
		}
		if (ptr[c] == 1)    //3
		{
			if (*next_r != -1)
			{
				stack.row[stack.n] = r-1;
				stack.col[stack.n] = c;
				stack.n++;
			}
			else
			{
				*next_r = r-1;
				*next_c = c;
			}
		}
		if (c < img->width - 1)    //4
		{
			if (ptr[c+1] == 1)
			{
				if (*next_r != -1)
				{
					stack.row[stack.n] = r-1;
					stack.col[stack.n] = c+1;
					stack.n++;
				}
				else
				{
					*next_r = r-1;
					*next_c = c+1;
				}
			}
		}
	}
	ptr = (uchar*) img->imageData + r*img->widthStep;
	if (c < img->width - 1 && ptr[c+1] == 1)  //5
	{
		if (*next_r != -1)
		{
			stack.row[stack.n] = r;
			stack.col[stack.n] = c+1;
			stack.n++;
		}
		else
		{
			*next_r = r;
			*next_c = c+1;
		}
	}
	if (r < img->height - 1)
	{
		ptr = (uchar*)img->imageData + (r+1) *img->widthStep;
		if (c < img->width - 1)  //6
		{
			if (ptr[c+1] == 1)
			{
				if (*next_r != -1)
				{
					stack.row[stack.n] = r+1;
					stack.col[stack.n] = c+1;
					stack.n++;
				}
				else
				{
					*next_r = r+1;
					*next_c = c+1;
				}
			}
		}
		if (ptr[c] == 1)  //7
		{
			if (*next_r != -1)
			{
				stack.row[stack.n] = r+1;
				stack.col[stack.n] = c;
				stack.n++;
			}
			else
			{
				*next_r = r+1;
				*next_c = c;
			}
		}
		if (c > 0)  //8
		{
			if (ptr[c-1] == 1) 
			{
				if (*next_r != -1)
				{
					stack.row[stack.n] = r+1;
					stack.col[stack.n] = c-1;
					stack.n++;
				}
				else
				{
					*next_r = r+1;
					*next_c = c-1;
				}
			}
		}
	}
	if (*next_r == -1) return 0;
	else return 1;
}
int to_number(int r, int c, int n_r, int n_c)
{
	if (((r - n_r == 1) && (c - n_c == 1))||((r - n_r == -1) && (c - n_c == -1))) return 3;
	else if (((r - n_r == 1) && (c - n_c == -1))||((r - n_r == -1) && (c - n_c == 1))) return 5;
	else if (c - n_c == 0) return 2;
	else if (r - n_r == 0) return 4;
	return 0;
}
void normalize(IplImage*img)
{
	int **temp = new int*[img->height];
	for (int i=0;i<img->height;i++)
	{
		temp[i] = new int[img->width];
		for (int j=0;j<img->width;j++)
		{
			temp[i][j] = 0;
		}
	}
	
	uchar* ptr;
	for (int i=img->height-1;i>=0;i--)
	{
		ptr = (uchar*)img->imageData + i*img->widthStep;
		for (int j=0;j<img->width;j++)
		{
			if (ptr[j] == 8)
			{							
				stack.n = 0;
				travelling_normalize(img,temp,i,j);
				while(stack.n > 0)
				{
					stack.n--;
					travelling_normalize(img,temp,stack.row[stack.n],stack.col[stack.n]);
				}
			}
		}
	}
	for (int i=0;i<img->height;i++)
	{
		delete [] temp[i];
	}
	delete [] temp;
	
	
	ptr = NULL;
	delete [] ptr;
}
void travelling_normalize(IplImage* img,int **temp, int start_r, int start_c)
{
	int r,c,next_c,next_r;
	r = start_r;
	c = start_c;
	uchar *ptr;
	int old_value;
	int k = find_next_value(img,temp,r,c,&next_r,&next_c);
	old_value = k;
	while (k)
	{
		if (k==8) k = to_number(r,c,next_r,next_c);
		ptr = (uchar*)img->imageData + r* img->widthStep;
		if (ptr[c] == 8)
		{
			old_value = k;
			ptr[c] = k;
		}
		else
		{
			ptr[c] = old_value;
		}
		temp[r][c] = 1;
		c = next_c;
		r = next_r;
		k = find_next_value(img,temp,r,c,&next_r,&next_c);
	}
}
int find_next_value(IplImage* img,int** temp, int r, int c, int* next_r, int* next_c)
{
	uchar* ptr;
	int result = 0;
	ptr = (uchar*)img->imageData + r*img->widthStep;
	*next_r = -1;
	if (c>0)  //1
	{
		if (temp[r][c-1] == 0)
		{
			if (ptr[c-1] != 0) 
			{	
				if (*next_r == -1)
				{	
					*next_r = r;
					*next_c = c-1;
					result =  ptr[c-1];
				}
				else
				{
					stack.row[stack.n] = r;
					stack.col[stack.n] = c-1;
					stack.n++;
				}
			}
		}
	}
	if (r > 0)
	{
		ptr = (uchar*)img->imageData + (r-1)*img->widthStep;
		if (c > 0)
		{
			if (temp[r-1][c-1] == 0)
			{
				if (ptr[c-1] != 0) 
				{	
					if (*next_r == -1)
					{	
						*next_r = r-1;
						*next_c = c-1;
						result =  ptr[c-1];
					}
					else
					{
						stack.row[stack.n] = r-1;
						stack.col[stack.n] = c-1;
						stack.n++;
					}
				}
			}
		}
		if (temp[r-1][c] == 0)
		{
			if (ptr[c] != 0) 
			{	
				if (*next_r == -1)
				{	
					*next_r = r-1;
					*next_c = c;
					result =  ptr[c];
				}
				else
				{
					stack.row[stack.n] = r-1;
					stack.col[stack.n] = c;
					stack.n++;
				}
			}
		}
		if (c<img->width-1)
		{
			if (temp[r-1][c+1] == 0)
			{
				if (ptr[c+1] != 0) 
				{	
					if (*next_r == -1)
					{	
						*next_r = r-1;
						*next_c = c+1;
						result =  ptr[c+1];
					}
					else
					{
						stack.row[stack.n] = r-1;
						stack.col[stack.n] = c+1;
						stack.n++;
					}
				}
			}
		}
	}
	ptr = (uchar*)img->imageData + r*img->widthStep;
	if (c < img->width-1)
	{
		if (temp[r][c+1] == 0)
		{
			if (ptr[c+1] != 0) 
			{	
				if (*next_r == -1)
				{	
					*next_r = r;
					*next_c = c+1;
					result =  ptr[c+1];
				}
				else
				{
					stack.row[stack.n] = r;
					stack.col[stack.n] = c+1;
					stack.n++;
				}
			}
		}
	}
	if (r < img->height-1)
	{
		ptr = (uchar*)img->imageData + (r+1)*img->widthStep;
		if (c<img->width-1)
		{
			if (temp[r+1][c+1] == 0)
			{
				if (ptr[c+1] != 0) 
				{	
					if (*next_r == -1)
					{	
						*next_r = r+1;
						*next_c = c+1;
						result =  ptr[c+1];
					}
					else
					{
						stack.row[stack.n] = r+1;
						stack.col[stack.n] = c+1;
						stack.n++;
					}
				}
			}
		}
		if (temp[r+1][c] == 0)
		{
			if (ptr[c] != 0) 
			{	
				if (*next_r == -1)
				{	
					*next_r = r+1;
					*next_c = c;
					result =  ptr[c];
				}
				else
				{
					stack.row[stack.n] = r+1;
					stack.col[stack.n] = c;
					stack.n++;
				}
			}
		}
		if (c > 0)
		{
			if (temp[r+1][c-1] == 0)
			{
				if (ptr[c-1] != 0) 
				{	
					if (*next_r == -1)
					{	
						*next_r = r+1;
						*next_c = c-1;
						result =  ptr[c-1];
					}
					else
					{
						stack.row[stack.n] = r+1;
						stack.col[stack.n] = c-1;
						stack.n++;
					}
				}
			}
		}
	}
	return result;
}