#include "cv.h"
#include "highgui.h"
#include <stdio.h>

/*
	use for thinning the image
	input is the gray_scale image
	after call , this image will be change in to its skeleton
	Example
		thinning(src);
	src is input and output
*/
void change_to_binary(IplImage* img);
void print_img(IplImage *img);
void copy_img(IplImage* src, IplImage* des);
bool check_cond1(IplImage* src, int row, int col, IplImage* des);
bool check_cond2(IplImage* src, int row, int col, IplImage* des);
void thinning(IplImage* src);

