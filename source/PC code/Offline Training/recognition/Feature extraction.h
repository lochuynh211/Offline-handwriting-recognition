#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#define N 3				//number or crossing 
#define NOR 3			//number of normalization
// LT = array[0][21][3]     or array[0][15][3] 
// DT = array[1][21][3]	 or array[1][15][3]
void get_feature_extraction(IplImage* img,float left_2_right[2][21][3],float right_2_left[2][21][3],float top_2_bot[2][15][3], float bot_2_top[2][15][3]);
void normalize_feature_extraction(IplImage* img, float left_2_right[2][21][3],float right_2_left[2][21][3],float top_2_bot[2][15][3], float bot_2_top[2][15][3],
									float n_left_2_right[2][7][3],float n_right_2_left[2][7][3],float n_top_2_bot[2][5][3], float n_bot_2_top[2][5][3]);