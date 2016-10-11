#include "stdafx.h"
#include "finding boundary.h"

/*
	find k in step 3.1 and 3.2
	phi : input 
	pc : input
	output : return i , the k element in phi (phi[k] = pc)
*/
int find_k(int phi[7], int pc)
{
	for (int i=0;i<7;i++)
	{
		if (phi[i] == pc) return i;
	}
	return 0;
}

/*
	tracing the new contour
	(x0, y0) is the starting point of the countour
	type : type of countour (type = 0 <=>outer , type = 1 <=> inner)
*/
void tracing(int** img, int x0, int y0, int type)
{
	int i;
	int tcode[8];
	int tcode0[8];
	int offset_x[8];
	int offset_y[8];
	int pcode;
	int x[8];
	int y[8];
	tcode0[0] = 7;tcode0[1] = 7;tcode0[2] = 1;tcode0[3] = 1;tcode0[4] = 3;tcode0[5] = 3;tcode0[6] = 5;tcode0[7] = 5;
	offset_x[0] = 1;offset_x[1] = 1;offset_x[2] = 0;offset_x[3] = -1;offset_x[4] = -1;offset_x[5] = -1;offset_x[6] = 0;offset_x[7] = 1;
	offset_y[0] = 0;offset_y[1] = -1;offset_y[2] = -1;offset_y[3] = -1;offset_y[4] = 0;offset_y[5] = 1;offset_y[6] = 1;offset_y[7] = 1;
	int xc, yc;
	//use for test3.1 and 3.2
	int n;
	int k;
	int phi[2][7];
	phi[0][0] = 5;phi[0][1] = 6;phi[0][2] = 7;phi[0][3] = 0;phi[0][4] = 1;phi[0][5] = 2;phi[0][6] = 3;
	phi[1][0] = 1;phi[1][1] = 2;phi[1][2] = 3;phi[1][3] = 4;phi[1][4] = 5;phi[1][5] = 6;phi[1][6] = 7;
	int pc;
//step1:
	img[y0][x0] = 3;
	xc = x0;
	yc = y0;
	i = 0;
	if (type == 0) 
	{
		img[y0][x0-1] = 2;
		pcode = 7;
	}
	else 
	{
		img[y0][x0+1] = 2;
		pcode = 3;
	}
	//step2
step2:
	if (i > 6) goto step5;
	else
	{
		if (i>0) tcode[i] = (tcode[i-1]+1)%8;
		else tcode[i] = tcode0[pcode];
		x[i] = xc + offset_x[tcode[i]];
		y[i] = yc + offset_y[tcode[i]];
		if (x[i] < 0 || x[i] >= 15 || y[i] < 0 || y[i] >= 21) goto step5;
		if (img[y[i]][x[i]] == 0 || img[y[i]][x[i]] == 2) 
		{
			img[y[i]][x[i]] = 2;
			i++;
			goto step2;
		}
		else 
		{
			img[y[i]][x[i]] = 3;
			goto step3;
		}
	}
step3:
	if (x[i] == x0 && y[i] == y0) 
	{
		goto step31;
	}
	else
	{
		goto step4;
	}
step31:
	pc = (tcode[i]+4)%8;
	k = find_k(phi[type], pc);
	n = k + 1;
	xc = x0;
	yc = y0;
step32:
	if (n > 6) goto step5;
	else
	{
		tcode[i] = phi[type][n];
		x[i] = xc + offset_x[tcode[i]];
		y[i] = yc + offset_y[tcode[i]];
		if (img[y[i]][x[i]] == 0 || img[y[i]][x[i]] == 2) 
		{
			img[y[i]][x[i]] = 2;
			n++;
			goto step32;
		}
		else 
		{
			img[y[i]][x[i]] = 3;
			goto step4;
		}
	}
step4:
		xc = x[i];
		yc = y[i];
		pcode = tcode[i];
		i = 0;
		goto step2;
step5:
	return;
}
void finding_boundary(IplImage* img_src, IplImage* img_des)
{
	uchar* ptr;
	//create a temporary matrix
	int** img_temp = new int*[img_src->height];
	for (int i=0;i<img_src->height;i++) 
	{
		img_temp[i] = new int[img_src->width];
		ptr = (uchar*)img_src->imageData + img_src->widthStep*i;
		for (int j=0;j<img_src->width;j++)
		{
			if (ptr[j]==1) img_temp[i][j] = 1;
			else img_temp[i][j] = 0;
		}
	}
	//tracing the contour
	for (int y=0;y<img_src->height;y++) 
		for (int x=0;x<img_src->width-1;x++)
		{
			if ((img_temp[y][x] == 0 && img_temp[y][x+1] == 1) || (img_temp[y][x] == 2 && img_temp[y][x+1] == 1))
			{
				tracing(img_temp, x+1, y, 0);
			}
			else if ((img_temp[y][x] == 1 && img_temp[y][x+1] == 0) || (img_temp[y][x] == 3 && img_temp[y][x+1] == 0))
			{
				tracing(img_temp, x, y, 1);
			}
		}
	//copy to destiny image
	for (int y=0;y<img_src->height;y++) 
	{
		ptr = (uchar*)img_des->imageData + y*img_des->widthStep;
		for (int x=0;x<img_src->width;x++)
		{
			if (img_temp[y][x] == 3) ptr[x] = 1;
			else ptr[x] = 0;
		}		
	}
}
