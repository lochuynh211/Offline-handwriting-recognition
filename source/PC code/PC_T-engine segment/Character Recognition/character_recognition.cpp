#include "character_recognition.h"

int numInputNodeGroup1;		
int numOutputNodeGroup1;
int numHiddenNodeGroup1;
double* inputGroup1;
double* outputGroup1;
double* hiddenOutGroup1;
double* weightInGroup1;
double* weightHidGroup1;

int numInputNodeGroup2;		
int numOutputNodeGroup2;
int numHiddenNodeGroup2;
double* inputGroup2;
double* outputGroup2;
double* hiddenOutGroup2;
double* weightInGroup2;
double* weightHidGroup2;

int numInputNodeGroup3;		
int numOutputNodeGroup3;
int numHiddenNodeGroup3;
double* inputGroup3;
double* outputGroup3;
double* hiddenOutGroup3;
double* weightInGroup3;
double* weightHidGroup3;

int numInputNodeGroup4;		
int numOutputNodeGroup4;
int numHiddenNodeGroup4;
double* inputGroup4;
double* outputGroup4;
double* hiddenOutGroup4;
double* weightInGroup4;
double* weightHidGroup4;

int numInputNodeGroup5;		
int numOutputNodeGroup5;
int numHiddenNodeGroup5;
double* inputGroup5;
double* outputGroup5;
double* hiddenOutGroup5;
double* weightInGroup5;
double* weightHidGroup5;

int numInputNodeGroup6;		
int numOutputNodeGroup6;
int numHiddenNodeGroup6;
double* inputGroup6;
double* outputGroup6;
double* hiddenOutGroup6;
double* weightInGroup6;
double* weightHidGroup6;

int numInputNodeGroup7;		
int numOutputNodeGroup7;
int numHiddenNodeGroup7;
double* inputGroup7;
double* outputGroup7;
double* hiddenOutGroup7;
double* weightInGroup7;
double* weightHidGroup7;

int numInputNodeGroup8;		
int numOutputNodeGroup8;
int numHiddenNodeGroup8;
double* inputGroup8;
double* outputGroup8;
double* hiddenOutGroup8;
double* weightInGroup8;
double* weightHidGroup8;

int numInputNodeGroup9;		
int numOutputNodeGroup9;
int numHiddenNodeGroup9;
double* inputGroup9;
double* outputGroup9;
double* hiddenOutGroup9;
double* weightInGroup9;
double* weightHidGroup9;

int numInputNodeGroup10;		
int numOutputNodeGroup10;
int numHiddenNodeGroup10;
double* inputGroup10;
double* outputGroup10;
double* hiddenOutGroup10;
double* weightInGroup10;
double* weightHidGroup10;



void initAll()
{

	//init group 1
	numInputNodeGroup1 = NUM_INPUT_GROUP1;		
	numOutputNodeGroup1 = NUM_OUTPUT_GROUP1;
	numHiddenNodeGroup1 = NUM_HIDDEN_GROUP1;
	inputGroup1 = (double*)malloc(numInputNodeGroup1 * sizeof(double));			//al of sample save here
	outputGroup1 = (double*)malloc(numOutputNodeGroup1 * sizeof(double));			
	hiddenOutGroup1 = (double*)malloc(numHiddenNodeGroup1 * sizeof(double));
	weightInGroup1 = (double*)malloc(numHiddenNodeGroup1 * numInputNodeGroup1 * sizeof(double));		//[hidden][input]
	weightHidGroup1 = (double*)malloc(numOutputNodeGroup1 * numHiddenNodeGroup1 * sizeof(double));		//[ouput][hidden]
	//ifstream* inFile1 = new ifstream("E:\\Training Sample\\networkGroup\\NetGroup1.dat", std::ios::in|std::ios::binary);
	//inFile1->read((char*)(weightInGroup1), numInputNodeGroup1 * numHiddenNodeGroup1 * sizeof(double));
	//inFile1->read((char*)(weightHidGroup1), numHiddenNodeGroup1 * numOutputNodeGroup1 * sizeof(double));
	//sinFile1->close();
	FILE *fp;
	if((fp=fopen("E:\\Training Sample\\networkGroup\\NetGroup1.dat", "rb"))==NULL) {
      printf("Cannot open file.\n");
    }
	//E:\\Training Sample\\networkGroup\\NetGroup1.dat

	fread((weightInGroup1), sizeof(double), numInputNodeGroup1 * numHiddenNodeGroup1, fp);
	fread((weightHidGroup1), sizeof(double), numHiddenNodeGroup1 * numOutputNodeGroup1, fp);
	fclose(fp);
	///////
	
	printf("gia tri cua weightInGroup1[0] = %f \n",weightInGroup1[0]);
	printf("gia tri cua weightInGroup1[0] = %f \n",weightInGroup1[numHiddenNodeGroup1 * numInputNodeGroup1 - 1]);
	printf("gia tri cua weightHidGroup1[0] = %f \n",weightHidGroup1[0]);
	printf("gia tri cua weightHidGroup1[0] = %f \n",weightHidGroup1[numOutputNodeGroup1 * numHiddenNodeGroup1 - 1]);

	///////
	
	//init group 2
	numInputNodeGroup2 = NUM_INPUT_GROUP2;		
	numOutputNodeGroup2 = NUM_OUTPUT_GROUP2;
	numHiddenNodeGroup2 = NUM_HIDDEN_GROUP2;
	inputGroup2 = (double*)malloc(numInputNodeGroup2 * sizeof(double));			//al of sample save here
	outputGroup2 = (double*)malloc(numOutputNodeGroup2 * sizeof(double));			
	hiddenOutGroup2 = (double*)malloc(numHiddenNodeGroup2 * sizeof(double));
	weightInGroup2 = (double*)malloc(numHiddenNodeGroup2 * numInputNodeGroup2 * sizeof(double));		//[hidden][input]
	weightHidGroup2 = (double*)malloc(numOutputNodeGroup2 * numHiddenNodeGroup2 * sizeof(double));		//[ouput][hidden]
	
	//FILE *fp;
	if((fp=fopen("E:\\Training Sample\\networkGroup\\NetGroup2.dat", "rb"))==NULL) {
      printf("Cannot open file.\n");
    }
	
	fread((weightInGroup2), sizeof(double), numInputNodeGroup2 * numHiddenNodeGroup2, fp);
	fread((weightHidGroup2), sizeof(double), numHiddenNodeGroup2 * numOutputNodeGroup2, fp);
	fclose(fp);

	//init group 3
	numInputNodeGroup3 = NUM_INPUT_GROUP3;		
	numOutputNodeGroup3 = NUM_OUTPUT_GROUP3;
	numHiddenNodeGroup3 = NUM_HIDDEN_GROUP3;
	inputGroup3 = (double*)malloc(numInputNodeGroup3 * sizeof(double));			//al of sample save here
	outputGroup3 = (double*)malloc(numOutputNodeGroup3 * sizeof(double));			
	hiddenOutGroup3 = (double*)malloc(numHiddenNodeGroup3 * sizeof(double));
	weightInGroup3 = (double*)malloc(numHiddenNodeGroup3 * numInputNodeGroup3 * sizeof(double));		//[hidden][input]
	weightHidGroup3 = (double*)malloc(numOutputNodeGroup3 * numHiddenNodeGroup3 * sizeof(double));		//[ouput][hidden]
	
	if((fp=fopen("E:\\Training Sample\\networkGroup\\NetGroup3.dat", "rb"))==NULL) {
      printf("Cannot open file.\n");
    }
	
	fread((weightInGroup3), sizeof(double), numInputNodeGroup3 * numHiddenNodeGroup3, fp);
	fread((weightHidGroup3), sizeof(double), numHiddenNodeGroup3 * numOutputNodeGroup3, fp);
	fclose(fp);

	
	//init group 4

	numInputNodeGroup4 = NUM_INPUT_GROUP4;		
	numOutputNodeGroup4 = NUM_OUTPUT_GROUP4;
	numHiddenNodeGroup4 = NUM_HIDDEN_GROUP4;
	inputGroup4 = (double*)malloc(numInputNodeGroup4 * sizeof(double));			//al of sample save here
	outputGroup4 = (double*)malloc(numOutputNodeGroup4 * sizeof(double));			
	hiddenOutGroup4 = (double*)malloc(numHiddenNodeGroup4 * sizeof(double));
	weightInGroup4 = (double*)malloc(numHiddenNodeGroup4 * numInputNodeGroup4 * sizeof(double));		//[hidden][input]
	weightHidGroup4 = (double*)malloc(numOutputNodeGroup4 * numHiddenNodeGroup4 * sizeof(double));		//[ouput][hidden]
	
	if((fp=fopen("E:\\Training Sample\\networkGroup\\NetGroup4.dat", "rb"))==NULL) {
      printf("Cannot open file.\n");
    }
	
	fread((weightInGroup4), sizeof(double), numInputNodeGroup4 * numHiddenNodeGroup4, fp);
	fread((weightHidGroup4), sizeof(double), numHiddenNodeGroup4 * numOutputNodeGroup4, fp);
	fclose(fp);


	
	//init group 5
	numInputNodeGroup5 = NUM_INPUT_GROUP5;		
	numOutputNodeGroup5 = NUM_OUTPUT_GROUP5;
	numHiddenNodeGroup5 = NUM_HIDDEN_GROUP5;
	inputGroup5 = (double*)malloc(numInputNodeGroup5 * sizeof(double));			//al of sample save here
	outputGroup5 = (double*)malloc(numOutputNodeGroup5 * sizeof(double));			
	hiddenOutGroup5 = (double*)malloc(numHiddenNodeGroup5 * sizeof(double));
	weightInGroup5 = (double*)malloc(numHiddenNodeGroup5 * numInputNodeGroup5 * sizeof(double));		//[hidden][input]
	weightHidGroup5 = (double*)malloc(numOutputNodeGroup5 * numHiddenNodeGroup5 * sizeof(double));		//[ouput][hidden]
	
	if((fp=fopen("E:\\Training Sample\\networkGroup\\NetGroup5.dat", "rb"))==NULL) {
      printf("Cannot open file.\n");
    }
	
	fread((weightInGroup5), sizeof(double), numInputNodeGroup5 * numHiddenNodeGroup5, fp);
	fread((weightHidGroup5), sizeof(double), numHiddenNodeGroup5 * numOutputNodeGroup5, fp);
	fclose(fp);

	
	//init group 6
	numInputNodeGroup6 = NUM_INPUT_GROUP6;		
	numOutputNodeGroup6 = NUM_OUTPUT_GROUP6;
	numHiddenNodeGroup6 = NUM_HIDDEN_GROUP6;
	inputGroup6 = (double*)malloc(numInputNodeGroup6 * sizeof(double));			//al of sample save here
	outputGroup6 = (double*)malloc(numOutputNodeGroup6 * sizeof(double));			
	hiddenOutGroup6 = (double*)malloc(numHiddenNodeGroup6 * sizeof(double));
	weightInGroup6 = (double*)malloc(numHiddenNodeGroup6 * numInputNodeGroup6 * sizeof(double));		//[hidden][input]
	weightHidGroup6 = (double*)malloc(numOutputNodeGroup6 * numHiddenNodeGroup6 * sizeof(double));		//[ouput][hidden]
	
	if((fp=fopen("E:\\Training Sample\\networkGroup\\NetGroup6.dat", "rb"))==NULL) {
      printf("Cannot open file.\n");
    }
	
	fread((weightInGroup6), sizeof(double), numInputNodeGroup6 * numHiddenNodeGroup6, fp);
	fread((weightHidGroup6), sizeof(double), numHiddenNodeGroup6 * numOutputNodeGroup6, fp);
	fclose(fp);
	
	//init group 7
	numInputNodeGroup7 = NUM_INPUT_GROUP7;		
	numOutputNodeGroup7 = NUM_OUTPUT_GROUP7;
	numHiddenNodeGroup7 = NUM_HIDDEN_GROUP7;
	inputGroup7 = (double*)malloc(numInputNodeGroup7 * sizeof(double));			//al of sample save here
	outputGroup7 = (double*)malloc(numOutputNodeGroup7 * sizeof(double));			
	hiddenOutGroup7 = (double*)malloc(numHiddenNodeGroup7 * sizeof(double));
	weightInGroup7 = (double*)malloc(numHiddenNodeGroup7 * numInputNodeGroup7 * sizeof(double));		//[hidden][input]
	weightHidGroup7 = (double*)malloc(numOutputNodeGroup7 * numHiddenNodeGroup7 * sizeof(double));		//[ouput][hidden]
	
	if((fp=fopen("E:\\Training Sample\\networkGroup\\NetGroup7.dat", "rb"))==NULL) {
      printf("Cannot open file.\n");
    }
	
	fread((weightInGroup7), sizeof(double), numInputNodeGroup7 * numHiddenNodeGroup7, fp);
	fread((weightHidGroup7), sizeof(double), numHiddenNodeGroup7 * numOutputNodeGroup7, fp);
	fclose(fp);

	//init group 8
	numInputNodeGroup8 = NUM_INPUT_GROUP8;		
	numOutputNodeGroup8 = NUM_OUTPUT_GROUP8_REAL;
	numHiddenNodeGroup8 = NUM_HIDDEN_GROUP8;
	inputGroup8 = (double*)malloc(numInputNodeGroup8 * sizeof(double));			//al of sample save here
	outputGroup8 = (double*)malloc(numOutputNodeGroup8 * sizeof(double));			
	hiddenOutGroup8 = (double*)malloc(numHiddenNodeGroup8 * sizeof(double));
	weightInGroup8 = (double*)malloc(numHiddenNodeGroup8 * numInputNodeGroup8 * sizeof(double));		//[hidden][input]
	weightHidGroup8 = (double*)malloc(numOutputNodeGroup8 * numHiddenNodeGroup8 * sizeof(double));		//[ouput][hidden]
	
	if((fp=fopen("E:\\Training Sample\\networkGroup\\NetGroup8.dat", "rb"))==NULL) {
      printf("Cannot open file.\n");
    }
	
	fread((weightInGroup8), sizeof(double), numInputNodeGroup8 * numHiddenNodeGroup8, fp);
	fread((weightHidGroup8), sizeof(double), numHiddenNodeGroup8 * numOutputNodeGroup8, fp);
	fclose(fp);

	//init group 9
	numInputNodeGroup9 = NUM_INPUT_GROUP9;		
	numOutputNodeGroup9 = NUM_OUTPUT_GROUP9;
	numHiddenNodeGroup9 = NUM_HIDDEN_GROUP9;
	inputGroup9 = (double*)malloc(numInputNodeGroup9 * sizeof(double));			//al of sample save here
	outputGroup9 = (double*)malloc(numOutputNodeGroup9 * sizeof(double));			
	hiddenOutGroup9 = (double*)malloc(numHiddenNodeGroup9 * sizeof(double));
	weightInGroup9 = (double*)malloc(numHiddenNodeGroup9 * numInputNodeGroup9 * sizeof(double));		//[hidden][input]
	weightHidGroup9 = (double*)malloc(numOutputNodeGroup9 * numHiddenNodeGroup9 * sizeof(double));		//[ouput][hidden]
	
	if((fp=fopen("E:\\Training Sample\\networkGroup\\NetGroup9.dat", "rb"))==NULL) {
      printf("Cannot open file.\n");
    }
	
	fread((weightInGroup9), sizeof(double), numInputNodeGroup9 * numHiddenNodeGroup9, fp);
	fread((weightHidGroup9), sizeof(double), numHiddenNodeGroup9 * numOutputNodeGroup9, fp);
	fclose(fp);

	//init group 10
	numInputNodeGroup10 = NUM_INPUT_GROUP10;		
	numOutputNodeGroup10 = NUM_OUTPUT_GROUP10;
	numHiddenNodeGroup10 = NUM_HIDDEN_GROUP10;
	inputGroup10 = (double*)malloc(numInputNodeGroup10 * sizeof(double));			//al of sample save here
	outputGroup10 = (double*)malloc(numOutputNodeGroup10 * sizeof(double));			
	hiddenOutGroup10 = (double*)malloc(numHiddenNodeGroup10 * sizeof(double));
	weightInGroup10 = (double*)malloc(numHiddenNodeGroup10 * numInputNodeGroup10 * sizeof(double));		//[hidden][input]
	weightHidGroup10 = (double*)malloc(numOutputNodeGroup10 * numHiddenNodeGroup10 * sizeof(double));		//[ouput][hidden]
	
	if((fp=fopen("E:\\Training Sample\\networkGroup\\NetGroup10.dat", "rb"))==NULL) {
      printf("Cannot open file.\n");
    }
	
	fread((weightInGroup10), sizeof(double), numInputNodeGroup10 * numHiddenNodeGroup10, fp);
	fread((weightHidGroup10), sizeof(double), numHiddenNodeGroup10 * numOutputNodeGroup10, fp);
	fclose(fp);
}
structCharacter characterRecognitionGroup1(Image* inImage)
{
	Image* preProcImage = (Image*)malloc(sizeof(Image));
	preProcessAll(inImage, preProcImage, 1);
	//printImage(preProcImage);
	getFeatureExtraction(preProcImage, inputGroup1, 1);
	free(preProcImage);
	int i,j;
	for( i = 0; i < numHiddenNodeGroup1; i++)
	{
		hiddenOutGroup1[i] = 0;
		for( j = 0; j < numInputNodeGroup1; j++)
		{
			hiddenOutGroup1[i] += weightInGroup1[i*numInputNodeGroup1 + j] * inputGroup1[j];
		}
		hiddenOutGroup1[i] = 1 / (1 + exp(-hiddenOutGroup1[i]));
	}
	for( i = 0; i < numOutputNodeGroup1; i++)
	{
		outputGroup1[i] = 0;
		for( j = 0; j < numHiddenNodeGroup1; j++)
		{
			outputGroup1[i] += weightHidGroup1[i*numHiddenNodeGroup1 + j] * hiddenOutGroup1[j];
		}
		outputGroup1[i] = 1 / (1 + exp(-outputGroup1[i]));
		cout<<convertLocationGroup1(i)<<"  :  "<<outputGroup1[i]<<"\n";
	}
	double max = 0;
	int k;

	//tinh toan output de ra chu~, tim gia tri index lon nhat trong cac output
	for( i = 0; i < numOutputNodeGroup1; i++)
	{
		if (outputGroup1[i] > max) 
		{
			max = outputGroup1[i];
			k = i;
		}
	}
	structCharacter result;
	result.character = convertLocationGroup1(k);
	result.maxValue = max;
	cout<<"this is the reuslt : "<<result.character<<"   :   "<<max<<"\n";
	return result;
}
structCharacter characterRecognitionGroup2(Image* inImage)
{
	int i,j;
	Image* preProcImage = (Image*)malloc(sizeof(Image));
	preProcessAll(inImage, preProcImage, 2);
	getFeatureExtraction(preProcImage, inputGroup2, 2);
	free(preProcImage);
	
	for( i = 0; i < numHiddenNodeGroup2; i++)
	{
		hiddenOutGroup2[i] = 0;
		for( j = 0; j < numInputNodeGroup2; j++)
		{
			hiddenOutGroup2[i] += weightInGroup2[i*numInputNodeGroup2 + j] * inputGroup2[j];
		}
		hiddenOutGroup2[i] = 1 / (1 + exp(-hiddenOutGroup2[i]));
	}
	for( i = 0; i < numOutputNodeGroup2; i++)
	{
		outputGroup2[i] = 0;
		for( j = 0; j < numHiddenNodeGroup2; j++)
		{
			outputGroup2[i] += weightHidGroup2[i*numHiddenNodeGroup2 + j] * hiddenOutGroup2[j];
		}
		outputGroup2[i] = 1 / (1 + exp(-outputGroup2[i]));
		cout<<convertLocationGroup2(i)<<"  :  "<<outputGroup2[i]<<"\n";
	}
	double max = 0;
	int k;

	//tinh toan output de ra chu~, tim gia tri index lon nhat trong cac output
	for( i = 0; i < numOutputNodeGroup2; i++)
	{
		if (outputGroup2[i] > max) 
		{
			max = outputGroup2[i];
			k = i;
		}
	}
	structCharacter result;
	result.character = convertLocationGroup2(k);
	result.maxValue = max;
	cout<<"this is the reuslt : "<<result.character<<"   :   "<<max<<"\n";
	return result;
}
structCharacter characterRecognitionGroup3(Image* inImage)
{
	int i,j;
	Image* preProcImage = (Image*)malloc(sizeof(Image));
	preProcessAll(inImage, preProcImage, 3);
	getFeatureExtraction(preProcImage, inputGroup3, 3);
	free(preProcImage);
	
	for( i = 0; i < numHiddenNodeGroup3; i++)
	{
		hiddenOutGroup3[i] = 0;
		for( j = 0; j < numInputNodeGroup3; j++)
		{
			hiddenOutGroup3[i] += weightInGroup3[i*numInputNodeGroup3 + j] * inputGroup3[j];
		}
		hiddenOutGroup3[i] = 1 / (1 + exp(-hiddenOutGroup3[i]));
	}
	for( i = 0; i < numOutputNodeGroup3; i++)
	{
		outputGroup3[i] = 0;
		for( j = 0; j < numHiddenNodeGroup3; j++)
		{
			outputGroup3[i] += weightHidGroup3[i*numHiddenNodeGroup3 + j] * hiddenOutGroup3[j];
		}
		outputGroup3[i] = 1 / (1 + exp(-outputGroup3[i]));
		cout<<convertLocationGroup3(i)<<"  :  "<<outputGroup3[i]<<"\n";
	}
	double max = 0;
	int k;

	//tinh toan output de ra chu~, tim gia tri index lon nhat trong cac output
	for( i = 0; i < numOutputNodeGroup3; i++)
	{
		if (outputGroup3[i] > max) 
		{
			max = outputGroup3[i];
			k = i;
		}
	}
	structCharacter result;
	result.character = convertLocationGroup3(k);
	result.maxValue = max;
	cout<<"this is the reuslt : "<<result.character<<"   :   "<<max<<"\n";
	return result;
}
structCharacter characterRecognitionGroup4(Image* inImage)
{
	int i,j;
	Image* preProcImage = (Image*)malloc(sizeof(Image));
	preProcessAll(inImage, preProcImage, 4);
	getFeatureExtraction(preProcImage, inputGroup4, 4);
	free(preProcImage);
	
	for( i = 0; i < numHiddenNodeGroup4; i++)
	{
		hiddenOutGroup4[i] = 0;
		for( j = 0; j < numInputNodeGroup4; j++)
		{
			hiddenOutGroup4[i] += weightInGroup4[i*numInputNodeGroup4 + j] * inputGroup4[j];
		}
		hiddenOutGroup4[i] = 1 / (1 + exp(-hiddenOutGroup4[i]));
	}
	for( i = 0; i < numOutputNodeGroup4; i++)
	{
		outputGroup4[i] = 0;
		for( j = 0; j < numHiddenNodeGroup4; j++)
		{
			outputGroup4[i] += weightHidGroup4[i*numHiddenNodeGroup4 + j] * hiddenOutGroup4[j];
		}
		outputGroup4[i] = 1 / (1 + exp(-outputGroup4[i]));
		cout<<convertLocationGroup4(i)<<"  :  "<<outputGroup4[i]<<"\n";
	}
	double max = 0;
	int k;

	//tinh toan output de ra chu~, tim gia tri index lon nhat trong cac output
	for( i = 0; i < numOutputNodeGroup4; i++)
	{
		if (outputGroup4[i] > max) 
		{
			max = outputGroup4[i];
			k = i;
		}
	}
	structCharacter result;
	result.character = convertLocationGroup4(k);
	result.maxValue = max;
	cout<<"this is the reuslt : "<<result.character<<"   :   "<<max<<"\n";
	return result;
}
structCharacter characterRecognitionGroup5(Image* inImage)
{
	int i,j;
	Image* preProcImage = (Image*)malloc(sizeof(Image));
	preProcessAll(inImage, preProcImage, 5);
	getFeatureExtraction(preProcImage, inputGroup5, 5);
	free(preProcImage);
	
	for( i = 0; i < numHiddenNodeGroup5; i++)
	{
		hiddenOutGroup5[i] = 0;
		for( j = 0; j < numInputNodeGroup5; j++)
		{
			hiddenOutGroup5[i] += weightInGroup5[i*numInputNodeGroup5 + j] * inputGroup5[j];
		}
		hiddenOutGroup5[i] = 1 / (1 + exp(-hiddenOutGroup5[i]));
	}
	for( i = 0; i < numOutputNodeGroup5; i++)
	{
		outputGroup5[i] = 0;
		for( j = 0; j < numHiddenNodeGroup5; j++)
		{
			outputGroup5[i] += weightHidGroup5[i*numHiddenNodeGroup5 + j] * hiddenOutGroup5[j];
		}
		outputGroup5[i] = 1 / (1 + exp(-outputGroup5[i]));
		cout<<convertLocationGroup5(i)<<"  :  "<<outputGroup5[i]<<"\n";
	}
	double max = 0;
	int k;

	//tinh toan output de ra chu~, tim gia tri index lon nhat trong cac output
	for( i = 0; i < numOutputNodeGroup5; i++)
	{
		if (outputGroup5[i] > max) 
		{
			max = outputGroup5[i];
			k = i;
		}
	}
	structCharacter result;
	result.character = convertLocationGroup5(k);
	result.maxValue = max;
	cout<<"this is the reuslt : "<<result.character<<"   :   "<<max<<"\n";
	return result;
}
structCharacter characterRecognitionGroup6(Image* inImage)
{
	int i,j;
	Image* preProcImage = (Image*)malloc(sizeof(Image));
	preProcessAll(inImage, preProcImage, 6);
	getFeatureExtraction(preProcImage, inputGroup6, 6);
	free(preProcImage);
	
	for( i = 0; i < numHiddenNodeGroup6; i++)
	{
		hiddenOutGroup6[i] = 0;
		for( j = 0; j < numInputNodeGroup6; j++)
		{
			hiddenOutGroup6[i] += weightInGroup6[i*numInputNodeGroup6 + j] * inputGroup6[j];
		}
		hiddenOutGroup6[i] = 1 / (1 + exp(-hiddenOutGroup6[i]));
	}
	for( i = 0; i < numOutputNodeGroup6; i++)
	{
		outputGroup6[i] = 0;
		for( j = 0; j < numHiddenNodeGroup6; j++)
		{
			outputGroup6[i] += weightHidGroup6[i*numHiddenNodeGroup6 + j] * hiddenOutGroup6[j];
		}
		outputGroup6[i] = 1 / (1 + exp(-outputGroup6[i]));
		cout<<convertLocationGroup6(i)<<"  :  "<<outputGroup6[i]<<"\n";
	}
	double max = 0;
	int k;

	//tinh toan output de ra chu~, tim gia tri index lon nhat trong cac output
	for( i = 0; i < numOutputNodeGroup6; i++)
	{
		if (outputGroup6[i] > max) 
		{
			max = outputGroup6[i];
			k = i;
		}
	}
	structCharacter result;
	result.character = convertLocationGroup6(k);
	result.maxValue = max;
	cout<<"this is the reuslt : "<<result.character<<"   :   "<<max<<"\n";
	return result;
}
structCharacter characterRecognitionGroup7(Image* inImage)
{
	int i,j;
	Image* preProcImage = (Image*)malloc(sizeof(Image));
	preProcessAll(inImage, preProcImage, 7);
	getFeatureExtraction(preProcImage, inputGroup7, 7);
	free(preProcImage);
	
	for( i = 0; i < numHiddenNodeGroup7; i++)
	{
		hiddenOutGroup7[i] = 0;
		for( j = 0; j < numInputNodeGroup7; j++)
		{
			hiddenOutGroup7[i] += weightInGroup7[i*numInputNodeGroup7 + j] * inputGroup7[j];
		}
		hiddenOutGroup7[i] = 1 / (1 + exp(-hiddenOutGroup7[i]));
	}
	for( i = 0; i < numOutputNodeGroup7; i++)
	{
		outputGroup7[i] = 0;
		for( j = 0; j < numHiddenNodeGroup7; j++)
		{
			outputGroup7[i] += weightHidGroup7[i*numHiddenNodeGroup7 + j] * hiddenOutGroup7[j];
		}
		outputGroup7[i] = 1 / (1 + exp(-outputGroup7[i]));
		cout<<convertLocationGroup7(i)<<"  :  "<<outputGroup7[i]<<"\n";
	}
	double max = 0;
	int k;

	//tinh toan output de ra chu~, tim gia tri index lon nhat trong cac output
	for( i = 0; i < numOutputNodeGroup7; i++)
	{
		if (outputGroup7[i] > max) 
		{
			max = outputGroup7[i];
			k = i;
		}
	}
	structCharacter result;
	result.character = convertLocationGroup7(k);
	result.maxValue = max;
	cout<<"this is the reuslt : "<<result.character<<"   :   "<<max<<"\n";
	return result;
}
structCharacter characterRecognitionGroup8(Image* inImage)
{
	int i,j;
	Image* preProcImage = (Image*)malloc(sizeof(Image));
	preProcessAll(inImage, preProcImage, 8);
	getFeatureExtraction(preProcImage, inputGroup8, 8);
	free(preProcImage);
	
	for( i = 0; i < numHiddenNodeGroup8; i++)
	{
		hiddenOutGroup8[i] = 0;
		for( j = 0; j < numInputNodeGroup8; j++)
		{
			hiddenOutGroup8[i] += weightInGroup8[i*numInputNodeGroup8 + j] * inputGroup8[j];
		}
		hiddenOutGroup8[i] = 1 / (1 + exp(-hiddenOutGroup8[i]));
	}
	for( i = 0; i < numOutputNodeGroup8; i++)
	{
		outputGroup8[i] = 0;
		for( j = 0; j < numHiddenNodeGroup8; j++)
		{
			outputGroup8[i] += weightHidGroup8[i*numHiddenNodeGroup8 + j] * hiddenOutGroup8[j];
		}
		outputGroup8[i] = 1 / (1 + exp(-outputGroup8[i]));
		cout<<convertLocationGroup8(i)<<"  :  "<<outputGroup8[i]<<"\n";
	}
	double max = 0;
	int k;

	//tinh toan output de ra chu~, tim gia tri index lon nhat trong cac output
	for( i = 0; i < numOutputNodeGroup8; i++)
	{
		if (outputGroup8[i] > max) 
		{
			max = outputGroup8[i];
			k = i;
		}
	}
	structCharacter result;
	result.character = convertLocationGroup8(k);
	result.maxValue = max;
	cout<<"this is the reuslt : "<<result.character<<"   :   "<<max<<"\n";
	return result;
}
structCharacter characterRecognitionGroup9(Image* inImage)
{
	int i,j;
	Image* preProcImage = (Image*)malloc(sizeof(Image));
	preProcessAll(inImage, preProcImage, 9);
	getFeatureExtraction(preProcImage, inputGroup9, 9);
	free(preProcImage);
	
	for( i = 0; i < numHiddenNodeGroup9; i++)
	{
		hiddenOutGroup9[i] = 0;
		for( j = 0; j < numInputNodeGroup9; j++)
		{
			hiddenOutGroup9[i] += weightInGroup9[i*numInputNodeGroup9 + j] * inputGroup9[j];
		}
		hiddenOutGroup9[i] = 1 / (1 + exp(-hiddenOutGroup9[i]));
	}
	for( i = 0; i < numOutputNodeGroup9; i++)
	{
		outputGroup9[i] = 0;
		for( j = 0; j < numHiddenNodeGroup9; j++)
		{
			outputGroup9[i] += weightHidGroup9[i*numHiddenNodeGroup9 + j] * hiddenOutGroup9[j];
		}
		outputGroup9[i] = 1 / (1 + exp(-outputGroup9[i]));
		cout<<convertLocationGroup9(i)<<"  :  "<<outputGroup9[i]<<"\n";
	}
	double max = 0;
	int k;

	//tinh toan output de ra chu~, tim gia tri index lon nhat trong cac output
	for( i = 0; i < numOutputNodeGroup9; i++)
	{
		if (outputGroup9[i] > max) 
		{
			max = outputGroup9[i];
			k = i;
		}
	}
	structCharacter result;
	result.character = convertLocationGroup9(k);
	result.maxValue = max;
	cout<<"this is the reuslt : "<<result.character<<"   :   "<<max<<"\n";
	return result;
}
structCharacter characterRecognitionGroup10(Image* inImage)
{
	int i,j;
	Image* preProcImage = (Image*)malloc(sizeof(Image));
	preProcessAll(inImage, preProcImage, 6);
	getFeatureExtraction(preProcImage, inputGroup10, 6);
	free(preProcImage);
	
	for( i = 0; i < numHiddenNodeGroup10; i++)
	{
		hiddenOutGroup10[i] = 0;
		for( j = 0; j < numInputNodeGroup10; j++)
		{
			hiddenOutGroup10[i] += weightInGroup10[i*numInputNodeGroup10 + j] * inputGroup10[j];
		}
		hiddenOutGroup10[i] = 1 / (1 + exp(-hiddenOutGroup10[i]));
	}
	for( i = 0; i < numOutputNodeGroup10; i++)
	{
		outputGroup10[i] = 0;
		for( j = 0; j < numHiddenNodeGroup10; j++)
		{
			outputGroup10[i] += weightHidGroup10[i*numHiddenNodeGroup10 + j] * hiddenOutGroup10[j];
		}
		outputGroup10[i] = 1 / (1 + exp(-outputGroup10[i]));
		cout<<convertLocationGroup10(i)<<"  :  "<<outputGroup10[i]<<"\n";
	}
	double max = 0;
	int k;

	//tinh toan output de ra chu~, tim gia tri index lon nhat trong cac output
	for( i = 0; i < numOutputNodeGroup10; i++)
	{
		if (outputGroup10[i] > max) 
		{
			max = outputGroup10[i];
			k = i;
		}
	}
	structCharacter result;
	result.character = convertLocationGroup10(k);
	result.maxValue = max;
	cout<<"this is the reuslt : "<<result.character<<"   :   "<<max<<"\n";
	return result;
}



char convertLocationGroup1(int location)
{
	switch(location)
	{
		case 0:
			return 'a';
		case 1:
			return 'c';
		case 2:
			return 'e';
		case 3:
			return 'i';
		case 4:
			return 'm';
		case 5:
			return 'n';
		case 6:
			return 'o';
		case 7:
			return 'r';
		case 8:
			return 's';
		case 9:
			return 'u';
		case 10:
			return 'v';
		case 11:
			return 'w';
		case 12:
			return 'x';
		case 13:
			return 'z';
	}
}
char convertLocationGroup2(int location)
{
	switch(location)
	{
		case 0:
			return 'g';
		case 1:
			return 'j';
		case 2:
			return 'p';
		case 3:
			return 'q';
		case 4:
			return 'y';
	}
}
char convertLocationGroup3(int location)
{
	switch(location)
	{
		case 0:
			return 'b';
		case 1:
			return 'd';
		case 2:
			return 'f';
		case 3:
			return 'h';
		case 4:
			return 'k';
		case 5:
			return 'l';
		case 6:
			return 't';
	}
}
char convertLocationGroup4(int location)
{
	return (char)(location + 'A');
}
char convertLocationGroup5(int location)
{
	return (char)(location + '0');
}
char convertLocationGroup6(int location)
{
	if (location < 26) return (char)(location + 'a');
	else return (char)(location - 26 + 'A');
}
char convertLocationGroup7(int location)
{
	return (char)(location + '0');
}
char convertLocationGroup8(int location)
{
	return (char)(location + '0');
}
char convertLocationGroup9(int location)
{
	switch(location)
	{
		case 0:
			return 'b';
		case 1:
			return 'd';
		case 2:
			return 'f';
		case 3:
			return 'h';
		case 4:
			return 'k';
		case 5:
			return 'l';
		case 6:
			return 't';
	}
	if (location >= 7 && location < 33) return (char)(location - 7 + 'A');
	else return (char)(location - 33 + '0');
}
char convertLocationGroup10(int location)
{
	return (char)(location + 'a');
}