
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
void init();    //init the network
void training();	//training the network
int recognise(char*);		//recognize using network
void Write_weight();
void Read_weight();
void get_input_and_target(int num_of_sample);

