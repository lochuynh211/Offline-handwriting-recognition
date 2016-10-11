
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
struct brand_point
{
	int row[20];
	int col[20];
	int n;
};

void init_starting_point(IplImage* img, int* r, int *l);
bool check_corner(int pre_r, int pre_c, int cur_r, int cur_c, int pos_r, int pos_c);
void find_starting_point(IplImage* img);
void travelling(IplImage* img, int start_r, int start_c);
int find_next_point(IplImage* img, int r, int c, int *next_r, int *next_c);
int to_number(int r, int c, int n_r, int n_c);
void normalize(IplImage*img);
void travelling_normalize(IplImage* img,int **temp, int start_r, int start_c);
int find_next_value(IplImage* img,int **temp, int r, int c, int* next_r, int* next_c);