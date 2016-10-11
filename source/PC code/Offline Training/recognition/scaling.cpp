#include "stdafx.h"
#include "scaling.h"

/*release noise by threshold*/
void threshold(IplImage* image, int threshold)
{
	uchar* ptr;
	for (int i=0;i<image->height;i++)
	{
		ptr = (uchar*)(image->imageData + i * image->widthStep);
		for (int j=0;j<image->width;j++)
		{
			if (ptr[j] < threshold) ptr[j] = 0;
			else ptr[j] = 255;
		}
	}
}

/*
	use average_kernel for image
	x y : is the location of pixel in image
	w : width of kernel
	h : height of kernel
*/
int average_kernel(IplImage* image, int x, int y, int w, int h)
{
	uchar* ptr;
	int sum = 0;
	x -= w/2;
	y -= h/2;
	for (int i=0;i<h;i++)
	{
		if (y < 0 || y >= image->height) {sum += 255 * w; y++; continue;}		//if go out the image
		else
		{
			ptr = (uchar*)(image->imageData + y * image->widthStep);
			for (int j=0;j<w;j++)
			{
				if (x < 0 || x >= image->width) {sum += 255; x++; continue;}	//if go out the image
				else
				{
					sum += ptr[x];
					x++;
				}
				
			}
			y++;
		}
		
	}
	return sum/(w*h);
}

/*
	scaling the image by method bilinear
	src : source image
	dst : destiny image
	width_of_kernel
	height_of_kernel

*/
void resize_image(IplImage* src, IplImage* dst, int width_of_kernel,int height_of_kernel)
{
	double ratio_w = (double)(src->width) / (double)(dst->width);
	double ratio_h = (double)(src->height) / (double)(dst->height);
	uchar* ptr;
	for (int i=0;i<dst->height;i++)
	{
		int y = (int)(i*ratio_h);
		ptr = (uchar*)(dst->imageData + i * dst->widthStep);
		for (int j=0;j<dst->width;j++)
		{
			int x = (int)(j*ratio_w);
			ptr[j] = average_kernel(src, x, y, width_of_kernel, height_of_kernel);
		}
	}
	threshold(dst, 220);			//threshold the image
}
/*
	find left and right boundary
	a : is the ouput , a[0] = left, a[1] = right, a[2] = top , a[3] = bottom
	threshold : input
*/
void find_point_to_cut(IplImage *image, int* a)
{
	uchar* ptr;
	a[0] = -1;
	a[1] = -1;
	a[2] = -1;
	a[3] = -1;
	for (int col = 0;col<image->width && a[0] == -1;col++)
	{
		for (int row = 0 ; row < image->height;row++)
		{
			ptr = (uchar*)image->imageData + row*image->widthStep;
			if (ptr[col] == 1) {
				a[0] = col;
				break;
			}
		}
	}
	for (int col = image->width -1 ;col >= 0 && a[1] == -1;col--)
	{
		for (int row = 0 ; row < image->height;row++)
		{
			ptr = (uchar*)image->imageData + row*image->widthStep;
			if (ptr[col] == 1) {
				a[1] = col;
				break;
			}
		}
	}
	for (int row = 0;row<image->height&&a[2] == -1;row++)
	{
		ptr = (uchar*)image->imageData+row*image->widthStep;
		for (int col = 0; col<image->width;col++)
		{
			if (ptr[col] == 1)
			{
				a[2] = row;
				break;
			}
		}
	}
	for (int row = image->height-1;row>= 0 &&a[3] == -1;row--)
	{
		ptr = (uchar*)image->imageData+row*image->widthStep;
		for (int col = 0; col<image->width;col++)
		{
			if (ptr[col] == 1)
			{
				a[3] = row;
				break;
			}
		}
	}
}

/*
	cutting the image by left and right boundary
*/
IplImage* cut_image(IplImage* src)
{
	IplImage* des;
	int *a = new int[4];
	find_point_to_cut(src,a);
	int left = a[0];
	int right = a[1];
	int top = a[2];
	int bot = a[3];
	int new_width = right - left;
	int new_height = bot - top;
	int expand_width = 0;
	int expand_height = 0;
	if (new_width > 15)
	{
		expand_width =4;
		new_width += expand_width;
	}
	else
	{
		expand_width = 12;
		new_width += expand_width;
	}
	
	expand_height = 4;
	new_height += expand_height;
	
	uchar* ptr;
	uchar* ptr_src;
	int temp;
	des = cvCreateImage(cvSize(new_width,new_height),src->depth,src->nChannels);
	for (int i = expand_height/2;i<des->height-expand_height/2;i++)
	{
		ptr = (uchar*)des->imageData + i*des->widthStep;
		ptr_src = (uchar*)src->imageData + (i-expand_height/2+top)*src->widthStep;
		for (int j=expand_width/2;j<des->width-expand_width/2;j++)
		{
			temp = ptr_src[j-expand_width/2+left];
			ptr[j] = temp;
		}
	}
	for (int i=0;i<expand_height/2;i++)
	{
		ptr = (uchar*)des->imageData + i*des->widthStep;
		ptr_src = (uchar*)des->imageData + (des->height-1-i)*des->widthStep;
		for (int j=0;j<des->width;j++)
		{
			ptr[j] = 0;
			ptr_src[j] = 0;
		}
	}
	for (int i=0;i<des->height;i++)
	{
		ptr = (uchar*)des->imageData + i*des->widthStep;
		for (int j = 0;j<expand_width/2;j++)
		{
			ptr[j] = 0;
			ptr[des->width-1-j] = 0;
		}
	}
	delete[] a;
	ptr = NULL;
	ptr_src = NULL;
	delete[] ptr;
	delete[] ptr_src;
	return des;
}


//another scaling agorithm
void scaling(IplImage* src, IplImage* des)
{
	uchar* ptr_src;
	uchar* ptr_des;

	//zero des image
	for (int i = 0; i<des->height; i++)
	{
		ptr_des = (uchar*)des->imageData + i*des->widthStep;
		for (int j = 0 ; j<des->width;j++)
		{
			ptr_des[j] = 0;
		}
	}

	int row;
	int col;
	double scale_row = double(des->height)/double(src->height);
	double scale_col = double(des->width)/double(src->width);
	for (int i = 0; i<src->height; i++)
	{
		ptr_src = (uchar*)src->imageData + i*src->widthStep;
		for (int j = 0 ; j<src->width;j++)
		{
			if (ptr_src[j] == 1)
			{
				row = cvRound(i*scale_row);
				col = cvRound(j*scale_col);
				if (row < des-> height && col < des->width)
				{
					ptr_des = (uchar*)des->imageData + row*des->widthStep;
					ptr_des[col] = 1;
				}
			}
		}
	}
	ptr_src = NULL;
	ptr_des = NULL;
	delete[] ptr_src;
	delete[] ptr_des;
}