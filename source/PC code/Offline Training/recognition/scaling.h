
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

void threshold(IplImage* image, int threshold);
int average_kernel(IplImage* image, int x, int y, int w, int h);
void resize_image(IplImage* src, IplImage* dst,int width_of_kernel,int height_of_kernel); //my scaling algorithm
void find_point_to_cut(IplImage *image, int*a);
IplImage* cut_image(IplImage* src);			//use for cutting the image
void scaling(IplImage* src, IplImage* des);  //another scaling algorithm
