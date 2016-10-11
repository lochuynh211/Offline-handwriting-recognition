#include "stdafx.h"
#include "neuron network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "scaling.h"
#include "finding boundary.h"
#include "thinning.h"
#include "Obtain direction value.h"
#include "Feature extraction.h"
#include <conio.h>
#include "io.h"
#using <system.dll>
using namespace System;
using namespace System::IO;

#define NUM_OF_SAMPLE 1000
#define Input 144
#define Hidden 50
#define Output 26
#define Sample 26
double w_in[Hidden][Input];		//50x144
double w_hid[Output][Hidden];	//26x50

double in[Input];
double target[Output];

double hid[Hidden];				//value tra ra cua hidden node
double out[Output];				//value tra ra cua output


void init()
{
	double temp;
	for (int i=0;i<Hidden;i++)
	{
		for (int j=0;j<Input;j++)
		{
			temp = rand();
			while (temp > 0.05 || temp < -0.05)
			{
				temp /= 2;
			}
			w_in[i][j] = temp;
		}
	}
	for (int i=0;i<Output;i++)
	{
		for (int j=0;j<Hidden;j++)
		{
			temp = rand();
			while (temp > 0.05 || temp < -0.05)
			{
				temp /= 2;
			}
			w_hid[i][j] = temp;
		}
	}
}
char* path = new char[];
char* tmp = "C:\\sample 1\\";
char* fold = new char[];
char* file = new char[];
char* fold_tmp = new char[];

struct _finddata_t c_file;
int hFile;

struct _finddata_t c_file1;
int hFile1;




void get_input_and_target(int num_sample)
{
	IplImage* src;
	strcpy(path,tmp);					
	path = strcat(path,fold);
	path = strcat(path,"\\");
	path = strcat(path,file);
	
	//printf("%s\n",path);

	src = cvLoadImage(path,CV_LOAD_IMAGE_GRAYSCALE);
	threshold(src, 200);
	change_to_binary(src);
	IplImage* cut_img;
	cut_img = cut_image(src);
	

	IplImage* scale = cvCreateImage(cvSize(15,21),src->depth,src->nChannels);
	scaling(cut_img,scale);
	
	
	IplImage* des = cvCreateImage(cvSize(15,21),src->depth,src->nChannels);
	copy_img(scale,des);
	cvReleaseImage(&scale);
	thinning(des);
	find_starting_point(des);
	normalize(des);

	//init
	float lr[2][21][3],rl[2][21][3],tb[2][15][3],bt[2][15][3];
	float nlr[2][7][3],nrl[2][7][3],ntb[2][5][3],nbt[2][5][3];
		for (int i=0;i<2;i++)
		{
			for (int j=0;j<21;j++)
			{
				for (int k=0;k<3;k++)
				{
					lr[i][j][k] = 0;
					rl[i][j][k] = 0;
				}
			}
			for (int j=0;j<15;j++)
			{
				for (int k=0;k<3;k++)
				{
					tb[i][j][k] = 0;
					bt[i][j][k] = 0;
				}
			}
			for (int j=0;j<7;j++)
			{
				for (int k=0;k<3;k++)
				{
					nlr[i][j][k] = 0;
					nrl[i][j][k] = 0;
				}
			}
			for (int j=0;j<5;j++)
			{
				for (int k=0;k<3;k++)
				{
					ntb[i][j][k] = 0;
					nbt[i][j][k] = 0;
				}
			}
		}

	//get vector
	get_feature_extraction(des,lr,rl,tb,bt);
	normalize_feature_extraction(des,lr,rl,tb,bt,nlr,nrl,ntb,nbt);

	//get in and target
	int n = 0;
	for (int i=0;i<2;i++)
	{
		for (int j=0;j<7;j++)
			for (int k=0;k<3;k++)
			{
				in[n] = nlr[i][j][k];
				n++;
			}
		for (int j=0;j<7;j++)
			for (int k=0;k<3;k++)
			{
				in[n] = nrl[i][j][k];
				n++;
			}
		for (int j=0;j<5;j++)
			for (int k=0;k<3;k++)
			{
				in[n] = ntb[i][j][k];
				n++;
			}
		for (int j=0;j<5;j++)
			for (int k=0;k<3;k++)
			{
				in[n] = nbt[i][j][k];
				n++;
			}
	}

	for (int i=0;i<Output;i++)
	{
		target[i] = 0.1000000;
	}
	target[num_sample] = 0.9000000;
	
	cvReleaseImage(&src);
	cvReleaseImage(&cut_img);
	cvReleaseImage(&des);
}
void training()
{
	double sig_out[Output];
	double sig_hid[Hidden];
	const double step = 0.2;
	int num_of_sam = 0;

	for (int number_of_interation = 0;number_of_interation < 10000;number_of_interation++)	//iteration = 100
	{	
		if (number_of_interation%100 == 0) printf("%d%\n",number_of_interation/100);
		hFile = _findfirst( "C:\\sample 1\\*.", &c_file );
		_findnext(hFile,&c_file);		//loai bo file .
		_findnext(hFile,&c_file);		//loai bo file ..
		do
		{
			strcpy(fold, c_file.name);
			strcpy(fold_tmp, tmp);
			fold_tmp = strcat(fold_tmp, c_file.name);
			fold_tmp = strcat(fold_tmp, "\\*.*");
			hFile1 = _findfirst(fold_tmp, &c_file1);
			_findnext(hFile1,&c_file1);
			_findnext(hFile1, &c_file1);
			num_of_sam = 0;
			do
			{
				if (strcmp(c_file1.name,"Thumbs.db") == 0) _findnext(hFile1, &c_file1);
				strcpy(file, c_file1.name);
				// do training ******************************
				//get_input_and_target(num_of_sam);
				
				//get input and target
				get_input_and_target(num_of_sam);

				//calculate value on each node
				for (int i=0;i<Hidden;i++)
				{
					hid[i] = 0;
					for (int j=0;j<Input;j++)
					{
						hid[i] += w_in[i][j]*in[j];
					}
					hid[i] = 1 / (1 + exp(-hid[i]));
				}
				for (int i=0;i<Output;i++)
				{
					out[i] = 0;
					for (int j=0;j<Hidden;j++)
					{
						out[i] += w_hid[i][j]*hid[j];
					}
					out[i] = 1 / (1 + exp(-out[i]));
				}
				//propagation
				for (int i=0;i<Output;i++)
				{
					sig_out[i] = out[i]*(1-out[i])*(target[i]-out[i]);
				}
				for (int i=0;i<Hidden;i++)
				{
					double sum = 0;
					for (int j=0;j<Output;j++)
					{
						sum += w_hid[j][i]*sig_out[j];
					}
					sig_hid[i] = hid[i]*(1-hid[i])*sum;
				}
				for (int i=0;i<Hidden;i++)
				{
					for (int j=0;j<Output;j++)
					{
						w_hid[j][i] += step*sig_out[j]*hid[i]; 
					}
				}
				for (int i=0;i<Input;i++)
				{
					for (int j=0;j<Hidden;j++)
					{
						w_in[j][i] += step*sig_hid[j]*in[i]; 
					}
				}
				num_of_sam++;
			}
			while (_findnext(hFile1, &c_file1) == 0);
			_findclose(hFile1);
		}
		while (_findnext( hFile, &c_file ) == 0);
		_findclose(hFile);
	}
}
void Write_weight()
{
	String^ fileName = "C:\\weight_file5.txt";
	StreamWriter^ sw = gcnew StreamWriter(fileName);
	sw->WriteLine("This is INPUT WEIGHT");
	for (int i=0;i<Input;i++)
	{
		for (int j=0;j<Hidden;j++)
		{
			sw->WriteLine(Convert::ToString(w_in[j][i]));
		}
	}
	sw->WriteLine("This is HIDDEN WEIGHT");
	for (int i=0;i<Hidden;i++)
	{
		for (int j=0;j<Output;j++)
		{
			sw->WriteLine(Convert::ToString(w_hid[j][i]));
		}
	}
	sw->Close();
}
void Read_weight()
{
	String^ fileName = "weight_file9.txt";
	StreamReader^ din = File::OpenText(fileName);
	System::String^ str;
	str = din->ReadLine();
	for (int i=0;i<Input;i++)
	{
		for (int j=0;j<Hidden;j++)
		{
			str = din->ReadLine();
			w_in[j][i] = Convert::ToDouble(str);
		}
	}
	str = din->ReadLine();
	for (int i=0;i<Hidden;i++)
	{
		for (int j=0;j<Output;j++)
		{
			str = din->ReadLine();
			w_hid[j][i] = Convert::ToDouble(str);
		}
	}
	din->Close();
}
int recognise(char* file_name)
{
	IplImage* src = cvLoadImage(file_name,CV_LOAD_IMAGE_GRAYSCALE);
	threshold(src, 200);
	change_to_binary(src);
	IplImage*cut_img;
	cut_img = cut_image(src);
	IplImage* scale = cvCreateImage(cvSize(15,21),src->depth,src->nChannels);
	scaling(cut_img,scale);
	IplImage* des = cvCreateImage(cvSize(15,21),src->depth,src->nChannels);
	copy_img(scale,des);
	cvReleaseImage(&scale);
	thinning(des);
	find_starting_point(des);
	normalize(des);
	print_img(des);
	
	double input[Input];
	double output[Output];
	//init
	float lr[2][21][3],rl[2][21][3],tb[2][15][3],bt[2][15][3];
	float nlr[2][7][3],nrl[2][7][3],ntb[2][5][3],nbt[2][5][3];
		for (int i=0;i<2;i++)
		{
			for (int j=0;j<21;j++)
			{
				for (int k=0;k<3;k++)
				{
					lr[i][j][k] = 0;
					rl[i][j][k] = 0;
				}
			}
			for (int j=0;j<15;j++)
			{
				for (int k=0;k<3;k++)
				{
					tb[i][j][k] = 0;
					bt[i][j][k] = 0;
				}
			}
			for (int j=0;j<7;j++)
			{
				for (int k=0;k<3;k++)
				{
					nlr[i][j][k] = 0;
					nrl[i][j][k] = 0;
				}
			}
			for (int j=0;j<5;j++)
			{
				for (int k=0;k<3;k++)
				{
					ntb[i][j][k] = 0;
					nbt[i][j][k] = 0;
				}
			}
		}

	//get vector
	get_feature_extraction(des,lr,rl,tb,bt);
	normalize_feature_extraction(des,lr,rl,tb,bt,nlr,nrl,ntb,nbt);

	//get in and target
	int n = 0;
	for (int i=0;i<2;i++)
	{
		for (int j=0;j<7;j++)
			for (int k=0;k<3;k++)
			{
				input[n] = nlr[i][j][k];
				n++;
			}
		for (int j=0;j<7;j++)
			for (int k=0;k<3;k++)
			{
				input[n] = nrl[i][j][k];
				n++;
			}
		for (int j=0;j<5;j++)
			for (int k=0;k<3;k++)
			{
				input[n] = ntb[i][j][k];
				n++;
			}
		for (int j=0;j<5;j++)
			for (int k=0;k<3;k++)
			{
				input[n] = nbt[i][j][k];
				n++;
			}
	}
	cvReleaseImage(&src);
	cvReleaseImage(&cut_img);
	
	cvReleaseImage(&des);
	
	for (int i=0;i<Hidden;i++)
	{
		hid[i] = 0;
		for (int j=0;j<Input;j++)
		{
			hid[i] += w_in[i][j]*input[j];
		}
		hid[i] = 1 / (1 + exp(-hid[i]));
	}
	printf("Day la ket wa\n");
	for (int i=0;i<Output;i++)
	{
		output[i] = 0;
		for (int j=0;j<Hidden;j++)
		{
			output[i] += w_hid[i][j]*hid[j];
		}
		output[i] = 1 / (1 + exp(-output[i]));
		Console::Write(Convert::ToChar(97+i));
		printf(" : ");
		printf("%f\n",output[i]);
	}
	double max = 0;
	int k;
	for (int i = 0;i<26;i++)
	{
		if (output[i] > max) 
		{
			max = output[i];
			k = i;
		}
	}
	//printf("day la chu : ");
	//Console::WriteLine(Convert::ToChar(97+k));
	return k+1;
}

