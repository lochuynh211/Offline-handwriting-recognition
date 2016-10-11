
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

/*
	using for finding the boundary of image 
	An example of utilisation is : 
		IplImage* img_src = cvLoadImage("C:\\test1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
		threshold(img_src, 200);
		IplImage* img_des = cvCreateImage(cvSize(img_src->width,img_src->height),img_src->depth,img_src->nChannels);
		finding_boundary(img_src, img_des);
*/
int find_k(int phi[7], int pc);
void tracing(int** img, int x0, int y0, int type);
void finding_boundary(IplImage* img_src, IplImage* img_des);
