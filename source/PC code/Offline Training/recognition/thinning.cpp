#include "stdafx.h"
#include "thinning.h"
void change_to_binary(IplImage* img)
{
	uchar* ptr;
	for (int i =0;i<img->height;i++)
	{
		ptr = (uchar*)img->imageData + i*img->widthStep;
		for (int j=0;j<img->width;j++)
		{
			if (ptr[j] == 0) ptr[j] = 1;
			else ptr[j] = 0;
		}
	}
	ptr = NULL;
	delete [] ptr;
	
}
void print_img(IplImage *img)
{
	uchar* ptr;
	for (int i =0;i<img->height;i++)
	{
		ptr = (uchar*)img->imageData + i*img->widthStep;
		for (int j=0;j<img->width;j++)
		{
			printf("%d ",ptr[j]);
		}
		printf("\n");
	}
	printf("\n");
}
void copy_img(IplImage* src, IplImage* des)
{
	uchar* ptr_src;
	uchar* ptr_des;
	int temp;
	for (int i =0;i<src->height;i++)
	{
		ptr_src = (uchar*)src->imageData + i*src->widthStep;
		ptr_des = (uchar*)des->imageData + i*des->widthStep;
		for (int j=0;j<src->width;j++)
		{
			temp = ptr_src[j];
			ptr_des[j] = temp;
		}
	}
	//ptr_src = NULL;
	//ptr_des = NULL;
	//delete[] ptr_src;
	//delete[] ptr_des;
}
bool check_cond1(IplImage* src, int row, int col, IplImage* des)
{
	int p2,p3,p4,p5,p6,p7,p8,p9;
	p2 = p3 = p4 = p5 = p6 = p7 = p8 = p9 = 0;
	uchar* ptr1 = (uchar*)src->imageData + row*src->widthStep;
	if (ptr1[col] == 0) return false;
	if (col > 0) p8 = ptr1[col-1];
	if (col < src->height-1) p4 = ptr1[col + 1];
	uchar* ptr0;
	if (row > 0) 
	{
		ptr0 = (uchar*)src->imageData + (row-1)*src->widthStep;
		if (col > 0) p9 = ptr0[col-1];
		if (col < src->height-1) p3 = ptr0[col + 1];
		p2 = ptr0[col];
	}
	uchar* ptr2;
	if (row < src->height-1) 
	{
		ptr2 = (uchar*)src->imageData + (row+1)*src->widthStep;
		if (col > 0) p7 = ptr2[col-1];
		if (col < src->height-1) p5 = ptr2[col + 1];
		p6 = ptr2[col];
	}
	int BP = 0;
	int AP = 0;

	ptr0 = NULL;
	ptr1 = NULL;
	ptr2 = NULL;
	delete[] ptr0;
	delete[] ptr1;
	delete[] ptr2;
	//caculate BP 
	BP = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

	if (BP < 2 || BP > 6) return false;

	//caculate AP
	if (p2 == 0 && p3 == 1) AP++;		//p2 p3
	if (p3 == 0 && p4 == 1) AP++;		//p3 p4
	if (p4 == 0 && p5 == 1) AP++;		//p4 p5
	if (p5 == 0 && p6 == 1) AP++;		//p5 p6
	if (p6 == 0 && p7 == 1) AP++;		//p6 p7
	if (p7 == 0 && p8 == 1) AP++;		//p7 p8
	if (p8 == 0 && p9 == 1) AP++;		//p8 p9
	if (p9 == 0 && p2 == 1) AP++;		//p9 p2
	
	if (AP != 1) return false;
	
	//another condition
	if (p4 == 0 || p6 == 0 || (p2 == 0 && p8 == 0))	//p4 p6 p2&&p8
	{
		uchar* ptr = (uchar*)des->imageData + row*des->widthStep;
		ptr[col] = 0;
		ptr = NULL;
		delete[] ptr;
		return true;
	}
	return false;
}
bool check_cond2(IplImage* src, int row, int col, IplImage* des)
{
	int p2,p3,p4,p5,p6,p7,p8,p9;
	p2 = p3 = p4 = p5 = p6 = p7 = p8 = p9 = 0;
	uchar* ptr1 = (uchar*)src->imageData + row*src->widthStep;
	if (ptr1[col] == 0) return false;
	if (col > 0) p8 = ptr1[col-1];
	if (col < src->height-1) p4 = ptr1[col + 1];
	uchar* ptr0;
	if (row > 0) 
	{
		ptr0 = (uchar*)src->imageData + (row-1)*src->widthStep;
		if (col > 0) p9 = ptr0[col-1];
		if (col < src->height-1) p3 = ptr0[col + 1];
		p2 = ptr0[col];
	}
	uchar* ptr2;
	if (row < src->height-1) 
	{
		ptr2 = (uchar*)src->imageData + (row+1)*src->widthStep;
		if (col > 0) p7 = ptr2[col-1];
		if (col < src->height-1) p5 = ptr2[col + 1];
		p6 = ptr2[col];
	}
	int BP = 0;
	int AP = 0;

	ptr0 = NULL;
	ptr1 = NULL;
	ptr2 = NULL;
	delete[] ptr0;
	delete[] ptr1;
	delete[] ptr2;

	//caculate BP 
	BP = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

	if (BP < 2 || BP > 6) return false;

	//caculate AP
	if (p2 == 0 && p3 == 1) AP++;		//p2 p3
	if (p3 == 0 && p4 == 1) AP++;		//p3 p4
	if (p4 == 0 && p5 == 1) AP++;		//p4 p5
	if (p5 == 0 && p6 == 1) AP++;		//p5 p6
	if (p6 == 0 && p7 == 1) AP++;		//p6 p7
	if (p7 == 0 && p8 == 1) AP++;		//p7 p8
	if (p8 == 0 && p9 == 1) AP++;		//p8 p9
	if (p9 == 0 && p2 == 1) AP++;		//p9 p2
	
	if (AP != 1) return false;

	//another condition
	if ( p2 == 0 || p8 == 0 || (p4 == 0 && p6 == 0 ))	//p2 p8 
	{
		uchar* ptr = (uchar*)des->imageData + row*des->widthStep;
		ptr[col] = 0;
		ptr = NULL;
		delete[] ptr;
		return true;
	}
	return false;
}
void thinning(IplImage* src)
{
	IplImage* des = cvCreateImage(cvSize(src->width,src->height),src->depth,src->nChannels);
	copy_img(src,des);
	bool check_terminal = true;
	while (check_terminal)
	{
		check_terminal = false;
		for (int i=0;i<src->height;i++)
			for (int j=0;j<src->width;j++)
			{
				if (check_cond1(src,i,j,des)) check_terminal = true;
			}
		copy_img(des,src);
		if (!check_terminal) break;
		check_terminal = false;
		for (int i=0;i<src->height;i++)
			for (int j=0;j<src->width;j++)
			{
				if (check_cond2(src,i,j,des)) check_terminal = true;
			}
		copy_img(des,src);
	}
	//print_img(src);
	uchar*ptr1;
	uchar*ptr2;
	int a0,a1,a2,a3;
	int a = 0;  //khong cho phep xoa 1 bit lien tiep nhau
	for (int i=0;i<src->height;i++)
	{
		ptr1 = (uchar*)src->imageData+i*src->widthStep;
		for (int j=0;j<src->width;j++)
		{
			a--;
			if (ptr1[j] == 1)
			{
				a0 = 0;
				a1 = 0;
				a2 = 0;
				a3 = 0;
				if (j>0) a0 = ptr1[j-1];
				if (j<src->width-1) a2 = ptr1[j+1];
				if (i>0) 
				{
					ptr2 = (uchar*)src->imageData+(i-1)*src->widthStep;
					a1 = ptr2[j];
				}
				if (i<src->height-1) 
				{
					ptr2 = (uchar*)src->imageData+(i+1)*src->widthStep;
					a3 = ptr2[j];
				}
				if (a != 0 && (a0 == 0) && (a2 == 1) && ((a1 == 1) || (a3 == 1))) 
				{
					ptr1[j] = 0;
					a = 1;
				}
			}
		}
		a = 0;
		for (int j=src->width-1;j>=0;j--)
		{
			a--;
			if (ptr1[j] == 1)
			{
				a0 = 0;
				a1 = 0;
				a2 = 0;
				a3 = 0;
				if (j<src->width-1) a0 = ptr1[j+1];
				if (j>0) a2 = ptr1[j-1];
				if (i>0) 
				{
					ptr2 = (uchar*)src->imageData+(i-1)*src->widthStep;
					a1 = ptr2[j];
				}
				if (i<src->height-1) 
				{
					ptr2 = (uchar*)src->imageData+(i+1)*src->widthStep;
					a3 = ptr2[j];
				}
				if (a != 0 && (a0 == 0) && (a2 == 1) && ((a1 == 1) || (a3 == 1))) 
				{
					ptr1[j] = 0;
					a = 1;
				}

			}
		}
	}
	cvReleaseImage(&des);
	ptr1 = NULL;
	ptr2 = NULL;
	delete[] ptr1;
	delete[] ptr2;
}
