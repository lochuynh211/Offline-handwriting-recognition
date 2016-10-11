#include "word_recognition.h"

//using for dictionary
char* dictionary;
int countOfDictionary;
int indexOfDictionary[26][27];
int headIndex;
int endIndex;
int countCharacter;
int countError;
char resultWord[MAX_WORD_LENGTH];
structError errorWord[20];
int ErrorCheck;

//using for word
Image* input;	
Image* thinningOfInput;

//using for baseline
int lowerBaseLine;
int upperBaseLine;
int middleRow;
int upCharacter;
int downCharacter;


//strokeWidth
int strokeWidth;

//using for segmentation
int averageCharacterWidth;
int* mVerticalDensity;

//using for artificial intelligent
seg_point_arr* leftSeg;
seg_point_arr* rightSeg;

//using for hole detection
Stack* leftStack;

char* wordRecognition(Image* inImage, int* countOfResultOfWord)
{
	char* word_result;
	input = (Image*)malloc(sizeof(Image));
	cutImage(inImage, input, 6);
	Image* tmp = (Image*)malloc(sizeof(Image));
	createImage(tmp, input->mHeight, input->mWidth);
	copyImage(input, tmp);
	thinningOfInput = (Image*)malloc(sizeof(Image));
	thinning(tmp, thinningOfInput);

	//init data
	int width = input->mWidth;
	int height = input->mHeight;
	averageCharacterWidth = 0;
	strokeWidth = 0;
	lowerBaseLine = -1;
	upperBaseLine = -1;

	//init mVerticalDensity
	mVerticalDensity = (int*)malloc(input->mWidth*sizeof(int));
	int sum = 0;
	int i, j;
	for ( j = 0; j < width; j++)
	{
		for ( i = 0; i < height; i++)
		{
			if (input->mData[i*width + j] == 1)
			{
				sum++;
			}
		}
		mVerticalDensity[j] = sum;
		sum = 0;
	}

	slantCorrection(); 
	calculateStrokeWidth();
	baseline();
	segment();
	
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, leftSeg);
	//showImage("left segment", tmp);
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, rightSeg);
	//showImage("right segment", tmp);
	
	//init dictionary
	countCharacter = 0;
	countError = 0;
	//resultWord = (char*)malloc(MAX_WORD_LENGTH); 
	headIndex = indexOfDictionary[convertCharacterToIndex1('a')][0];
	endIndex = countOfDictionary;
	ErrorCheck = 0;
	
	int check = 0;;
	if (leftSeg ->real_length == rightSeg->real_length)
	{
		check = recursiveValidation();
		if (check == 1)
		{
			(*countOfResultOfWord) = countCharacter;
			word_result = (char*) malloc(countCharacter*sizeof(char));
			printf("countOfResultOfWord = %d \n", (*countOfResultOfWord));
			printf("HERE IS THE RESULT  :");
			for( i = 0; i < countCharacter; i++)
			{
				word_result[i] = resultWord[i];
				printf("%c",resultWord[i]);
			}
			printf("\n");
			printf("ket qua tra ve \n");
			for( i = 0; i < (*countOfResultOfWord); i++)
			{
				printf("%c",word_result[i]);
			}
		}
		else
		{
			return NULL;
		}
		
	}
	else
	{
		return NULL;
	}	
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///              DICTIONARY CHECKING                    //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void createDictionary()
{
	/*
	ifstream* inFile = new ifstream("E:\\Training Sample\\Dictionary.dat", std::ios::in|std::ios::binary);
	ifstream* inFile2 = new ifstream("E:\\Training Sample\\IndexOfDictionary.dat", std::ios::in|std::ios::binary);
	inFile->read((char*)(&countOfDictionary), sizeof(int));
	dictionary = (char*)malloc(countOfDictionary);
	inFile->read(dictionary, countOfDictionary);
	inFile2->read((char*)(&indexOfDictionary), 26*27*sizeof(int));
	inFile->close();
	inFile2->close();
	*/
	
	
	FILE *fp, *fp2;
	//if((fp=fopen("E:\\Training Sample\\Dictionary.dat", "rb"))==NULL) {
	if((fp=fopen("/SYS/Dictionary.dat", "rb"))==NULL) {
      //printf("Cannot open file.\n");
    }
	//if((fp2=fopen("E:\\Training Sample\\IndexOfDictionary.dat", "rb"))==NULL) {
	if((fp2=fopen("/SYS/Dictionary.dat", "rb"))==NULL) {
      //printf("Cannot open file.\n");
    }
	fread((&countOfDictionary), sizeof(int), 1, fp);
	dictionary = (char*)malloc(countOfDictionary);
	fread((dictionary), sizeof(char), countOfDictionary, fp);
	fread((&indexOfDictionary), sizeof(int), 26*27, fp2);
	fclose(fp);
	fclose(fp2);
	

	/*
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
	if((fp=fopen("/SYS/NetGroup1.dat", "rb"))==NULL) {
      //printf("Cannot open file.\n");
    }
	
	fread((weightInGroup1), sizeof(double), numInputNodeGroup1 * numHiddenNodeGroup1, fp);
	fread((weightHidGroup1), sizeof(double), numHiddenNodeGroup1 * numOutputNodeGroup1, fp);
	fclose(fp);
	*/
	//cout<<"Dictionary is ready\n";
	//cout<<"Dictionary is ready\n";
}

int convertCharacterToIndex1(char cha)
{
	if(cha >= 'A' && cha <='Z') return cha - 'A';
	else return cha - 'a';
}

int convertCharacterToIndex2(char cha)
{
	if(cha >= 'A' && cha <='Z') return cha - 'A' + 1;
	else if (cha >= 'a' && cha <='z') return cha - 'a' + 1;
	else return 0;
}

int checkingOneCharacter(char character)
{
	if (countError > 0)
	{
		int tempHeadIndex = headIndex;
		int i;
		for (i = headIndex; i <= endIndex; i++)
		{
			if (dictionary[i] == 0)
			{
				if ((i - tempHeadIndex) <= countCharacter)
				{
					tempHeadIndex = i + 1;
					continue;
				}
				else
				{

					if (compareTwoCharacter(dictionary[tempHeadIndex + countCharacter], character) == 1) //neu dung la char
					{
						return 1;
					}
					else //neu ko phai
					{
						tempHeadIndex = i + 1;
						continue;
					}
				}
			}
		}
		return 0;
	}
	else
	{
		//check cho ki tu thu 3 tro len, luc nay da co Head and en
		int tempHeadIndex = headIndex;
		int i;
		for (i = headIndex; i <= endIndex; i++)
		{
			if (dictionary[i] == 0)
			{
				if ((i - tempHeadIndex) <= countCharacter)
				{
					tempHeadIndex = i + 1;
					continue;
				}
				else
				{
					if (compareTwoCharacter(dictionary[tempHeadIndex + countCharacter], character) == 1) //neu dung la char
					{
						return 1;
					}
					else //neu ko phai
					{
						tempHeadIndex = i + 1;
						continue;
					}
				}
			}
		}
		return 0;
	}
}
int checkingTwoCharacter(char character1, char character2)
{
	int result1 = checkingOneCharacter(character1);
	int result2 = checkingOneCharacter(character2);
	if (result2 == 1)
	{
		if (result1 == 1) return 3;
		else return 2;
	}
	else
	{
		if (result1 == 1) return 1;
		else return 0;
	}
}

int checkingThreeCharacter(char character1_1, char character1_2, char character2)
{
	int result1 = checkingConnectedCharacter(character1_1, character1_2);
	int result2 = checkingOneCharacter(character2);
	if (result2 == 1)
	{
		if (result1 == 1) return 3;   //ca 2 deu dung
		else return 2;   //joint dung hon
	}
	else
	{
		if (result1 == 1) return 1;  //left and right dung
		else return 0; //ca 2  deu sai
	}
}

int checkingConnectedCharacter(char left, char right)
{
	int tempHeadIndex = headIndex;
	int i;
	for (i = headIndex; i <= endIndex; i++)
	{
		if (dictionary[i] == 0)
		{
			if ((i - tempHeadIndex) <= countCharacter + 1)
			{
				tempHeadIndex = i + 1;
				continue;
			}
			else
			{
				if (compareTwoCharacter(dictionary[tempHeadIndex + countCharacter], left) == 1 && compareTwoCharacter(dictionary[tempHeadIndex + countCharacter + 1], right) == 1) //neu dung la char
				{
					return 1;
				}
				else //neu ko phai
				{
					tempHeadIndex = i + 1;
					continue;
				}
			}
		}
	}
	return 0;
}

int addCharacter(structCharacter character, int error, structCharacter characterJoint) // error == 0 => ko co error trong chu~
{
	if (error == 0)
	{
		////cout<<"ADD CHARACTER HERE AT  :  "<<countCharacter<<"    "<<character.character<<"\n";
		resultWord[countCharacter] = character.character;
		countCharacter++;
		errorWord[countError].charLeft = character;
		errorWord[countError].charJoint = characterJoint;
		countError++;
		return 1;
	}
	if (countError > 0)
	{
		////cout<<"ADD CHARACTER HERE AT  :  "<<countCharacter<<"    "<<character.character<<"\n";
		resultWord[countCharacter] = character.character;
		countCharacter++;
		return 1;
	}
	if (error > 0)
	{
		////cout<<"ADD CHARACTER HERE AT  :  "<<countCharacter<<"    "<<character.character<<"\n";
		int tempHeadIndex = headIndex;
		int tempEndIndex = endIndex;
		int i,j;
		for( i = headIndex; i <= endIndex; i++)
		{
			if (dictionary[i] == 0)
			{
				if ((i - tempHeadIndex) <= countCharacter)
				{
					tempHeadIndex = i + 1;
					continue;
				}
				else
				{
					if (compareTwoCharacter(dictionary[tempHeadIndex + countCharacter], character.character) == 1) //neu dung la char
					{
						headIndex = tempHeadIndex;
						int tempHeadIndex2 = tempHeadIndex;
						//tim end of index
						for( j = headIndex; j <= endIndex; j++)
						{
							if (dictionary[j] == 0)
							{
								tempEndIndex = j;
								if (compareTwoCharacter(dictionary[tempHeadIndex2 + countCharacter], character.character) == 0)	//tim thay end of index
								{
									endIndex = tempEndIndex;
									resultWord[countCharacter] = character.character;
									countCharacter++;
									return 1;
								}
								else
								{
									tempHeadIndex2 = j+1;
									continue;
								}
							}
						}
						//neu giu toi tan cung thi van tra ve la 1
						resultWord[countCharacter] = character.character;
						countCharacter++;
						return 1;
					}
					else //neu ko phai
					{
						tempHeadIndex = i + 1;
						continue;
					} //end ket thuc tim kiem endIndex
				} //end else neu bat dau so sanh ki tu
			} //end if tai dictionary[i] == 0
		}//end for
		return 0;
	}
}

int compareTwoCharacter(char cha1, char cha2)
{
	if (cha1 >= 'A' && cha1 <= 'Z') cha1 = cha1 - 'A' + 'a';
	if (cha2 >= 'A' && cha2 <= 'Z') cha1 = cha1 - 'A' + 'a';
	return (cha1 == cha2);
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///              SUPPORTED IMAGE FUNCTION               //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void add_over_seg_point(Image* src, seg_point_arr* arr){
	int i,j;
	unsigned char* ptr;
	int col;
	for(j = 0; j < arr->real_length; j++){
		col = arr->arr[j].pos;
		for(i = 0; i < src->mHeight; i++){
			ptr = (unsigned char*)src->mData + i*src->mWidth;
			if(col == -1) break;
			ptr[col] = 1;
		}
		if(col == -1) break;
	}
}

void add_baseline(Image *src, int upper, int lower){
	int i;
	unsigned char* ptr = (unsigned char*)src->mData + upper*src->mWidth; 
	unsigned char* ptr2 = (unsigned char*)src->mData + lower*src->mWidth; 
	for(i=0;i<src->mWidth;i++){
		ptr[i] = 1;
		ptr2[i] = 1;
	}
}
void add_horizalLine(Image* src, int row)
{
	int i;
	unsigned char* ptr = (unsigned char*)src->mData + row*src->mWidth;  
	for(i=0;i<src->mWidth;i++){
		ptr[i] = 1;
	}
}
void add_verticalLine(Image* src, int col)
{
	int i;
	for(i = 0;i<src->mHeight;i++){
		src->mData[i * src->mWidth + col] = 1;
	}
}

/*
IplImage* invert(Image* src)
{
	int width = src->mWidth;
	int height = src->mHeight;
	IplImage* img = cvCreateImage(cvSize(width, height), 8, 1);
	unsigned char*ptr;
	int i, j;
	for(i=0;i<img->height;i++)
	{
		ptr=(unsigned char*)img->imageData+i*img->widthStep;
		for(j=0;j<img->width;j++)
		{
			if (src->mData[i*width + j] == 0) ptr[j] = 255;
			else
				ptr[j] = 0;
		}
	}
	return img;
}
*/

/*
void //showImage(char* name,Image* src)
{
	cvNamedWindow(name, 1);
	IplImage* result = invert(src);
	cv//showImage(name, result);
}
*/

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///                   Slant Correction                  //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void slantCorrection()
{
	int phi = slantDectection();
	if (phi > 5 || phi < -5)
	{
		Image* slant = rotateSlant(input, phi);
		Image* result = (Image*)malloc(sizeof(Image));
		cutImage(slant, result, 6);
		input = result;
	}
}

int slantDectection()
{
	int max_ver = 0;
	int update_max_ver = 0;
	int update_phi = 0;
	update_max_ver = findMaxVertical(input);
	
	int phi;
	for ( phi = -25; phi <= 25; phi++)
	{
		Image* des = rotateSlant(input, phi);
		max_ver = findMaxVertical(des);
		
		if (max_ver > 2*update_max_ver)
		{
			update_max_ver = max_ver;
			update_phi = phi;
		}
		if (phi != 0) free(des);
	}
	return update_phi;
}

int findMaxVertical(Image* src)
{
	int width = src->mWidth;
	int height = src->mHeight;
	int max = 0;
	int sum = 0;
	int j, i;
	for (j = 0; j < width; j++)
	{
		for (i = 0; i < height; i++)
		{
			if (src->mData[i*width + j] == 1)
			{
				sum++;
			}
		}
		if (sum > max) max = sum;
		sum = 0;
	}
	return max;
}

Image* rotateSlant(Image* input, int phi)
{
	if (phi == 0) return input;
	double angle = ((double)(phi)/180)*3.14;
	//double angle = (double (phi)/180)*3.14;
	int width = input->mWidth;
	int height = input->mHeight;
    
	if (phi > 0)
	{
		int rightShift = cvRound(-height*tan(angle));  // < 0
		if (rightShift - (-width*tan(angle)) < 0) rightShift++;	//doi` them 1 pixel
		int newWidth = width - rightShift;
		Image* output = (Image*)malloc(sizeof(Image));
		createImage(output, height, newWidth);

		int i, j;
		for (i = 0; i < height; i++)
			for (j = 0; j < newWidth; j++)
			{
				output->mData[i*newWidth + j] = 0; 
			}
		for (i = 0; i < height; i++)
			for (j = 0; j < width; j++)
			{
				if (input->mData[i*width + j] == 1)
				{
					double x = j - i*tan(angle);  //hoanh do
					int y = i;  //tung do
					int x1 = cvRound(x) - rightShift;
					
					int x2;
					if (x - x1 < 0) x2 = x1 - 1;
					else x2 = x1 + 1;
					if (x1 >= 0 && x1 < newWidth) output->mData[y*newWidth + x1] = 1;
					if (x2 >= 0 && x2 < newWidth) output->mData[y*newWidth + x2] = 1;
				}
			}
		return output;
 	}
	else
	{	
		int leftShift = cvRound(-height*tan(angle));  // > 0
		if (leftShift - (-width*tan(angle)) < 0) leftShift++;	//doi` them 1 pixel
		int newWidth = width + leftShift;
		Image* output = (Image*)malloc(sizeof(Image));
		createImage(output, height, newWidth);

		int i, j;
		for (i = 0; i < height; i++)
			for (j = 0; j < newWidth; j++)
			{
				output->mData[i*newWidth + j] = 0; 
			}
		for (i = 0; i < height; i++)
			for (j = 0; j < width; j++)
			{
				if (input->mData[i*width + j] == 1)
				{
					int x = j + cvRound(-i*tan(angle));  //hoanh do
					int y = i;  //tung do
					int x1 = x;
					
					int x2;
					if (x - x1 < 0) x2 = x1 - 1;
					else x2 = x1 + 1;
					if (x1 >= 0 && x1 < newWidth) output->mData[y*newWidth + x1] = 1;
					if (x2 >= 0 && x2 < newWidth) output->mData[y*newWidth + x2] = 1;
				}
			}
		return output;
	}
}


//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///              StrokeWidth Detection                  //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void calculateStrokeWidth()
{
	Image* src = input;
	int count = 0;
	int temp;
	int width = src->mWidth;
	int height = src->mHeight;
	unsigned char* data = src->mData;
	int* result = (int*)malloc((width + height)*sizeof(int));
	int* countResult = (int*)malloc((width + height)*sizeof(int));
	int i, j;
	//duyet theo row
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (data[i*width + j] == 1)
			{
				temp = 0;
				while (data[i*width + j] == 1)
				{
					temp++;
					j++;
				}
				findingForStrokeWidth(temp, result, countResult, &count);
				j--;
			}
		}
	}
	for ( j = 0; j < width; j++)
	{
		for ( i = 0; i < height; i++)
		{
			if (data[i*width + j] == 1)
			{
				temp = 0;
				while (data[i*width + j] == 1)
				{
					temp++;
					i++;
				}
				findingForStrokeWidth(temp, result, countResult, &count);
				i--;
			}
		}
	}
	int max = 0;
	int k = 0;
	for ( i = 0; i < count; i++)
	{
		if (countResult[i] > max) 
		{
			max = countResult[i];
			k = i;
		}
	}
	int finalResult = result[k];
	free(result);
	free(countResult);
	strokeWidth = finalResult;
	strokeWidth++;
}

void findingForStrokeWidth(int value, int *result, int *countResult, int *count)
{
	int i;
	for (i = 0; i < *count; i++)
	{
		if (value == result[i]) 
		{
			
			countResult[i]++;
			return;
		}
	}
	result[*count] = value;
	countResult[*count] = 1;
	*(count) += 1;	
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///              BaseLine Detection                     //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void baseline()
{
	lowerBaseLine = -1;
	upperBaseLine = -1;
	
	Image* src = input;
	int lowerline, upperline, stroke;
	stroke = strokeWidth;
	////cout<<"stroke Width   "<<stroke<<"\n";
	rowDensity* row = findingRowDensity(input);
	int middleRow = findMiddleRow(row);
	int upperLine = findUpperBaseLine(middleRow, row);
	int lowerLine = findLowerBaseLine(middleRow, row);

	int* his = (int*)malloc(src->mHeight*sizeof(int));
	int* devi_his = (int*)malloc(src->mHeight*sizeof(int));
	density_his(src, his);
	density_deviation(his,devi_his,src->mHeight);
	int max_pos = find_max_element(his, 0, src->mHeight,2);
	max_pos = middleRow;
	lowerline = find_max_element(devi_his, max_pos + 1, src->mHeight, 1);

	int i;
	if (lowerline > middleRow + strokeWidth)
	{
		if (lowerline > lowerLine)  //neu HaiLower nam duoi
		{
			for ( i = middleRow; i < lowerline; i++)
			{
				if (row->transition[i] < 3)
				{
					lowerBaseLine = i;
					break;
				}
			}
		}
		else   //neu o gan hon
		{
			lowerBaseLine = lowerline;		//gan bang cua Hai
		}
	}
	else
	{
		for ( i = middleRow; i < lowerLine; i++)
		{
			if (row->transition[i] < 3)
			{
				lowerBaseLine = i;
				break;
			}
		}
	}
	if (lowerBaseLine == -1) lowerBaseLine = lowerLine;

	upperline = find_max_element(devi_his, 0, ceil(lowerBaseLine - 2.5*stroke), 0);

	if (upperline < middleRow - strokeWidth)		
	{
		if (upperline < upperLine)   //Cua Hai nam tren
		{	
			for ( i = middleRow; i >= upperLine; i--)
			{
				if (row->transition[i] < 3)
				{
					upperBaseLine = i;
					break;
				}
			}
		}
		else	//neu Haiupper > newUpper (nam duoi)
		{
			upperBaseLine = upperline;  //cua Hai
		}
	}
	else
	{
		for ( i = middleRow; i >= 0; i--)
		{
			if (row->transition[i] < 3)
			{
				upperBaseLine = i;
				break;
			}
		}
	}
	if (upperBaseLine == -1) upperBaseLine = upperLine;
}

rowDensity* findingRowDensity(Image* input)
{
	rowDensity* result = (rowDensity*)malloc(sizeof(rowDensity));
	result->density = (int*)malloc(input->mHeight*sizeof(int));
	result->transition = (int*)malloc(input->mHeight*sizeof(int));
	int height = input->mHeight;
	int width = input->mWidth;
	unsigned char* data = input->mData;
	int countTransition = 0;
    int countDensity = 0;
	int i, j;
	for (i = 0; i < height; i++)
	{
		countDensity = 0;
		countTransition = 0;
		for (j = 0; j < width; j++)
		{
			if (data[i*width + j] == 1)
			{
				countTransition++;
				while(data[i*width + j] == 1)
				{
					countDensity++;
					j++;
				}
				j--;
			}
		}
		result->transition[i] = countTransition;
		result->density[i] = countDensity;
	}
	return result;
}

colDensity* findingColDensity(Image* input, int begin, int end)
{
	colDensity* result = (colDensity*)malloc(sizeof(colDensity));
	result->density = (int*)malloc(input->mWidth*sizeof(int));
	result->transition = (int*)malloc(input->mWidth*sizeof(int));
	result->flag = (int*)malloc(input->mWidth*sizeof(int));
	result->flag2 = (int*)malloc(input->mWidth*sizeof(int));
	int height = input->mHeight;
	int width = input->mWidth;
	unsigned char* data = input->mData;
	int countTransition = 0;
    int countDensity = 0;
	int flag = 0;
	int flag2 = 0;
	int i, j;

	for (i = 0; i < width; i++)  //cot
	{
		countDensity = 0;
		countTransition = 0;
		flag = 3;
		flag2 = 3;
		for (j = end; j >= begin; j--)  //hang
		{
			if (data[j*width + i] == 1)
			{
				countTransition++;
				int checking = j;
				while(data[j*width + i] == 1)
				{
					countDensity++;
					j--;
				}
				j++;
				if (i == 91)
					printf("");
				if ((j + (checking - j)/2) < middleRow) flag = 2;//(middleRow + (lowerBaseLine - middleRow)/3)) flag = 2;
				else flag = 1;
				if (j + (checking - j)/2  < (middleRow + (lowerBaseLine - middleRow)/3)) flag2 = 2;
				else if ((checking - j) <= 2*strokeWidth) flag2 = 1;
				else flag2 = 2;
			}
		}
		result->transition[i] = countTransition;
		result->density[i] = countDensity;
		result->flag[i] = flag;
		result->flag2[i] = flag2;
	}
	return result;
}

int findMiddleRow(rowDensity* row)
{
	int height = input->mHeight;
	int maxTransition = 0;
	int maxDensity = 0;
	int result = -1;
	int i;
	for (i = 0; i < height; i++)
	{
		if (row->transition[i] > maxTransition)
		{
			result = i;
			maxTransition = row->transition[i];
			maxDensity = row->density[i];
		}
		else if (row->transition[i] == maxTransition)
		{
			if (row->density[i] >= maxDensity)
			{
				result = i;
				maxTransition = row->transition[i];
				maxDensity = row->density[i];
			}
		}
	}
	return result;
}

int findUpperBaseLine(int middleRow, rowDensity* row)
{
	rowDensity* deviration = (rowDensity*)malloc(sizeof(rowDensity));
	deviration->density = (int*)malloc(middleRow*sizeof(int));
	deviration->transition = (int*)malloc(middleRow*sizeof(int));
	
	int i;
	for (i = middleRow - 1; i >= 0; i--)
	{
		deviration->density[i] = abs(row->density[i+1] - row->density[i]);
		deviration->transition[i] = abs(row->transition[i+1] - row->transition[i]);
	}

	int maxTransition = 0;
	int maxDensity = 0;
	int result = -1;
	for (i = 0; i < middleRow; i++)
	{
		if (deviration->transition[i] > maxTransition)
		{
			result = i;
			maxTransition = deviration->transition[i];
			maxDensity = deviration->density[i];
		}
		else if (deviration->transition[i] == maxTransition)
		{
			if (deviration->density[i] >= maxDensity)
			{
				result = i;
				maxTransition = deviration->transition[i];
				maxDensity = deviration->density[i];
			}
		}
	}
	for (i = middleRow - 1; i >= 0; i--)
	{
		if (row->transition[i] < 3) return i;
	}
	return result;
}
int findLowerBaseLine(int middleRow, rowDensity* row)
{
	rowDensity* deviration = (rowDensity*)malloc(sizeof(rowDensity));
	deviration->density = (int*)malloc((input->mHeight - middleRow - 1)*sizeof(int));
	deviration->transition = (int*)malloc((input->mHeight - middleRow - 1)*sizeof(int));
	int i;
	for (i = middleRow + 1; i < input->mHeight; i++)
	{
		deviration->density[i - middleRow - 1] = abs(row->density[i-1] - row->density[i]);
		deviration->transition[i - middleRow - 1] = abs(row->transition[i-1] - row->transition[i]);
	}
	int maxTransition = 0;
	int maxDensity = 0;
	int result = -1;
	for (i = 0; i < input->mHeight - middleRow - 1; i++)
	{
		if (deviration->transition[i] > maxTransition)
		{
			result = i;
			maxTransition = deviration->transition[i];
			maxDensity = deviration->density[i];
		}
		else if (deviration->transition[i] == maxTransition)
		{
			if (deviration->density[i] >= maxDensity)
			{
				result = i;
				maxTransition = deviration->transition[i];
				maxDensity = deviration->density[i];
			}
		}
	}
	result += middleRow + 1;
	for (i = middleRow + 1; i < input->mHeight; i++)
	{
		if (row->transition[i] < 3) return i;
	}
	return result;
}

void density_his(Image *src , int *his){
	int i,j,count;
	count = 0;
	unsigned char *ptr_src;
	for(i = 0; i < src->mHeight; i++){
		count = 0;
		ptr_src = (unsigned char*)src->mData + i*src->mWidth;
		for(j = 0; j < src->mWidth; j++){
			if(ptr_src[j] == 1) count++;
		}
		his[i] = count;
	}
}

void density_deviation(int* his, int* devi_his, int length){
	int i;
	devi_his[0] = 0;
	for(i = 1; i < length; i++){
		devi_his[i] = abs(his[i] - his[i-1]);
	}
}

int find_max_element(int* arr, int head, int tail, int flag){
	int max;
	int pos;
	int i;
	if(flag == 1){
		max= arr[head];
		for(i=head;i<=tail;i++){
			if(arr[i] >= max){
				max = arr[i];
				pos = i;
			}
		}
	}
	else if (flag == 0){
		max= arr[tail];	
		for(i=tail;i>=head;i--){
			if(arr[i] >= max){
				max = arr[i];
				pos = i;
			}
		}
	}
	else
	{
		max= arr[head];
		for(i=head;i<=tail;i++){
			if(arr[i] >= max && i > 3*strokeWidth){
				max = arr[i];
				pos = i;
			}
		}
	}

	return pos;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///               Segment Detection                     //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void segment()
{
	Image* tmp = (Image*)malloc(sizeof(Image));
	createImage(tmp, input->mHeight, input->mWidth);

	Image* src = input;
	seg_point_arr* over_seg_arr_left;
	seg_point_arr* over_seg_arr_right;

	//using for test
	int test2 = 0;

	//init
	int characterWidth = 0;
	averageCharacterWidth = input->mHeight/2;
	
	over_seg_arr_left = over_segment(src, strokeWidth, upperBaseLine, lowerBaseLine);
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_left);
	//showImage("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh", tmp);
	
	
	int check = 1;
	double segment_criteria = 0.9*strokeWidth * (lowerBaseLine - upperBaseLine);
	int segment_density;
	while (check == 1)
	{
		check = 0;
		if (over_seg_arr_left->arr[0].pos <= 1*strokeWidth) 
		{
			remove_array_element(over_seg_arr_left, 0);
			check = 1;
			continue;
		}
		if ((src->mWidth - 1 - over_seg_arr_left->arr[over_seg_arr_left->real_length - 1].pos) <= 1*strokeWidth)
		{
			remove_array_element(over_seg_arr_left, over_seg_arr_left->real_length - 1);
			check = 1;
			continue;
		}
		segment_density = segment_pixel_density(input, 0, over_seg_arr_left->arr[0].pos, upperBaseLine - strokeWidth, lowerBaseLine + strokeWidth);
		if (segment_density < segment_criteria)
		{
			remove_array_element(over_seg_arr_left, 0);
			check = 1;
			continue;
		}
		segment_density = segment_pixel_density(input, over_seg_arr_left->arr[over_seg_arr_left->real_length - 1].pos, input->mWidth - 1, upperBaseLine - strokeWidth, lowerBaseLine + strokeWidth);
		if (segment_density < segment_criteria)
		{
			remove_array_element(over_seg_arr_left, over_seg_arr_left->real_length - 1);
			check = 1;
			continue;
		}
	}
	
	int i, j;
	for ( j = 0; j < over_seg_arr_left->real_length; j++)
	{
		if (holeDetection(over_seg_arr_left->arr[j].pos, &test2) == 1) 
		{
			remove_array_element(over_seg_arr_left, j);
			j --;
		}
	}

	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_left);
	//showImage("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", tmp);
	*/

	removeFaultSegment(over_seg_arr_left);

	

	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_left);
	//showImage("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", tmp);
	*/

	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_left);
	//showImage("11111111111111111111111111111111111111", tmp);
	*/

	STFPC2(src, over_seg_arr_left, strokeWidth, upperBaseLine, lowerBaseLine);


	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_left);
	//showImage("22222222222222222222222222222222222222222", tmp);
	*/

	post_STFPC(src, over_seg_arr_left, strokeWidth, upperBaseLine, lowerBaseLine);

	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_left);
	//showImage("33333333333333333333333333333333333", tmp);
	*/

	//calculate averageCharacterWidth again
	averageCharacterWidth = 0;
	int count = 0;
	for (i = 0; i < over_seg_arr_left->real_length - 1; i++)
	{
		if (over_seg_arr_left->arr[i].flag == 3)
		{
			if (over_seg_arr_left->arr[i + 1].flag == 3)
			{
				count++;
				characterWidth += over_seg_arr_left->arr[i + 1].pos - over_seg_arr_left->arr[i].pos;
			}
		}
	}
	if (characterWidth != 0) averageCharacterWidth = characterWidth / count;
	if (averageCharacterWidth == 0) averageCharacterWidth = input->mHeight/2;
	////cout<<"this is character Width  "<<averageCharacterWidth<<"\n";

	//hole detection
	for (j = 0; j < over_seg_arr_left->real_length; j++)
	{
		if (holeDetection(over_seg_arr_left->arr[j].pos, &test2) == 1) 
		{
			remove_array_element(over_seg_arr_left, j);
			j--;
		}
	}

	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_left);
	//showImage("444444444444444444444444444444444444", tmp);
	*/

	/*
	//hole detection
	for (j = 0; j < over_seg_arr_left->real_length; j++)
	{
		if (holeDetection2(over_seg_arr_left->arr[j].pos, test2) == 1) 
		{
			remove_array_element(over_seg_arr_left, j);
			j--;
		}
	}
	*/
	
	//right segment
	over_seg_arr_right = over_segment2(src, strokeWidth, upperBaseLine, lowerBaseLine);

	int check2 = 1;
	
	while (check2 == 1)
	{
		check2 = 0;
		if (over_seg_arr_right->arr[0].pos <= strokeWidth) 
		{
			remove_array_element(over_seg_arr_right, 0);
			check2 = 1;
			continue;
		}
		if ((src->mWidth - 1 - over_seg_arr_right->arr[over_seg_arr_right->real_length - 1].pos) <= strokeWidth)
		{
			remove_array_element(over_seg_arr_right, over_seg_arr_right->real_length - 1);
			check2 = 1;
			continue;
		}
		segment_density = segment_pixel_density(input, 0, over_seg_arr_right->arr[0].pos, upperBaseLine - strokeWidth, lowerBaseLine + strokeWidth);
		if (segment_density < segment_criteria)
		{
			remove_array_element(over_seg_arr_right, 0);
			check2 = 1;
			continue;
		}
		segment_density = segment_pixel_density(input, over_seg_arr_right->arr[over_seg_arr_right->real_length - 1].pos, input->mWidth - 1, upperBaseLine - strokeWidth, lowerBaseLine + strokeWidth);
		if (segment_density < segment_criteria)
		{
			remove_array_element(over_seg_arr_right, over_seg_arr_right->real_length - 1);
			check2 = 1;
			continue;
		}		
	}

	//////cout<<averageCharacterWidth<<"\n";
	for( j = 0; j < over_seg_arr_right->real_length; j++)
	{
		if (holeDetection(over_seg_arr_right->arr[j].pos, &test2) == 1) 
		{
			remove_array_element(over_seg_arr_right, j);
				j--;
		}
	}

	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_right);
	//showImage("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", tmp);
	*/

	removeFaultSegment(over_seg_arr_right);
	
	
	

	
	
	
	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_right);
	//showImage("5555555555555555555555555555555555555555555", tmp);
	*/

	STFPC3(src, over_seg_arr_right, strokeWidth, upperBaseLine, lowerBaseLine);

	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_right);
	//showImage("66666666666666666666666666666666666666666666666", tmp);
*/

	post_STFPC2(src, over_seg_arr_right, strokeWidth, upperBaseLine, lowerBaseLine);

	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_right);
	//showImage("777777777777777777777777777777777777777777777", tmp);
*/

	for ( j = 0; j < over_seg_arr_right->real_length; j++)
	{
		if (holeDetection(over_seg_arr_right->arr[j].pos, &test2) == 1) 
		{
			remove_array_element(over_seg_arr_right, j);
			j--;
		}
	}

	/*
	copyImage(input, tmp);
	add_baseline(tmp, upperBaseLine, lowerBaseLine);
	add_over_seg_point(tmp, over_seg_arr_right);
	//showImage("888888888888888888888888888888888888", tmp);
*/

	/*
	for ( j = 0; j < over_seg_arr_right->real_length; j++)
	{
		if (holeDetection2(over_seg_arr_right->arr[j].pos, test2) == 1) 
		{
			remove_array_element(over_seg_arr_right, j);
			j--;
		}
	}*/

	leftSeg = over_seg_arr_left;
	rightSeg = over_seg_arr_right;
	//////cout<<"hole detecion  "<<holeDetection(leftSeg->arr[3].pos, test2)<<"\n";;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///					Over Segment                     //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

seg_point_arr* over_segment(Image* src, int stroke, int upper, int lower)
{
	int i,j;
	int* density_arr = (int*)malloc(src->mWidth*sizeof(int));
	pixel_density(src,upper, lower, density_arr);   //tinh density cho moi colum

	int* over_seg_arr = (int*)malloc(src->mWidth*sizeof(int));
	int* over_seg_den = (int*)malloc(src->mWidth*sizeof(int)); 
			// over_seg_den[i]=0 khi density_arr[i]=0
			//	over_seg_den[i]=1 khi density_arr[i]<seg_criteria
		
	init_over_seg_arr(over_seg_arr, src->mWidth);
	init_over_seg_arr(over_seg_den, src->mWidth);
	double seg_criteria = SEG_CRITERIA_RATE*(stroke);     //vung pixel
	
	int over_seg_arr_length;
	find_seg_point(src, density_arr, over_seg_arr, over_seg_den, seg_criteria, &over_seg_arr_length);
	
		//input la density_arr, seg_criteria
		//tinh ra output la over_seg_arr, over_seg_den va chieu dai over_seg_arr_length

	Image* tmp = (Image*)malloc(sizeof(Image));
	createImage(tmp, input->mHeight, input->mWidth);
	for( i = 0; i < over_seg_arr_length; i++)
	{
		//int tail = cont_black_point->arr[i].tail;
		int head = over_seg_arr[i];
		for( j = 0; j < input->mHeight; j++)
		{
			tmp->mData[j*input->mWidth + head] = 1;
			//tmp->mData[j*input->mWidth + tail] = 1;
		}
	}
	//showImage("first segmentation", tmp);

	int_array* dis_point_arr= (int_array *) create_int_array(over_seg_arr_length); //cac segment roi rac
	cont_arr* cont_black_point = (cont_arr *) create_cont_array(over_seg_arr_length);   //cac segment den lien tuc
	cont_arr* cont_white_point = (cont_arr *) create_cont_array(over_seg_arr_length);   //cac segment trang lien tuc
	seg_point_classification(over_seg_arr, over_seg_arr_length, over_seg_den, dis_point_arr, cont_black_point, cont_white_point);  
			//phan loai cac segment theo cac loai nhu tren

	
	copyImage(input, tmp);
	for( i = 0; i < cont_black_point->real_length; i++)
	{
		int tail = cont_black_point->arr[i].tail;
		int head = cont_black_point->arr[i].head;
		for( j = 0; j < input->mHeight; j++)
		{
			tmp->mData[j*input->mWidth + head] = 1;
			tmp->mData[j*input->mWidth + tail] = 1;
		}
	}
	//showImage("continue black", tmp);

	copyImage(input, tmp);
	for( i = 0; i < cont_white_point->real_length; i++)
	{
		int tail = cont_white_point->arr[i].tail;
		int head = cont_white_point->arr[i].head;
		for( j = 0; j < input->mHeight; j++)
		{
			tmp->mData[j*input->mWidth + head] = 1;
			tmp->mData[j*input->mWidth + tail] = 1;
		}
	}
	//showImage("continue white", tmp);

	copyImage(input, tmp);
	for( i = 0; i < dis_point_arr->real_length; i++)
	{
		//int tail = cont_white_point->arr[i].tail;
		int head = dis_point_arr->arr[i];
		for( j = 0; j < input->mHeight; j++)
		{
			tmp->mData[j*input->mWidth + head] = 1;
			//tmp->mData[j*input->mWidth + tail] = 1;
		}
	}
	//showImage("dispoint segment", tmp);


	int black_segment_length = cal_black_segment_length(cont_black_point, stroke);  
	int_array* black_segment = create_int_array(black_segment_length);  
	cal_black_segment(cont_black_point, stroke, black_segment);  //chuan hoa cac black segment
		//vi du   1 1 1 1 1 1 : 5 -> 10 va strokeWidth = 3 => chuan hoa thanh 1 1 : 5, 8   
	int_array* white_segment = create_int_array(cont_white_point->real_length);
	cal_white_segment(cont_white_point, white_segment);   //chuan hoa cac white segment

	seg_point_arr* over_seg_point_arr = create_seg_point_array(dis_point_arr->real_length + black_segment->length + white_segment->length);
	cal_over_seg_arr2(over_seg_point_arr, dis_point_arr, black_segment, white_segment);  //tich hop ca 3 loai tren
	sort_arr2(over_seg_point_arr);   //sap xep lai
	return over_seg_point_arr;
}

seg_point_arr* over_segment2(Image* src, int stroke, int upper, int lower)
{
	int* density_arr = (int*)malloc(src->mWidth*sizeof(int));
	pixel_density(src,upper, lower, density_arr);   //tinh density cho moi colum

	int* over_seg_arr = (int*)malloc(src->mWidth*sizeof(int));
	int* over_seg_den = (int*)malloc(src->mWidth*sizeof(int)); 
			// over_seg_den[i]=0 khi density_arr[i]=0
			//	over_seg_den[i]=1 khi density_arr[i]<seg_criteria
		
	init_over_seg_arr(over_seg_arr, src->mWidth);
	init_over_seg_arr(over_seg_den, src->mWidth);
	double seg_criteria = SEG_CRITERIA_RATE*(stroke);     //vung pixel
	
	int over_seg_arr_length;
	find_seg_point(src, density_arr, over_seg_arr, over_seg_den, seg_criteria, &over_seg_arr_length);
		//input la density_arr, seg_criteria
		//tinh ra output la over_seg_arr, over_seg_den va chieu dai over_seg_arr_length

	int_array* dis_point_arr= (int_array *) create_int_array(over_seg_arr_length); //cac segment roi rac
	cont_arr* cont_black_point = (cont_arr *) create_cont_array(over_seg_arr_length);   //cac segment den lien tuc
	cont_arr* cont_white_point = (cont_arr *) create_cont_array(over_seg_arr_length);   //cac segment trang lien tuc
	seg_point_classification(over_seg_arr, over_seg_arr_length, over_seg_den, dis_point_arr, cont_black_point, cont_white_point);  
			//phan loai cac segment theo cac loai nhu tren
	
	
	int black_segment_length = cal_black_segment_length(cont_black_point, stroke);  
	int_array* black_segment = create_int_array(black_segment_length);  
	cal_black_segment2(cont_black_point, stroke, black_segment);  //chuan hoa cac black segment
		//vi du   1 1 1 1 1 1 : 5 -> 10 va strokeWidth = 3 => chuan hoa thanh 1 1 : 5, 8   
	int_array* white_segment = create_int_array(cont_white_point->real_length);
	cal_white_segment2(cont_white_point, white_segment);   //chuan hoa cac white segment
	
	seg_point_arr* over_seg_point_arr = create_seg_point_array(dis_point_arr->real_length + black_segment->length + white_segment->length);
	cal_over_seg_arr2(over_seg_point_arr, dis_point_arr, black_segment, white_segment);  //tich hop ca 3 loai tren
	sort_arr2(over_seg_point_arr);   //sap xep lai
	return over_seg_point_arr;
}

void pixel_density(Image *src, int upper, int lower, int* density_arr)
{
	int i,j;
	int densityCount;
	unsigned char* data = src->mData;
	int width = src->mWidth;
	for(j = 0; j < src->mWidth; j++){
		densityCount = 0;
		for(i = upper; i <= lower; i++){
			if(data[i*width + j] == 1) densityCount++;
		}
		density_arr[j] = densityCount;
	}
}

void init_over_seg_arr(int* arr, int width){
	int i;
	for(i=0;i<width;i++){
		arr[i] = -1;
	}
}

void find_seg_point(Image* src, int * density_arr, int* over_seg_arr, int* over_seg_den, double seg_criteria, int* length){
	int i;
	int pos=0;
	for(i=0;i<src->mWidth;i++){
		if(density_arr[i] == 0){
			over_seg_arr[pos] = i;
			over_seg_den[pos] = 0;
			pos++;
		}
		else if(density_arr[i]<seg_criteria){
			over_seg_arr[pos] = i;
			over_seg_den[pos] = 1;
			pos++;
		}
	}
	(*length) = pos;
}	
int_array* create_int_array(int length){
	int_array* temp =(int_array *) malloc(sizeof(int_array));
	temp->length =  length;
	temp->real_length = 0;
	temp->arr = (int *) malloc(length*sizeof(int));
	return temp;
}
cont_arr* create_cont_array(int length){
	cont_arr* temp =(cont_arr *) malloc(sizeof(cont_arr));
	temp->length =  length;
	temp->real_length = 0;
	temp->arr = (cont_area *) malloc(length*sizeof(cont_area));
	return temp;
}
void seg_point_classification(int* over_seg_arr, int length, int* over_seg_den,int_array* dis_point_arr, cont_arr* cont_black_point, cont_arr* cont_white_point){
	int i;
	int cur_pos;
	int next_pos;
	int cur_value, next_value;
	int flag = 1; // =1: discrete point; =2: continous black point; =3: continous white point
	int cont_head;
	int cont_tail;
	int dis_arr_index = 0;
	int cont_black_index = 0;
	int cont_white_index = 0;
	
	for(i=0; i<length; i++){
		cur_pos = over_seg_arr[i];
		next_pos = over_seg_arr[i+1];
		cur_value = over_seg_den[i];
		next_value = over_seg_den[i+1];
		if(flag == 1){
			if(next_pos == cur_pos + 1){
				if(cur_value == next_value && cur_value == 1){
					flag = 2;
					cont_head = cur_pos;
				}
				else if(cur_value == next_value && cur_value == 0){
					flag = 3;
					cont_head = cur_pos;
				}
				else{
					dis_point_arr->arr[dis_arr_index] = cur_pos;
					dis_arr_index++;
				}
			}
			else{
				dis_point_arr->arr[dis_arr_index] = cur_pos;
				dis_arr_index++;
			}
		}
		else if(flag == 2){
				if(next_value == 0 || next_pos != cur_pos + 1){   //cap nhat pixel den ma hien thi pixel trang
					cont_tail = cur_pos;
					//cap nhat mang black
					update_cont_arr(cont_black_point,cont_black_index, cont_head, cont_tail);
					cont_black_index++;
					flag = 1;
				}
				else if(next_value == 1 && next_pos == cur_pos + 1){
					cont_tail = cur_pos;
				}
		}
		else if(flag == 3){
			if(next_value == 1 || next_pos != cur_pos + 1){
					cont_tail = cur_pos;
					//cap nhat mang black
					update_cont_arr(cont_white_point,cont_white_index, cont_head, cont_tail);
					cont_white_index++;
					flag = 1;
				}
				else if(next_value == 0 && next_pos == cur_pos + 1){
					cont_tail = cur_pos;
				}
		}
	}
	dis_point_arr->real_length = dis_arr_index;
	cont_black_point->real_length = cont_black_index;
	cont_white_point->real_length = cont_white_index;
}

void update_cont_arr(cont_arr* arr, int pos, int head, int tail ){
	(arr->arr[pos]).head = head;
	(arr->arr[pos]).tail = tail;
}

int cal_black_segment_length(cont_arr* cont_black_point, int stroke){
	int i, result;
	result = 0;
	int temp;
	for(i = 0; i < cont_black_point->real_length; i++)
	{
		temp = (cont_black_point->arr[i].tail - cont_black_point->arr[i].head + 1)/stroke + 2;
		//if (temp == 1) temp++;
		result = result + temp;
	}
	return result;
}
void cal_black_segment(cont_arr* cont_black_point, int stroke, int_array * segment){
	int i,j;
	int count;
	int cur_length = 0;
	int cur_pos;
	for(i = 0; i < cont_black_point->real_length; i++){
		count = (cont_black_point->arr[i].tail - cont_black_point->arr[i].head + 1)/stroke + 2;
		//if (count == 1) count++;
		cur_pos = cont_black_point->arr[i].head;
		for(j=0;j<count;j++){
			if (j == count - 1 && j > 0)
			{
				cur_pos = cont_black_point->arr[i].tail;
				segment->arr[cur_length + j] = cur_pos;
			}
			else
			{
				segment->arr[cur_length + j] = cur_pos;
				cur_pos = cur_pos + stroke;
			}
		}
		cur_length += count;
	}
}

void cal_black_segment2(cont_arr* cont_black_point, int stroke, int_array * segment){
	int i,j;
	int count;
	int cur_length = 0;
	int cur_pos;
	for(i = cont_black_point->real_length - 1; i >= 0; i--)
	{
		count = (cont_black_point->arr[i].tail - cont_black_point->arr[i].head + 1)/stroke + 2;
		//if (count == 1) count++;
		cur_pos = cont_black_point->arr[i].tail;
		for(j = count - 1; j >= 0; j--)
		{
			if (j == 0)
			{
				//segment->arr[cur_length + j] = cur_pos;
				segment->arr[cur_length + j] = cont_black_point->arr[i].head;
			}
			else
			{
				segment->arr[cur_length + j] = cur_pos;
				cur_pos = cur_pos - stroke;
			}
		}
		cur_length += count;
	}
}


void cal_white_segment(cont_arr* cont_white, int_array * segment){
	int i, j;
	int min, minPos;
	for(i = 0; i < cont_white->real_length; i++){
		min = mVerticalDensity[cont_white->arr[i].head];
		minPos = cont_white->arr[i].head;
		for ( j = cont_white->arr[i].head; j <= cont_white->arr[i].tail; j++)
		{
			if (mVerticalDensity[j] <= min)
			{
				min = mVerticalDensity[j];
				minPos = j;
			}
		}
		segment->arr[i] = minPos;
	}
}
void cal_white_segment2(cont_arr* cont_white, int_array * segment){
	int i, j;
	int min, minPos;
	for(i = 0; i < cont_white->real_length; i++){
		min = mVerticalDensity[cont_white->arr[i].head];
		minPos = cont_white->arr[i].head;
		for ( j = cont_white->arr[i].tail; j >= cont_white->arr[i].head; j--)
		{
			if (mVerticalDensity[j] <= min)
			{
				min = mVerticalDensity[j];
				minPos = j;
			}
		}
		segment->arr[i] = minPos;
	}
}

void cal_over_seg_arr2(seg_point_arr* over_seg, int_array* dis_arr, int_array* black_seg, int_array* white_seg){
	int i,pos;
	pos = 0;
	for(i=0;i<dis_arr->real_length;i++){
		over_seg->arr[i].pos = dis_arr->arr[i];
		over_seg->arr[i].flag = 1;
	}
	pos = dis_arr->real_length;
	for(i=0;i<black_seg->length;i++)
	{
		over_seg->arr[i+pos].pos = black_seg->arr[i];
		over_seg->arr[i+pos].flag = 2;
	}
	pos = pos + black_seg->length;
	for(i=0;i<white_seg->length;i++)
	{
		over_seg->arr[i+pos].pos = white_seg->arr[i];
		over_seg->arr[i+pos].flag = 3;
	}
}

void sort_arr2(seg_point_arr* arr){
	int i, j;
	for ( i = 0; i < arr->length - 1; i++) {
        int min = i;
        for ( j = i + 1; j < arr->length; j++) {
			if (arr->arr[j].pos < arr->arr[min].pos) {
                min = j;
            }
        }
        if (i != min) {
			int swap = arr->arr[i].pos;
			int swap2 = arr->arr[i].flag;
			arr->arr[i].pos = arr->arr[min].pos;
			arr->arr[i].flag = arr->arr[min].flag;
			arr->arr[min].pos = swap;
			arr->arr[min].flag = swap2;
        }
    }
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///					     STFPC                          //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void STFPC2(Image *src, seg_point_arr* over_arr, int stroke, int upper, int lower){
	int density_criteria = (stroke)*(lower - upper)*2/2;
	int low = 0;
	int high = input->mHeight - 1;
	low = upperBaseLine;
	high = lowerBaseLine;

	int i;
	int segment_density, next_segment_density;
	int flagLeft = 0;
	int flagRight = 0;
	for(i = -1; i < over_arr->real_length - 1; i++)
	{
		if (i == -1) 
		{
			flagLeft = 3;
			flagRight = over_arr->arr[i+1].flag;
			segment_density = segment_pixel_density(src, 0, over_arr->arr[i+1].pos, low, high);
		}
		else 
		{
			flagLeft = over_arr->arr[i].flag;
			flagRight = over_arr->arr[i+1].flag;
			segment_density = segment_pixel_density(src,over_arr->arr[i].pos,over_arr->arr[i+1].pos,low, high);
		}
		if(segment_density < density_criteria){
			if (i == over_arr->real_length - 2) next_segment_density = segment_pixel_density(src,over_arr->arr[i+1].pos, input->mWidth - 1, low, high);
			else next_segment_density = segment_pixel_density(src,over_arr->arr[i+1].pos,over_arr->arr[i+2].pos, low, high);
				if(next_segment_density >= density_criteria){
					//remove i
					if(flagLeft == 3 && flagRight == 3) {}
					else if(flagLeft == 3 && flagRight != 3) 
					{
						remove_array_element(over_arr, i+1);
						i = -2;
					}
					else if (flagLeft != 3 && flagRight == 3)
					{
						if (i != -1) 
						{
							remove_array_element(over_arr, i);
							i = -2;
						}
					}
					else
					{
						remove_array_element(over_arr, i+1);
						i = -2;
					}
				}
				else{
					//remove i+1, update i = i+1
					if(flagLeft == 3 && flagRight == 3) {}
					else if(flagRight == 3 && flagLeft != 3)
					{
						if (i != -1) 
						{
							remove_array_element(over_arr, i);
							i = -2;
						}
					}
					else if(flagRight != 3 && flagLeft == 3)
					{
						remove_array_element(over_arr, i+1);
						i = -2;
					}
					else 
					{
						remove_array_element(over_arr, i+1);
						i = -2;
					}
				}
		}
	}
}
void STFPC3(Image *src, seg_point_arr* over_arr, int stroke, int upper, int lower){
	int i;
	int density_criteria = (stroke)*(lower - upper)*2/2;
	int low = 0;
	int high = input->mHeight - 1;
	low = upperBaseLine;
	high = lowerBaseLine;
	int segment_density, next_segment_density;
	int flagLeft, flagRight;

	for(i = over_arr->real_length; i > 0; i--)
	{
		if (i == over_arr->real_length)
		{
			segment_density = segment_pixel_density(src,over_arr->arr[i-1].pos, input->mWidth - 1, low, high);
			flagRight = 3;
			flagLeft = over_arr->arr[i-1].flag;
		}
		else 
		{
			flagRight = over_arr->arr[i].flag;
			flagLeft = over_arr->arr[i-1].flag;
			segment_density = segment_pixel_density(src,over_arr->arr[i-1].pos,over_arr->arr[i].pos, low, high);
		}
		if(segment_density < density_criteria)
		{
			if (i == 1)  next_segment_density = segment_pixel_density(src, 0, over_arr->arr[i - 1].pos, low, high);
			else next_segment_density = segment_pixel_density(src,over_arr->arr[i - 2].pos,over_arr->arr[i - 1].pos, low, high);
				if(next_segment_density >= density_criteria){
					//remove i
					if(flagLeft == 3 && flagRight == 3) {}
					else if(flagRight == 3 && flagLeft != 3)
					{
						remove_array_element(over_arr, i - 1);//i - 1
						i = over_arr->real_length + 1;
					}
					else if(flagRight != 3 && flagLeft == 3)
					{
						if (i != over_arr->real_length)
						{
							remove_array_element(over_arr, i);
							i = over_arr->real_length + 1;
						}
					}
					else
					{
						remove_array_element(over_arr, i - 1);
						i = over_arr->real_length + 1;
					}
				}
				else
				{
					//remove i+1, update i = i+1
					if(flagLeft == 3 && flagRight == 3) {}
					else if(flagLeft == 3 && flagRight != 3)
					{
						if (i != over_arr->real_length) 
						{
							remove_array_element(over_arr, i);
							i = over_arr->real_length + 1;
						}
					}
					else if(flagLeft != 3 && flagRight == 3)
					{
						remove_array_element(over_arr, i - 1);
						i = over_arr->real_length + 1;
					}
					else 
					{
						remove_array_element(over_arr, i - 1);
						i = over_arr->real_length + 1;
					}
				}
		}
	}
}

void post_STFPC(Image* src, seg_point_arr* over_arr, int stroke, int upper, int lower)
{
	int density_criteria = stroke*(lower - upper)/2;
	
	/*
	int i;
	int segment_density, next_segment_density;
	for(i=0;i<over_arr->real_length - 1;i++){
		segment_density = segment_pixel_density(src,over_arr->arr[i].pos,over_arr->arr[i+1].pos,upper,lower);
		if(segment_density < density_criteria){
				next_segment_density = segment_pixel_density(src,over_arr->arr[i+1].pos,over_arr->arr[i+2].pos,upper,lower);
				if(next_segment_density >= density_criteria){
					//remove i
					if(over_arr->arr[i].flag == 3 && over_arr->arr[i+1].flag == 3) {
						if(segment_density < ceil(0.4*density_criteria)) remove_array_element(over_arr, i);
					}
					else if(over_arr->arr[i].flag == 3 && over_arr->arr[i+1].flag != 3) remove_array_element(over_arr, i+1);
					else remove_array_element(over_arr, i);
				}
				else{
					if(over_arr->arr[i].flag == 3 && over_arr->arr[i+1].flag == 3){
						if(segment_density < ceil(0.4*density_criteria) && next_segment_density > ceil(0.55*density_criteria)) remove_array_element(over_arr, i);
					}
					else if(over_arr->arr[i+1].flag == 3 && over_arr->arr[i].flag != 3) remove_array_element(over_arr, i);
					else remove_array_element(over_arr, i+1);
					//i = i + 1;
				}
		}
	}
	*/

	int low = 0;
	int high = input->mHeight - 1;
	low = upperBaseLine;
	high = lowerBaseLine;

	int i;
	int segment_density, next_segment_density;
	int flagLeft = 0;
	int flagRight = 0;
	for(i = -1; i < over_arr->real_length - 1; i++)
	{
		if (i == -1) 
		{
			flagLeft = 3;
			flagRight = over_arr->arr[i+1].flag;
			segment_density = segment_pixel_density(src, 0, over_arr->arr[i+1].pos, low, high);
		}
		else 
		{
			flagLeft = over_arr->arr[i].flag;
			flagRight = over_arr->arr[i+1].flag;
			segment_density = segment_pixel_density(src,over_arr->arr[i].pos,over_arr->arr[i+1].pos,low, high);
		}
		if(segment_density < density_criteria){
			if (i == over_arr->real_length - 2) next_segment_density = segment_pixel_density(src,over_arr->arr[i+1].pos, input->mWidth - 1, low, high);
			else next_segment_density = segment_pixel_density(src,over_arr->arr[i+1].pos,over_arr->arr[i+2].pos, low, high);
				if(next_segment_density >= density_criteria){
					//remove i
					if(flagLeft == 3 && flagRight == 3)
					{
						if(segment_density < ceil(0.4*density_criteria)) remove_array_element(over_arr, i + 1);
					}
				}
				else{
					//remove i+1, update i = i+1
					if(flagLeft == 3 && flagRight == 3)
					{
						if(segment_density < ceil(0.4*density_criteria) && next_segment_density > ceil(0.55*density_criteria)) remove_array_element(over_arr, i + 1);
					}
				}
		}
	}
}

void post_STFPC2(Image* src, seg_point_arr* over_arr, int stroke, int upper, int lower)
{
	int density_criteria = stroke*(lower - upper)/2;
	int i;
	int low = 0;
	int high = input->mHeight - 1;
	low = upperBaseLine;
	high = lowerBaseLine;
	int segment_density, next_segment_density;
	int flagLeft, flagRight;

	for(i = over_arr->real_length; i > 0; i--){
		if (i == over_arr->real_length)
		{
			segment_density = segment_pixel_density(src,over_arr->arr[i-1].pos, input->mWidth - 1, low, high);
			flagRight = 3;
			flagLeft = over_arr->arr[i-1].flag;
		}
		else 
		{
			flagRight = over_arr->arr[i].flag;
			flagLeft = over_arr->arr[i-1].flag;
			segment_density = segment_pixel_density(src,over_arr->arr[i-1].pos,over_arr->arr[i].pos, low, high);
		}
		if(segment_density < density_criteria)
		{
			if (i == 1)  next_segment_density = segment_pixel_density(src, 0, over_arr->arr[i - 1].pos, low, high);
			else next_segment_density = segment_pixel_density(src,over_arr->arr[i - 2].pos,over_arr->arr[i - 1].pos, low, high);
				if(next_segment_density >= density_criteria){
					//remove i
					if(flagLeft == 3 && flagRight == 3)
					{
						if(segment_density < ceil(0.4*density_criteria)) remove_array_element(over_arr, i - 1);
					}
				}
				else
				{
					//remove i+1, update i = i+1
					if(flagLeft == 3 && flagRight == 3)
					{
						if(segment_density < ceil(0.4*density_criteria) && next_segment_density > ceil(0.55*density_criteria)) remove_array_element(over_arr, i - 1);
					}
				}
		}
	}
	
}

int segment_pixel_density(Image *src, int head, int tail, int upper, int lower){
	int i,j;
	int density_count = 0;
	//unsigned char* ptr;
	unsigned char* ptr;
	for(j=head;j<=tail;j++){
		for(i=upper;i<=lower;i++){
			ptr = (unsigned char*)src->mData+i*src->mWidth;
			if(ptr[j]==1) density_count++;
		}
	}
	return density_count;
}


void remove_array_element(seg_point_arr* arr, int index){
	int i;
	arr->real_length--;
	for(i=index;i<arr->real_length;i++){
		arr->arr[i].pos = arr->arr[i+1].pos;
		arr->arr[i].flag = arr->arr[i+1].flag;
	}
	for(i=arr->real_length;i<arr->length;i++){
		//arr->arr[i].pos = -1;
		arr->arr[i].flag = -1;
	}
	
}

seg_point_arr* create_seg_point_array(int length){
	seg_point_arr* temp =(seg_point_arr *) malloc(sizeof(seg_point_arr));
	temp->length =  length;
	temp->real_length = length;
	temp->arr = (seg_point *) malloc(length*sizeof(seg_point));
	return temp;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///					HOLE DETECTION                      //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

int holeDetection(int col, int* countTransition)
{
	*countTransition = 0;
	int height = thinningOfInput->mHeight;
	int width = thinningOfInput->mWidth;
	unsigned char* data = thinningOfInput->mData;
	int upperRow, lowerRow, thirdRow;
	upperRow = lowerRow = thirdRow = -1;
	int i, j;
	for (i = 0; i < height; i++)
	{
		if (data[i*width + col] == 1) 
		{
			(*countTransition)++;
			if (upperRow == -1) upperRow = i;
			else if (lowerRow == -1) lowerRow = i;
			else thirdRow = i;
			while (data[i*width + col] == 1) i++;
			i--;	
		}
	}
	
	if (*countTransition < 2) return 0;
	//////cout<<"upperRow  "<<upperRow<<"   lowerRow"<<lowerRow<<"\n";

	//nua trai
	int newLeftWidth = (int)(ceil(averageCharacterWidth*0.75));
	int newRightWidth = (int)(ceil(averageCharacterWidth*0.75));
	if (newLeftWidth > col) newLeftWidth = col + 1;
	if (newRightWidth > input->mWidth - col) newRightWidth = input->mWidth - col - 1;

	//them vo
	//if (newRightWidth > newLeftWidth) newRightWidth = newLeftWidth;
	//else newLeftWidth = newRightWidth;

	int leftOffset = col - newLeftWidth + 1;
	Image* leftImage = (Image*)malloc(sizeof(Image));
	createImage(leftImage, input->mHeight, newLeftWidth);
	for( i = 0; i < input->mHeight; i++)
		for( j = 0; j < newLeftWidth; j++)
		{
			leftImage->mData[i*newLeftWidth + j] = thinningOfInput->mData[i*input->mWidth + j + leftOffset];
		}
	
	int lengthOfLeftUpperEndPoint = 0;
	Image* leftImage1 = (Image*)malloc(sizeof(Image));
	createImage(leftImage1, leftImage->mHeight, leftImage->mWidth);
	copyImage(leftImage, leftImage1);
	int* leftUpperEndPoint = endLeftContour(upperRow, &lengthOfLeftUpperEndPoint, leftImage1, 0);

	copyImage(leftImage, leftImage1);
	int lengthOfLeftLowerEndPoint = 0;
	int* leftLowerEndPoint = endLeftContour(lowerRow, &lengthOfLeftLowerEndPoint, leftImage1, 0);
	
	int* leftThirdEndPoint;
	int lengthOfLeftThirdEndPoint;
	if (thirdRow != -1)
	{
		copyImage(leftImage, leftImage1);
		lengthOfLeftThirdEndPoint = 0;
		leftThirdEndPoint = endLeftContour(thirdRow, &lengthOfLeftThirdEndPoint, leftImage1, 0);
	}
	

	int rightOffset = col;
	Image* rightImage = (Image*)malloc(sizeof(Image));
	createImage(rightImage, input->mHeight, newRightWidth);
	for( i = 0; i < input->mHeight; i++)
		for( j = 0; j < newRightWidth; j++)
		{
			rightImage->mData[i*newRightWidth + j] = thinningOfInput->mData[i*input->mWidth + j + rightOffset];
		}

	Image* rightImage1 = (Image*)malloc(sizeof(Image));
	createImage(rightImage1, rightImage->mHeight, rightImage->mWidth);
	copyImage(rightImage, rightImage1);
	int lengthOfRightUpperEndPoint = 0;
	int* rightUpperEndPoint = endRightContour(upperRow, &lengthOfRightUpperEndPoint, rightImage1, 0);

	copyImage(rightImage, rightImage1);
	int lengthOfRightLowerEndPoint = 0;
	int* rightLowerEndPoint = endRightContour(lowerRow, &lengthOfRightLowerEndPoint, rightImage1, 0);
	
	int* rightThirdEndPoint;
	int lengthOfRightThirdEndPoint;
	if (thirdRow != -1)
	{
		copyImage(rightImage, rightImage1);
		lengthOfRightThirdEndPoint = 0;
		rightThirdEndPoint = endRightContour(thirdRow, &lengthOfRightThirdEndPoint, rightImage1, 0);
	}

	int standard = 2*strokeWidth;

	////printImage(leftImage);
	//////cout<<"\n\n\n";
	////printImage(rightImage);
	//////cout<<"\n\n\n";
	//so sanh duong tren va duong duoi
	int checkLeft12 = 0;
	for( i = 0; i < lengthOfLeftUpperEndPoint && checkLeft12 == 0; i++)
	{
		for( j = 0; j < lengthOfLeftLowerEndPoint && checkLeft12 == 0; j++)
		{
			if (i == 0 && j == 0) continue;                  
			if (abs(leftUpperEndPoint[i*2] - leftLowerEndPoint[j*2]) <= standard)
			{
				if (abs(leftUpperEndPoint[i*2 + 1] - leftLowerEndPoint[j*2 + 1]) <= standard) checkLeft12 = 1;
			}
		}
	}
	if (checkLeft12 == 1)	//dung
	{
		int checkRight12 = 0;
		for( i = 0; i < lengthOfRightUpperEndPoint && checkRight12 == 0; i++)
		{
			for( j = 0; j < lengthOfRightLowerEndPoint && checkRight12 == 0; j++)
			{
				if (i == 0 && j == 0) continue;
				if (abs(rightUpperEndPoint[i*2] - rightLowerEndPoint[j*2]) <= standard)
				{
					if (abs(rightUpperEndPoint[i*2 + 1] - rightLowerEndPoint[j*2 + 1]) <= standard) checkRight12 = 1;
				}
			}
		}
		if (checkRight12 == 1) return checkRight12;
	}

	if (thirdRow != -1)
	{
		int checkLeft13 = 0;
		for( i = 0; i < lengthOfLeftUpperEndPoint && checkLeft13 == 0; i++)
		{
			for( j = 0; j < lengthOfLeftThirdEndPoint && checkLeft13 == 0; j++)
			{
				if (i == 0 && j == 0) continue;
				if (abs(leftUpperEndPoint[i*2] - leftThirdEndPoint[j*2]) <= standard)
				{
					if (abs(leftUpperEndPoint[i*2 + 1] - leftThirdEndPoint[j*2 + 1]) <= standard) checkLeft13 = 1;
				}
			}
		}
		if (checkLeft13 == 1)	//dung
		{
			int checkRight13 = 0;
			for( i = 0; i < lengthOfRightUpperEndPoint && checkRight13 == 0; i++)
			{
				for( j = 0; j < lengthOfRightThirdEndPoint && checkRight13 == 0; j++)
				{
					if (i == 0 && j == 0) continue;
					if (abs(rightUpperEndPoint[i*2] - rightThirdEndPoint[j*2]) <= standard)
					{
						if (abs(rightUpperEndPoint[i*2 + 1] - rightThirdEndPoint[j*2 + 1]) <= standard) checkRight13 = 1;
					}
				}
			}
			if (checkRight13 == 1) return checkRight13;
		}
		int checkLeft23 = 0;
		for( i = 0; i < lengthOfLeftLowerEndPoint && checkLeft23 == 0; i++)
		{
			for( j = 0; j < lengthOfLeftThirdEndPoint && checkLeft23 == 0; j++)
			{
				if (i == 0 && j == 0) continue;
				if (abs(leftLowerEndPoint[i*2] - leftThirdEndPoint[j*2]) <= standard)
				{
					if (abs(leftLowerEndPoint[i*2 + 1] - leftThirdEndPoint[j*2 + 1]) <= standard) checkLeft23 = 1;
				}
			}
		}
		if (checkLeft23 == 1)	//dung
		{
			int checkRight23 = 0;
			for( i = 0; i < lengthOfRightLowerEndPoint && checkRight23 == 0; i++)
			{
				for( j = 0; j < lengthOfRightThirdEndPoint && checkRight23 == 0; j++)
				{
					if (i == 0 && j == 0) continue;
					if (abs(rightLowerEndPoint[i*2] - rightThirdEndPoint[j*2]) <= standard)
					{
						if (abs(rightLowerEndPoint[i*2 + 1] - rightThirdEndPoint[j*2 + 1]) <= standard) checkRight23 = 1;
					}
				}
			}
			if (checkRight23 == 1) return checkRight23;
		}
	}
	return 0;
}

int holeDetection2(int col, int* countTransition)
{
	*countTransition = 0;
	int height = thinningOfInput->mHeight;
	int width = thinningOfInput->mWidth;
	Image* thinningOfInpu = (Image*)malloc(sizeof(Image));
	createImage(thinningOfInpu, height, width);
	copyImage(thinningOfInput, thinningOfInpu);

	unsigned char* data = thinningOfInpu->mData;
	int upperRow, lowerRow, thirdRow;
	upperRow = lowerRow = thirdRow = -1;
	int i, j;
	for (i = 0; i < height; i++)
	{
		if (data[i*width + col] == 1) 
		{
			(*countTransition)++;
			if (upperRow == -1) upperRow = i;
			else if (lowerRow == -1) lowerRow = i;
			else thirdRow = i;
			while (data[i*width + col] == 1)
			{
				i++;
			}
			i--;
		}
	}
	if (*countTransition != 1) return 0;
	//////cout<<"upperRow  "<<upperRow<<"   lowerRow"<<lowerRow<<"\n";

	//nua trai va nua phai
	int newLeftWidth = (int)(ceil(averageCharacterWidth*0.75));
	int newRightWidth = (int)(ceil(averageCharacterWidth*0.75));
	if (newLeftWidth > col + 1) newLeftWidth = col + 1;
	if (newRightWidth > input->mWidth -1 - col) newRightWidth = input->mWidth - col - 1;

	int leftOffset = col - newLeftWidth + 1;
	Image* leftImage = (Image*)malloc(sizeof(Image));
	createImage(leftImage, input->mHeight, newLeftWidth);
	for (i = 0; i < input->mHeight; i++)
		for (j = 0; j < newLeftWidth; j++)
		{
			leftImage->mData[i*newLeftWidth + j] = thinningOfInpu->mData[i*input->mWidth + j + leftOffset];
		}
	int lengthOfLeftUpperEndPoint = 0;
	int* leftUpperEndPoint = endLeftContour(upperRow, &lengthOfLeftUpperEndPoint, leftImage, 1);
	

	//right image
	int rightOffset = col;
	Image* rightImage = (Image*)malloc(sizeof(Image));
	createImage(rightImage, input->mHeight, newRightWidth);
	for (i = 0; i < input->mHeight; i++)
		for (j = 0; j < newRightWidth; j++)
		{
			rightImage->mData[i*newRightWidth + j] = thinningOfInpu->mData[i*input->mWidth + j + rightOffset];
		}
	int lengthOfRightUpperEndPoint = 0;
	int* rightUpperEndPoint = endRightContour(upperRow, &lengthOfRightUpperEndPoint, rightImage, 1);

	int standard = HOLE_RATE*strokeWidth;

	//xu li khi chi co 1 nhat cat
	////printImage(leftImage);
	//////cout<<"\n\n\n";
	////printImage(rightImage);
	//////cout<<"\n\n\n";
	for (i = 0; i < lengthOfLeftUpperEndPoint; i++)
		for (j = 0; j < lengthOfRightUpperEndPoint; j++)
		{
			if (i == 0 || j == 0) continue;
			if (abs(leftUpperEndPoint[i*2] - rightUpperEndPoint[j*2]) <= standard)
			{
				if (abs(leftUpperEndPoint[i*2 + 1] - (rightUpperEndPoint[j*2 + 1] + leftImage->mWidth - 1)) <= standard) 
					return 1;
			}
		}
	return 0;
}

int findingNextPoint1(int curRow, int curCol, int *nextRow, int *nextCol, Stack* stack, Image* input)
{
	unsigned char* data = input->mData;
	int width = input->mWidth;
	int height = input->mHeight;
	int result = 0;

	unsigned char* ptr = data + curRow * width;		//duyet theo con tro tung hang
	if (curCol > 0 && *(ptr + curCol - 1) == 1)		//1 diem ngay ben trai
	{
		result = 1;
		*nextRow = curRow;
		*nextCol = curCol - 1;
	}
	if (curRow > 0)
	{
		ptr = data + (curRow - 1)*width;
		if (curCol > 0)		
		{
			if (*(ptr + curCol - 1)  == 1)		//goc tren trai
			{
				if (result != 0)
				{
					pushStack(stack, curRow - 1, curCol - 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow - 1;
					*nextCol = curCol - 1;
				}
			}
		}
		if (*(ptr + curCol) == 1)    //phia tren
		{
			if (result != 0)
			{
				pushStack(stack, curRow - 1, curCol);
			}
			else
			{
				result = 1;
				*nextRow = curRow - 1;
				*nextCol = curCol;
			}
		}
		if (curCol < width - 1)    //goc tren phai
		{
			if (*(ptr + curCol + 1) == 1)
			{
				if (result != 0)
				{
					pushStack(stack, curRow - 1, curCol + 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow - 1;
					*nextCol = curCol + 1;
				}
			}
		}
	}
	ptr = data + curRow * width;
	if (curCol < width - 1 && *(ptr + curCol + 1) == 1)  //phia ben phai
	{
		if (result != 0)
		{
			pushStack(stack, curRow, curCol + 1);
		}
		else
		{
			result = 1;
			*nextRow = curRow;
			*nextCol = curCol + 1;
		}
	}
	if (curRow < height - 1)
	{
		ptr = data + (curRow + 1)*width;
		if (curCol < width - 1)  //goc duoi phai
		{
			if (*(ptr + curCol + 1) == 1)
			{
				if (result != 0)
				{
					pushStack(stack, curRow + 1, curCol + 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow + 1;
					*nextCol = curCol + 1;
				}
			}
		}
		if (*(ptr + curCol) == 1)  //phia duoi
		{
			if (result != 0)
			{
				pushStack(stack, curRow + 1, curCol);
			}
			else
			{
				result = 1;
				*nextRow = curRow + 1;
				*nextCol = curCol;
			}
		}
		if (curCol > 0)  //goc duoi trai
		{
			if (*(ptr + curCol -1) == 1) 
			{
				if (result != 0)
				{
					pushStack(stack, curRow + 1, curCol - 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow + 1;
					*nextCol = curCol - 1;
				}
			}
		}
	}
	return result;
}
void reduceStackForHoleDetection(Image* input, Stack* stack)
{
	int i,j;
	unsigned char* data = input->mData;
	int width = input->mWidth;
	for( i = 0; i < stack->n; i++)
	{
		int row = *(stack->row + i);
		int col = *(stack->col + i);
		if (*(data + row*width + col) != 1)
		{
			for( j = i; j < stack->n - 1; j++)
			{
				*(stack->row + j) = *(stack->row + j + 1);
				*(stack->col + j) = *(stack->col + j + 1);
			}
			i--;
			stack->n--;
		}
	}
}
int* endLeftContour(int row, int* lengOfResult, Image* input, int type)
{
	if (type == 0)
	{
		int* result = (int*)malloc(20*sizeof(int));  //10 phan tu
		*lengOfResult = 0;
		Stack* stack = (Stack*)malloc(sizeof(Stack));
		createStack(stack, NORMAL_STACK_MAXSIZE);
		pushStack(stack, row, input->mWidth - 1);		//gan vo col cuoi cung = 1
		result[(*lengOfResult)*2] = row;
		result[(*lengOfResult)*2 + 1] = input->mWidth - 1;
		(*lengOfResult)++;
		while(stack->n > 0)
		{
			reduceStackForHoleDetection(input, stack);
			if (stack->n <= 0) break;
			int startRow = *(stack->row + stack->n - 1);
			int startCol = *(stack->col + stack->n  - 1);
			stack->n--;
			int curRow = startRow;
			int curCol = startCol;
			int nextRow, nextCol;
			input->mData[startRow*input->mWidth + startCol] = 2;
			while (findingNextPoint1(curRow, curCol, &nextRow, &nextCol, stack, input) != 0)
			{
				curRow = nextRow;
				curCol = nextCol;
				input->mData[curRow*input->mWidth + curCol] = 2;
			}
			input->mData[curRow*input->mWidth + curCol] = 2;
			result[(*lengOfResult)*2] = curRow;
			result[(*lengOfResult)*2 + 1] = curCol;
			(*lengOfResult)++;
		}
		return result;
	}
	else
	{
		int* result = (int*)malloc(20*sizeof(int));  //10 phan tu
		(*lengOfResult) = 0;
		Stack* stack = (Stack*)malloc(sizeof(Stack));
		createStack(stack, NORMAL_STACK_MAXSIZE);
		pushStack(stack, row, input->mWidth - 1);		//gan vo col cuoi cung = 1
		result[(*lengOfResult)*2] = row;
		result[(*lengOfResult)*2 + 1] = input->mWidth - 1;
		(*lengOfResult)++;
		while(stack->n > 0)
		{
			reduceStackForHoleDetection(input, stack);
			if (stack->n <= 0) break;
			int startRow = *(stack->row + stack->n - 1);
			int startCol = *(stack->col + stack->n  - 1);
			stack->n--;
			int curRow = startRow;
			int curCol = startCol;
			int nextRow, nextCol;
			input->mData[startRow*input->mWidth + startCol] = 2;
			int check = 0;
			while (findingNextPoint1(curRow, curCol, &nextRow, &nextCol, stack, input) != 0)
			{
				if (curCol != curRow) check = 1;
				curRow = nextRow;
				curCol = nextCol;
				input->mData[curRow*input->mWidth + curCol] = 2;
			}
			if (check == 1)
			{
				input->mData[curRow*input->mWidth + curCol] = 8;
				result[(*lengOfResult)*2] = curRow;
				result[(*lengOfResult)*2] = curRow;
				result[(*lengOfResult)*2 + 1] = curCol;
				(*lengOfResult)++;
			}
		}
		return result;	
	}
}
int* endRightContour(int row, int* lengOfResult, Image* input, int type)
{
	if (type == 0)
	{
		int* result = (int*)malloc(20*sizeof(int));  //10 phan tu
		(*lengOfResult) = 0;
		Stack* stack = (Stack*)malloc(sizeof(Stack));
		createStack(stack, NORMAL_STACK_MAXSIZE);
		pushStack(stack, row, 0);		//gan vo col cuoi cung = 1
		result[(*lengOfResult)*2] = row;
		result[(*lengOfResult)*2 + 1] = 0;
		(*lengOfResult)++;
		while(stack->n > 0)
		{
			reduceStackForHoleDetection(input, stack);
			if (stack->n <= 0) break;
			int startRow = *(stack->row + stack->n - 1);
			int startCol = *(stack->col + stack->n  - 1);
			stack->n--;
			int curRow = startRow;
			int curCol = startCol;
			int nextRow, nextCol;
			input->mData[startRow*input->mWidth + startCol] = 2;
			while (findingNextPoint1(curRow, curCol, &nextRow, &nextCol, stack, input) != 0)
			{
				curRow = nextRow;
				curCol = nextCol;
				input->mData[curRow*input->mWidth + curCol] = 2;
			}
			input->mData[curRow*input->mWidth + curCol] = 2;
			result[(*lengOfResult)*2] = curRow;
			result[(*lengOfResult)*2 + 1] = curCol;
			(*lengOfResult)++;
		}
		return result;
	}
	else
	{
		int* result = (int*)malloc(20*sizeof(int));  //10 phan tu
		(*lengOfResult) = 0;
		Stack* stack = (Stack*)malloc(sizeof(Stack));
		createStack(stack, NORMAL_STACK_MAXSIZE);
		pushStack(stack, row, 0);		//gan vo col cuoi cung = 1
		result[(*lengOfResult)*2] = row;
		result[(*lengOfResult)*2 + 1] = 0;
		(*lengOfResult)++;
		while(stack->n > 0)
		{
			reduceStackForHoleDetection(input, stack);
			if (stack->n <= 0) break;
			int startRow = *(stack->row + stack->n - 1);
			int startCol = *(stack->col + stack->n  - 1);
			stack->n--;
			int curRow = startRow;
			int curCol = startCol;
			int nextRow, nextCol;
			input->mData[startRow*input->mWidth + startCol] = 2;
			int check = 0;
			while (findingNextPoint1(curRow, curCol, &nextRow, &nextCol, stack, input) != 0)
			{
				if (curCol != nextCol) check = 1;
				curRow = nextRow;
				curCol = nextCol;
				input->mData[curRow*input->mWidth + curCol] = 2;
			}
			if (check == 1)
			{
				input->mData[curRow*input->mWidth + curCol] = 8;
				result[(*lengOfResult)*2] = curRow;
				result[(*lengOfResult)*2 + 1] = curCol;
				(*lengOfResult)++;
			}
		}
		return result;
	}
	
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///			    Connected Component                     //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

int findoutConnectedComponent(Image* src, Image* des)
{
	int result = 2;
	int height = src->mHeight;
	int width = src->mWidth;
	leftStack = (Stack*)malloc(sizeof(Stack));
	createStack(leftStack, height*width);

	int i, j;
	for (j = 0; j < width; j++)
	{
		for (i = lowerBaseLine; i >= upperBaseLine; i--)
		{
			if (des->mData[i*width + j] == 1)
			{
				pushStack2(leftStack, i, j);
				while(leftStack->n > 0)
				{
					int curRow, curCol, nextRow, nextCol;
					curRow = *(leftStack->row + leftStack->n - 1);
					curCol = *(leftStack->col + leftStack->n  - 1);
					leftStack->n--;
					des->mData[curRow*des->mWidth + curCol] = result;
					while (findingNextPointForConnectedComponents(curRow, curCol, &nextRow, &nextCol, leftStack, des) != 0)
					{
						curRow = nextRow;
						curCol = nextCol;
						des->mData[curRow*width + curCol] = result;
					}
				}
				result++;
			}
		}
	}
	free(leftStack);
	return result;
}
int findingNextPointForConnectedComponents(int curRow, int curCol, int *nextRow, int *nextCol, Stack* stack, Image* input)
{
	unsigned char* data = input->mData;
	int width = input->mWidth;
	int height = input->mHeight;
	int result = 0;
	unsigned char* ptr = data + curRow * width;		//duyet theo con tro tung hang
	if (curCol > 0 && *(ptr + curCol - 1) == 1)		//1 diem ngay ben trai
	{
		result = 1;
		*nextRow = curRow;
		*nextCol = curCol - 1;
	}
	if (curRow > 0)
	{
		ptr = data + (curRow - 1)*width;
		if (curCol > 0)		
		{
			if (*(ptr + curCol - 1)  == 1)		//goc tren trai
			{
				if (result != 0)
				{
					pushStack2(stack, curRow - 1, curCol - 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow - 1;
					*nextCol = curCol - 1;
				}
			}
		}
		if (*(ptr + curCol) == 1)    //phia tren
		{
			if (result != 0)
			{
				pushStack2(stack, curRow - 1, curCol);
			}
			else
			{
				result = 1;
				*nextRow = curRow - 1;
				*nextCol = curCol;
			}
		}
		if (curCol < width - 1)    //goc tren phai
		{
			if (*(ptr + curCol + 1) == 1)
			{
				if (result != 0)
				{
					pushStack2(stack, curRow - 1, curCol + 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow - 1;
					*nextCol = curCol + 1;
				}
			}
		}
	}
	ptr = data + curRow * width;
	if (curCol < width - 1 && *(ptr + curCol + 1) == 1)  //phia ben phai
	{
		if (result != 0)
		{
			pushStack2(stack, curRow, curCol + 1);
		}
		else
		{
			result = 1;
			*nextRow = curRow;
			*nextCol = curCol + 1;
		}
	}
	if (curRow < height - 1)
	{
		ptr = data + (curRow + 1)*width;
		if (curCol < width - 1)  //goc duoi phai
		{
			if (*(ptr + curCol + 1) == 1)
			{
				if (result != 0)
				{
					pushStack2(stack, curRow + 1, curCol + 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow + 1;
					*nextCol = curCol + 1;
				}
			}
		}
		if (*(ptr + curCol) == 1)  //phia duoi
		{
			if (result != 0)
			{
				pushStack2(stack, curRow + 1, curCol);
			}
			else
			{
				result = 1;
				*nextRow = curRow + 1;
				*nextCol = curCol;
			}
		}
		if (curCol > 0)  //goc duoi trai
		{
			if (*(ptr + curCol -1) == 1) 
			{
				if (result != 0)
				{
					pushStack2(stack, curRow + 1, curCol - 1);
				}
				else
				{
					result = 1;
					*nextRow = curRow + 1;
					*nextCol = curCol - 1;
				}
			}
		}
	}
	return result;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///	      supported function for validation             //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

Image* createImageFromInput(int left, int right, int value, Image* src)
{
	int width = right - left + 1;
	int height = input->mHeight;
	Image* result;
	int i, j;
	if (value == 0)
	{
		result = (Image*)malloc(sizeof(Image));
		createImage(result, height, width);
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				result->mData[i*width + j] = input->mData[i*input->mWidth + j + left];
			}
		}
	}
	else
	{
		width = src->mWidth;
		result = (Image*)malloc(sizeof(Image));
		createImage(result, height, width);
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				if (src->mData[i*width + j] == value)
				{
					result->mData[i*width + j] = 1;
				}
				else
				{
					result->mData[i*width + j] = 0;
				}
			}
		}
	}
	return result;
}
void leftRightUpBottom(Image *src, int* left, int* right, int* top, int* bottom)
{
	(*left) = -1;
	(*right) = -1;
	(*top) = -1;
	(*bottom) = -1;
	int height = src->mHeight;
	int width = src->mWidth;

	int col, row;
	//find (*left) boundary to cut here
	for (col = 0; col < width && (*left) == -1; col++)
	{
		for(row = 0 ; row < height; row++)
		{

			if (src->mData[width * row + col] == 1) {
				(*left) = col;
				break;
			}
		}
	}
	//find right boundary to cut here
	for(col = width - 1; col >= 0 && (*right) == -1; col--)
	{
		for(row = 0; row < height; row++)
		{
			if (src->mData[width * row + col] == 1) {
				(*right) = col;
				break;
			}
		}
	}
	//find top boundary to cut here
	for(row = 0; row < height && (*top) == -1; row++)
	{
		for(col = 0; col<width; col++)
		{
			if (src->mData[width * row + col] == 1)
			{
				(*top) = row;
				break;
			}
		}
	}
	//find bottom boundary to cut here
	for(row = height-1; row >= 0 && (*bottom) == -1; row--)
	{
		for(col = 0; col < width; col++)
		{
			if (src->mData[width * row + col] == 1)
			{
				(*bottom) = row;
				break;
			}
		}
	}
}
structCharacter characterRecognition(Image* src, int left, int right, int top, int bottom)
{
	structCharacter result;
	if (top < (upperBaseLine/2 + strokeWidth)&& upperBaseLine >= lowerBaseLine*0.25)  //chu cao phia tren
	{
		result = characterRecognitionGroup3(src);
	}
	else
	{
		if ((src->mHeight > lowerBaseLine + (lowerBaseLine - upperBaseLine)*0.5)&& (bottom >= lowerBaseLine + (src->mHeight - lowerBaseLine)*0.5))
		{
			result = characterRecognitionGroup2(src);
		}
		else
		{
			result = characterRecognitionGroup1(src);
		}
	}
	return result;
}

int validation(int left_1, int right_1, int left_2, int right_2)
{
	int left;         //left trai nhat
	int middleLeft;   //left cua ki tu ben phai
	int middleRight;  //right cua ki tu ben trai
	int right;        //right phai nhat
	int leftFlag;
	int middleLeftFlag;
	int middleRightFlag;
	int rightFlag;
	if (left_1 == -1 && right_2 != -1)
	{
		left = 0;
		middleLeft = rightSeg->arr[left_2].pos;
		middleRight = leftSeg->arr[right_1].pos;
		right = leftSeg->arr[right_2].pos;
		leftFlag = 3;
		middleLeftFlag = rightSeg->arr[left_2].flag;
		middleRightFlag = leftSeg->arr[right_1].flag;
		rightFlag = leftSeg->arr[right_2].flag;
	}
	else if (left_1 == -1 && right_2 == -1)
	{
		left = 0;
		middleLeft = rightSeg->arr[left_2].pos;
		middleRight = leftSeg->arr[right_1].pos;
		right = input->mWidth - 1;
		leftFlag = 3;
		middleLeftFlag = rightSeg->arr[left_2].flag;
		middleRightFlag = leftSeg->arr[right_1].flag;
		rightFlag = 3;
	}
	else if (right_2 == -1)
	{
		left = rightSeg->arr[left_1].pos;
		middleLeft = rightSeg->arr[left_2].pos;
		middleRight = leftSeg->arr[right_1].pos;
		right = input->mWidth - 1;
		leftFlag = rightSeg->arr[left_1].flag;
		middleLeftFlag = rightSeg->arr[left_2].flag;
		middleRightFlag = leftSeg->arr[right_1].flag;
		rightFlag = 3;
	}
	else
	{
		left = rightSeg->arr[left_1].pos;
		middleLeft = rightSeg->arr[left_2].pos;
		middleRight = leftSeg->arr[right_1].pos;
		right = leftSeg->arr[right_2].pos;
		leftFlag = rightSeg->arr[left_1].flag;
		middleLeftFlag = rightSeg->arr[left_2].flag;
		middleRightFlag = leftSeg->arr[right_1].flag;
		rightFlag = leftSeg->arr[right_2].flag;
	}

	Image* leftImage = createImageFromInput(left, middleRight, 0 , NULL);
	Image* rightImage = createImageFromInput(middleLeft, right, 0 , NULL);
	Image* jointImage = createImageFromInput(left, right, 0 , NULL);

	Image* componentLeftImage = (Image*)malloc(sizeof(Image));
	createImage(componentLeftImage, leftImage->mHeight, leftImage->mWidth);
	copyImage(leftImage, componentLeftImage);
	int numOfLeftComponent = findoutConnectedComponent(leftImage, componentLeftImage);

	Image* componentRightImage = (Image*)malloc(sizeof(Image));
	createImage(componentRightImage, rightImage->mHeight, rightImage->mWidth);
	copyImage(rightImage, componentRightImage);
	int numOfRightComponent = findoutConnectedComponent(rightImage, componentRightImage);

	Image* componentJointImage = (Image*)malloc(sizeof(Image));
	createImage(componentJointImage, jointImage->mHeight, jointImage->mWidth);
	copyImage(jointImage, componentJointImage);
	int numOfJointComponent = findoutConnectedComponent(jointImage, componentJointImage);

	structCharacter leftCharacterCandidate;
	structCharacter rightCharacterCandidate;
	structCharacter jointCharacterCandidate_Left;     //dung cho truong hop tach roi
	structCharacter jointCharacterCandidate_Right;    //dung cho truong hop tach roi
	structCharacter jointCharacterCandidate_Connect;  //dung cho truong hop ko tach roi
	int countOfLeftCharacterCandidate = 0;
	int countOfRightCharacterCandidate = 0;
	int countOfJointCharacterCandidate = 0;
	boundary leftCharacterCandidateBoundary;
	boundary rightCharacterCandidateBoundary;
	boundary jointCharacterCandidateBoundary_Left;
	boundary jointCharacterCandidateBoundary_Right;
	boundary jointCharacterCandidateBoundary_Connect;

	//leftImage->//printImage();
	////printImage(componentLeftImage);
	int i,j;
	for( i = 2; i < numOfLeftComponent; i++)
	{
		int left, right, top, bottom;
		Image* tempImage = createImageFromInput(0, 0, i, componentLeftImage);
		leftRightUpBottom(tempImage, &left, &right, &top, &bottom);
		if ((bottom < lowerBaseLine && top > upperBaseLine && bottom - top <= 0.4*(lowerBaseLine - upperBaseLine)) ||
			(bottom >= lowerBaseLine && top > upperBaseLine && lowerBaseLine - top <= 0.4*(lowerBaseLine - upperBaseLine)) ||
			(bottom < lowerBaseLine && top <= upperBaseLine && bottom - upperBaseLine <= 0.4*(lowerBaseLine - upperBaseLine)))		//check neu ko phai la tu
		{
			////cout<<"left image at component "<<i<<" no need to be checked\n";
		}
		else
		{
			if (countOfLeftCharacterCandidate == 1) return MY_ERROR;
			leftCharacterCandidate = characterRecognition(tempImage, left, right, top, bottom);
			leftCharacterCandidateBoundary.left = left;
			leftCharacterCandidateBoundary.right = right;
			leftCharacterCandidateBoundary.top = top;
			leftCharacterCandidateBoundary.bottom = bottom;
			countOfLeftCharacterCandidate++;
			////cout<<"left image at component "<<i<<" : "<<leftCharacterCandidate.character<<"\n";
		}
	}

	////cout<<"\n";
	//rightImage->//printImage();
	////printImage(componentRightImage);
	for( i = 2; i < numOfRightComponent; i++)
	{
		int left, right, top, bottom;
		Image* tempImage = createImageFromInput(0, 0, i, componentRightImage);
		leftRightUpBottom(tempImage, &left, &right, &top, &bottom);
		if ((bottom < lowerBaseLine && top > upperBaseLine && bottom - top <= 0.4*(lowerBaseLine - upperBaseLine)) ||
			(bottom >= lowerBaseLine && top > upperBaseLine && lowerBaseLine - top <= 0.4*(lowerBaseLine - upperBaseLine)) ||
			(bottom < lowerBaseLine && top <= upperBaseLine && bottom - upperBaseLine <= 0.4*(lowerBaseLine - upperBaseLine)))
		{
			////cout<<"right image at component "<<i<<" no need to be checked\n";
		}
		else
		{
			if (countOfRightCharacterCandidate == 1) return MY_ERROR;
			rightCharacterCandidate = characterRecognition(tempImage, left, right, top, bottom);
			rightCharacterCandidateBoundary.left = left;
			rightCharacterCandidateBoundary.right = right;
			rightCharacterCandidateBoundary.top = top;
			rightCharacterCandidateBoundary.bottom = bottom;
			countOfRightCharacterCandidate++;
			////cout<<"right image at component "<<i<<" : "<<rightCharacterCandidate.character<<"\n";
		}
	}

	////cout<<"\n";
	//jointImage->//printImage();
	////printImage(componentJointImage);
	for( i = 2; i < numOfJointComponent; i++)
	{
		int left, right, top, bottom;
		Image* tempImage = createImageFromInput(0, 0, i, componentJointImage);
		leftRightUpBottom(tempImage, &left, &right, &top, &bottom);

		if ((bottom < lowerBaseLine && top > upperBaseLine && bottom - top <= 0.4*(lowerBaseLine - upperBaseLine)) ||
			(bottom >= lowerBaseLine && top > upperBaseLine && lowerBaseLine - top <= 0.4*(lowerBaseLine - upperBaseLine)) ||
			(bottom < lowerBaseLine && top <= upperBaseLine && bottom - upperBaseLine <= 0.4*(lowerBaseLine - upperBaseLine)))
		{
			////cout<<"joint image at component "<<i<<" no need to be checked\n";
		}
		else
		{   
			if (countOfJointCharacterCandidate > 1) return MY_ERROR;
			if (countOfJointCharacterCandidate == 0)
			{
				jointCharacterCandidate_Left = characterRecognition(tempImage, left, right, top, bottom);
				jointCharacterCandidateBoundary_Left.left = left;
				jointCharacterCandidateBoundary_Left.right = right;
				jointCharacterCandidateBoundary_Left.top = top;
				jointCharacterCandidateBoundary_Left.bottom = bottom;
				countOfJointCharacterCandidate++;
				////cout<<"joint image at component "<<i<<" : "<<jointCharacterCandidate_Left.character<<"\n";
			}
			else
			{
				jointCharacterCandidate_Right = characterRecognition(tempImage, left, right, top, bottom);
				jointCharacterCandidateBoundary_Right.left = left;
				jointCharacterCandidateBoundary_Right.right = right;
				jointCharacterCandidateBoundary_Right.top = top;
				jointCharacterCandidateBoundary_Right.bottom = bottom;
				countOfJointCharacterCandidate++;
				////cout<<"joint image at component "<<i<<" : "<<jointCharacterCandidate_Right.character<<"\n";
			}	
		}
		//delete tempImage;
	}
	if (countOfJointCharacterCandidate == 2 && jointCharacterCandidateBoundary_Right.left < jointCharacterCandidateBoundary_Left.left) return MY_ERROR;
	if (countOfLeftCharacterCandidate == 0) return 1;
	if (countOfRightCharacterCandidate == 0) return 1;
	if (countOfRightCharacterCandidate == 1)
	{
		jointCharacterCandidate_Connect = jointCharacterCandidate_Left;
		jointCharacterCandidateBoundary_Connect = jointCharacterCandidateBoundary_Left;
	}

							///////////////////////////////////////////////////////////	
							///////////////////////////////////////////////////////////
							//					VALIDATION HERE						 //
							///////////////////////////////////////////////////////////
	
	if (countOfJointCharacterCandidate == countOfLeftCharacterCandidate + countOfRightCharacterCandidate) //co 1 duong cat trang o giua, ko can xet nhung thu khac
	{
		int checkLeft = 1;	
		int checkRight = 1;
		int checkJoint_Left = 1;
		int checkJoint_Right = 1;
 
		if (leftCharacterCandidate.maxValue < STANDARD_MAX) checkLeft = 0;
		if (rightCharacterCandidate.maxValue < STANDARD_MAX) checkRight = 0;
		if (jointCharacterCandidate_Left.maxValue < STANDARD_MAX) checkJoint_Left = 0;
		if (jointCharacterCandidate_Right.maxValue < STANDARD_MAX) checkJoint_Right = 0;
		
		//kiem tra
		if (checkLeft == 1 && checkJoint_Left == 1)
		{
			if (leftCharacterCandidate.character == jointCharacterCandidate_Left.character)
			{
				int check = checkingOneCharacter(leftCharacterCandidate.character);   //du la 0 hay 1 cung add vao
				addCharacter(leftCharacterCandidate, check, jointCharacterCandidate_Left);
			}
			else
			{
				int check = checkingTwoCharacter(leftCharacterCandidate.character, jointCharacterCandidate_Left.character);
				if (check == 3 || check == 2) 
				{
					addCharacter(jointCharacterCandidate_Left, check, leftCharacterCandidate);
				}
				else addCharacter(leftCharacterCandidate, check, jointCharacterCandidate_Left);
			}
		}
		else if (checkLeft == 1 && checkJoint_Left == 0)   
		{
			int check = checkingTwoCharacter(leftCharacterCandidate.character, jointCharacterCandidate_Left.character);
			if (check == 3 || check == 2) 
			{
				addCharacter(jointCharacterCandidate_Left, check, leftCharacterCandidate);
			}
			else addCharacter(leftCharacterCandidate, check, jointCharacterCandidate_Left);
		}
		else if (checkLeft == 0 && checkJoint_Left == 1)   //uu tien chon joint
		{
			int check = checkingOneCharacter(jointCharacterCandidate_Left.character);   //du la 0 hay 1 cung add vao
			addCharacter(jointCharacterCandidate_Left, check, leftCharacterCandidate);
		}
		else   //uu tien chon joint neu maxValue > 0.5 or next segpoint == 3, uu tien right
		{
			if (rightSeg->arr[left_2].flag == 3 || jointCharacterCandidate_Left.maxValue > leftCharacterCandidate.maxValue)
			{
				int check = checkingOneCharacter(jointCharacterCandidate_Left.character);   //du la 0 hay 1 cung add vao
				addCharacter(jointCharacterCandidate_Left, check, leftCharacterCandidate);
			}
			else
			{
				int check = checkingOneCharacter(leftCharacterCandidate.character);   //lay left
				addCharacter(leftCharacterCandidate, check, jointCharacterCandidate_Left);
			}
		}
		return 2;
	}
	else		//neu ca 2 segment co gan voi nhau voi nhau 
	{
		jointCharacterCandidate_Connect = jointCharacterCandidate_Left;
		jointCharacterCandidateBoundary_Connect = jointCharacterCandidateBoundary_Left;

		int checkLeft = 1;	
		int checkRight = 1;
		int checkJoint = 1;
		
		char leftCha = leftCharacterCandidate.character;
		char rightCha = rightCharacterCandidate.character;
		char jointCha = jointCharacterCandidate_Connect.character;

		if (leftCharacterCandidate.maxValue < STANDARD_MAX) checkLeft = 0;
		if (rightCharacterCandidate.maxValue < STANDARD_MAX) checkRight = 0;
		if (jointCharacterCandidate_Connect.maxValue < STANDARD_MAX) checkJoint = 0;

		int checking_i_left = 0;
		int checking_i_right = 0;
		//int checkingPlausible = checkPlausible(leftCha, rightCha, jointCha, &checkLeft, &checkRight, &checkJoint);
		int checkingPlausible = checkPlausible(leftCharacterCandidate, rightCharacterCandidate, jointCharacterCandidate_Connect, &checkLeft, &checkRight, &checkJoint);
		if (checkingPlausible == 2)
		{
			if (leftCha == 'i')
			{
				int value = findoutConnectedComponent2(left, middleRight);
				if (value > 3) //ton tai i
				{
					checking_i_left = 1;
					addCharacter(leftCharacterCandidate, checkingOneCharacter(leftCha), jointCharacterCandidate_Connect);
					return 2;
				}
				else
				{
					////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
					////cout<<"This is return in CASE 000000000000000000000000 and 1111111111111111 return\n";
					return 1;
				}
			}
			if (rightCha == 'i')
			{
				int value = findoutConnectedComponent2(left, middleRight);
				if (value > 3) //ton tai i
				{
					checking_i_right = 1;
				}
				else
				{
					////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
					////cout<<"This is return in CASE 0000000000000000000000000000000 and 222222222222222222222222 return\n";
					return 1;
				}
			}
		}
		//kiem tra
		if (checkLeft == 1)  //trai co tu 
		{
			if (checkRight == 1) //phai cung co tu
			{
				if (checkJoint == 1)    ///////////////////////////////////     CASE 1
				{
					int checking_dictionary = checkingThreeCharacter(leftCha, rightCha, jointCha);
					if (checking_dictionary == 3)  //ca 2 deu dung
					{
						////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
						////cout<<"This is return in CASE 111111111111111111111111111 and 1111111111111111 return\n";
						return 1;
					}
					else if (checking_dictionary == 2)//co minh joint dung
					{
						////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
						////cout<<"This is return in CASE 111111111111111111111111111 and 22222222222222222222 return\n";
						return 1;
					}
					else if (checking_dictionary == 1)//co minh left dung
					{
						addCharacter(leftCharacterCandidate, checkingOneCharacter(leftCha), jointCharacterCandidate_Connect);
						////cout<<"This is return in CASE 111111111111111111111111111 and 333333333333333333333 return\n";
						return 2;
					}
					else  //ca 2 deu sai
					{
						if (rightFlag == 3)  //ke tiep la net dut
						{
							////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
							////cout<<"This is return in CASE 111111111111111111111111111 and 44444444444444444444444 return\n";
							return 1;
						}
						else  //ke tiep ko phai net dut
						{
							addCharacter(leftCharacterCandidate, checkingOneCharacter(leftCha), jointCharacterCandidate_Connect);
							////cout<<"This is return in CASE 111111111111111111111111111 and 555555555555555555555555 return\n";
							return 2;
						}
					}
				}
				else	///////////////////////////////////     CASE 2 
				{
					addCharacter(leftCharacterCandidate, checkingOneCharacter(leftCha), jointCharacterCandidate_Connect);
					////cout<<"This is return in CASE 222222222222222222222222 and 1111111111111111 return\n";
					return 2;
				}
			}
			else   //phai ko co tu
			{
				if (checkJoint == 1) ///////////////////////////////////     CASE 3
				{
					////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
					////cout<<"This is return in CASE 3333333333333333333333 and 1111111111111111 return\n";
					return 1;
				}
				else	///////////////////////////////////     CASE 4
				{
					int checking_left = checkingOneCharacter(leftCha);
					if (checking_left = 1)
					{
						addCharacter(leftCharacterCandidate, checking_left, jointCharacterCandidate_Connect);
						////cout<<"This is return in CASE 4444444444444444444444 and 1111111111111111 return\n";
						return 2;
					}
					else
					{
						int checking_joint = checkingThreeCharacter(leftCha, rightCha, jointCha);
						if (checking_joint == 2 || checking_joint == 3)
						{
							////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
							////cout<<"This is return in CASE 4444444444444444444444 and 222222222222222222 return\n";
							return 1;
						}
						else
						{
							addCharacter(leftCharacterCandidate, checking_left, jointCharacterCandidate_Connect);
							////cout<<"This is return in CASE 4444444444444444444444 and 3333333333333333333 return\n";
							return 2;
						}
					}
				}
			}
		}
		else   //neu trai ko co tu
		{
			if (checkRight == 1) //phai co tu
			{
				if (checkJoint == 1)///////////////////////////////////     CASE 5
				{
					////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
					////cout<<"This is return in CASE 555555555555555555 and 1111111111111111 return\n";
					return 1;
				}
				else	///////////////////////////////////     CASE 6
				{
					int checking_joint = checkingThreeCharacter(leftCha, rightCha, jointCha);
					if (checking_joint == 2 || checking_joint == 3 || rightFlag == 3)
					{
						////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
						////cout<<"This is return in CASE 66666666666666666666 and 1111111111111111 return\n";
						return 1;
					}
					else
					{
						addCharacter(leftCharacterCandidate, checkingOneCharacter(leftCha), jointCharacterCandidate_Connect);
						////cout<<"This is return in CASE 66666666666666666666 and 2222222222222222222 return\n";
						return 2;
					}
					////cout<<"This is return in CASE 6 and FIRST return\n";
				}
			}
			else  //phai ko co chu~                               
			{
				if (checkJoint == 1) ///////////////////////////////////     CASE 7
				{
					////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
					////cout<<"This is return in CASE 777777777777777 and 1111111111111111 return\n";
					return 1;
				}
				else	///////////////////////////////////     CASE 8
				{
					int checking_joint = checkingThreeCharacter(leftCha, rightCha, jointCha);
					if (checking_joint == 2 || checking_joint == 3 || rightFlag == 3)
					{
						////cout<<"WE WANT TO JOINT THIS SEGMENT\n";
						////cout<<"This is return in CASE 8888888888888888888 and 1111111111111111 return\n";
						return 1;
					}
					else
					{
						addCharacter(leftCharacterCandidate, checkingOneCharacter(leftCha), jointCharacterCandidate_Connect);
						////cout<<"This is return in CASE 8888888888888888888 and 22222222222222222 return\n";
						return 2;
					}
				}
			}
		}
	}
}

int recursiveValidation()
{
	int check = 2;
	int i,j;
	for( i = -1; i < leftSeg->real_length - 1 && check != 0; i++)
	{
		if (i == -1 && leftSeg->real_length > 1)
		{
			check = validation(i, i + 1, i + 1, i + 2);
			if (check == MY_ERROR)
			{
				return MY_ERROR;
			}
			else if (check == 1)
			{
				remove_array_element(leftSeg, i + 1);
				remove_array_element(rightSeg, i + 1);
				i--;
			}
		}
		else if (i == leftSeg->real_length - 2 || leftSeg->real_length == 1)
		{
			check = validation(i, i + 1, i + 1, -1);
			if (check == MY_ERROR)
			{
				return MY_ERROR;
			}
			else if (check == 1)
			{
				remove_array_element(leftSeg, i+1);
				remove_array_element(rightSeg, i+1);
				i--;
			}
		}
		else
		{
			check = validation(i, i + 1, i + 1, i + 2);
			if (check == MY_ERROR)
			{
				return MY_ERROR;
			}
			else if (check == 1)  
			{
				remove_array_element(leftSeg, i+1);
				remove_array_element(rightSeg, i+1);
				i--;
			}
		}
	}
	Image* endImage = createImageFromInput(rightSeg->arr[rightSeg->real_length-1].pos, input->mWidth, 0, NULL);
	Image* componentEndImage = (Image*)malloc(sizeof(Image));
	createImage(componentEndImage, endImage->mHeight, endImage->mWidth);
	copyImage(endImage, componentEndImage);
	int numOfEndComponent = findoutConnectedComponent(endImage, componentEndImage);
	int countOfEndCharacterCandidate = 0;
	structCharacter endCharacterCandidate;
	boundary endCharacterCandidateBoundary;
	for( i = 2; i < numOfEndComponent; i++)
	{
		int left, right, top, bottom;
		Image* tempImage = createImageFromInput(0, 0, i, componentEndImage);
		leftRightUpBottom(tempImage, &left, &right, &top, &bottom);
		if ((bottom < lowerBaseLine && top > upperBaseLine && bottom - top <= 0.4*(lowerBaseLine - upperBaseLine)) ||
			(bottom >= lowerBaseLine && top > upperBaseLine && lowerBaseLine - top <= 0.4*(lowerBaseLine - upperBaseLine)) ||
			(bottom < lowerBaseLine && top <= upperBaseLine && bottom - upperBaseLine <= 0.4*(lowerBaseLine - upperBaseLine)))
		{
			////cout<<"left image at component "<<i<<" no need to be checked\n";
		}
		else
		{
			if (countOfEndCharacterCandidate > 0) return MY_ERROR;
			endCharacterCandidate = characterRecognition(tempImage, left, right, top, bottom);
			endCharacterCandidateBoundary.left = left;
			endCharacterCandidateBoundary.right = right;
			endCharacterCandidateBoundary.top = top;
			endCharacterCandidateBoundary.bottom = bottom;
			countOfEndCharacterCandidate++;
			////cout<<"left image at component "<<i<<" : "<<endCharacterCandidate.character<<"\n";
		}
	}
	structCharacter end;
	end.character = '[';
	end.maxValue = 0;
	addCharacter(endCharacterCandidate, checkingOneCharacter(endCharacterCandidate.character), end);
	Image* tmpShow = (Image*)malloc(sizeof(Image));
	createImage(tmpShow, input->mHeight, input->mWidth);
	copyImage(input, tmpShow);
	for (i = 0; i < leftSeg->real_length; i++)
	{
		add_verticalLine(tmpShow, leftSeg->arr[i].pos);
	}
	//showImage("tesssssssssssssssssssssssssss", tmpShow);
	return 1;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///	                    ADD IN                          //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
void removeFaultSegment(seg_point_arr* seg)
{
	int i,j;
	for( i = 0; i < seg->real_length; i++)
	{
		if (i < seg->real_length - 1)
		{
			if (checkingFaultSegment(seg->arr[i].pos) == 1 && (checkingFaultSegment(seg->arr[i+1].pos) == 1))
			{
				remove_array_element(seg, i);
				i--;
			}
			else if (checkingFaultSegment(seg->arr[i].pos) == 1)
			{
				remove_array_element(seg, i);
				//i;
			}
		}
		else if (i == seg->real_length - 1)
		{
			if (checkingFaultSegment(seg->arr[i].pos) == 1)
			remove_array_element(seg, i);
			break;
		}
	}
}

int checkingFaultSegment(int col)
{
	int i,j;
	int count = 0;
	for( j = upperBaseLine - 2*strokeWidth; j <= lowerBaseLine + 2*strokeWidth; j++)
	{
		if (input->mData[j*input->mWidth + col] == 1)
		{
			count++;
			while (input->mData[j*input->mWidth + col] == 1)j++;
			j--;
		}
	}
	if (count >= 2) return 1;
	else return 0;
}

int checkPlausible(structCharacter leftCha, structCharacter rightCha, structCharacter jointCha, int* checkLeft, int* checkRight, int* checkJoint)
{
	char left = leftCha.character;
	char right = rightCha.character;
	char joint = jointCha.character;
	//dinh toi i => return 2 o ngoai, neu dung la i  thi *checkJoint = 0; neu ko thi 2 cai con lai = 0;
	if (((left == 'a' || left == 'b' || left == 'c' || left == 'd' || left == 'e' || left == 'g' ||left == 'd' ||
		left == 'o' || left == 'p' || left == 'q' || left == 's' || left == 'x' || left == 'z')&&leftCha.maxValue >= 0.5) || 
		  ((right == 'a' || right == 'b' || right == 'c' || right == 'd' || right == 'e' || right == 'g' ||right == 'd' ||
		  right == 'o' || right == 'p' || right == 'q' || right == 's' || right == 'x') && rightCha.maxValue >= 0.5))
	{
		*checkJoint = 0;
		return 1;
	}
	else if ((left == 'v' && right == 'v' && joint == 'm') ||
			(left == 'v' && right == 'v' && joint == 'w') ||
			(left == 'n' && right == 'n' && joint == 'm') ||
			(left == 'n' && right == 'n' && joint == 'w') ||
			(left == 'n' && right == 'v' && joint == 'm') ||
			(left == 'n' && right == 'v' && joint == 'w') ||
			(left == 'v' && right == 'n' && joint == 'm') ||
			(left == 'v' && right == 'n' && joint == 'w') ||
			(left == 'r' && right == 'i' && joint == 'n') ||
			(left == 'i' && right == 'i' && joint == 'u') ||
			(left == 'i' && right == 'j' && joint == 'y'))
	{
		*checkLeft = 0;
		*checkRight = 0;
		return 1;
	}
	else if ((left == 'k' && right == 'i' && joint == 'h') ||   // k i h
			(left == 'f' && right == 'i' && joint == 'h') ||   
			(left == 'l' && right == 'i' && joint == 'h') ||
			(left == 'w' && right == 'i' && joint == 'm') ||
			(left == 'i' && right == 'w' && joint == 'm') ||
			(left == 'w' && right == 'i' && joint == 'w') ||
			(left == 'i' && right == 'w' && joint == 'w') ||
			(left == 'n' && right == 'i' && joint == 'm') ||
			(left == 'i' && right == 'n' && joint == 'm') ||
			(left == 'i' && right == 'u' && joint == 'w') ||
			(left == 'i' && right == 'v' && joint == 'w') ||
			(left == 'v' && right == 'i' && joint == 'w') ||
			(left == 'i' && right == 'v' && joint == 'm') ||
			(left == 'v' && right == 'i' && joint == 'm') ||
			(left == 'i' && right == 'v' && joint == 'n') ||
			(left == 'v' && right == 'i' && joint == 'n') ||
			(left == 'n' && right == 'i' && joint == 'w') ||
			(left == 'i' && right == 'n' && joint == 'w') ||
			(left == 'm' && right == 'i' && joint == 'm') ||
			(left == 'i' && right == 'm' && joint == 'm') ||
			(left == 'u' && right == 'i' && joint == 'w') )
	{
		return 2;	//check lai i
	}
	else if (left == 'i' || right == 'i') return 2;
	return 0;
}

int findoutConnectedComponent2(int left, int right)
{
	Image* des = createImageFromInput(left, right, 0, NULL);
	int result = 2;
	int height = des->mHeight;
	int width = des->mWidth;
	leftStack = (Stack*)malloc(sizeof(Stack));
	createStack(leftStack, height*width);

	int i, j;
	for (j = 0; j < width; j++)
	{
		for (i = height - 1; i >= 0; i--)
		{
			if (des->mData[i*width + j] == 1)
			{
				pushStack2(leftStack, i, j);
				while(leftStack->n > 0)
				{
					int curRow, curCol, nextRow, nextCol;
					curRow = *(leftStack->row + leftStack->n - 1);
					curCol = *(leftStack->col + leftStack->n  - 1);
					leftStack->n--;
					des->mData[curRow*des->mWidth + curCol] = result;
					while (findingNextPointForConnectedComponents(curRow, curCol, &nextRow, &nextCol, leftStack, des) != 0)
					{
						curRow = nextRow;
						curCol = nextCol;
						des->mData[curRow*width + curCol] = result;
					}
				}
				result++;
			}
		}
	}
	free(leftStack);
	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///              BaseLine2 Detection                    //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void baseline2()
{
	/*Image* tmpShowImage = (Image*)malloc(sizeof(Image));
	createImage(tmpShowImage, input->mHeight, input->mWidth);
	copyImage(input, tmpShowImage);*/

	lowerBaseLine = -1;
	upperBaseLine = -1;
	int height = input->mHeight;
	int width = input->mWidth;
	
	
	//loai bo i va nhung cham nho
	Image* src = (Image*)malloc(sizeof(Image));
	createImage(src, input->mHeight, input->mWidth);
	copyImage(input, src);
	Image* comImage = (Image*)malloc(sizeof(Image));
	createImage(comImage, input->mHeight, input->mWidth);
	copyImage(input, comImage);
	int i;
	int j, k;
	int numConnectedComponent = findConnectedComponent(input, comImage);

	//loai bo nhung cham nho
	for( i = 2; i < numConnectedComponent; i++)
	{
		int left, right, top, bottom;
		Image* tempImage = createImageFromInput(0, 0, i, comImage);
		leftRightUpBottom(tempImage, &left, &right, &top, &bottom);
		{
			if (right - left < 3*strokeWidth && bottom - top < 3*strokeWidth)
			{
				for (j = top; j <= bottom; j++)
					for (k = left; k <= right; k++)
					{
						if (comImage->mData[j*width + k] == i)
						{
							src->mData[j*width + k] = 0;
						}
					}
			}
		}
	}
	//tin row Densition and Transition
	rowDensity* rowDens = findingRowDensity(src);
	//tim baseline dua tren rowDens
	findBaseLine(rowDens);
	free(comImage);
	free(rowDens);
}

void findBaseLine(rowDensity* rowDens)
{

	Image* tmpImage = (Image*)malloc(sizeof(Image));
	createImage(tmpImage, input->mHeight, input->mWidth);
	copyImage(input, tmpImage);

	Image* tmpImage2 = (Image*)malloc(sizeof(Image));
	createImage(tmpImage2, input->mHeight, input->mWidth);
	copyImage(input, tmpImage2);

	Image* tmpImage3 = (Image*)malloc(sizeof(Image));
	createImage(tmpImage3, input->mHeight, input->mWidth);
	copyImage(input, tmpImage3);

	


	int maxTransition = rowDens->transition[strokeWidth];
	int maxPos;
	int maxPos_begin = -1;
	int maxPos_end = -1;
	int i, j;
	printf("%d and %d\n", maxTransition, rowDens->transition[strokeWidth]);
	for (i = strokeWidth; i < input->mHeight - strokeWidth; i++)
	{
		if (rowDens->transition[i] > maxTransition*3/2 && rowDens->transition[i + strokeWidth] > maxTransition*3/2)
		{
			printf("%d and %d\n", maxTransition, rowDens->transition[i]);
			maxTransition = rowDens->transition[i];
			maxPos_begin = -1;
		}
		if (rowDens->transition[i] >= maxTransition)// || rowDens->transition[i] == maxTransition + 1)
		{
			if (maxPos_begin == -1) 
			{
				maxPos_begin = i;
				maxPos_end= i;
			}
			else maxPos_end = i;
		}
	}
	maxPos = maxPos_begin + (maxPos_end - maxPos_begin)/2;
	add_horizalLine(tmpImage3, maxPos_begin);
	add_horizalLine(tmpImage3, maxPos_end);
	add_horizalLine(tmpImage3, maxPos);

	if (maxPos_end - maxPos_begin < input->mHeight/6)
	{
		int oldTransition = maxTransition*2/3;
		maxTransition = rowDens->transition[strokeWidth];
		maxPos_begin = -1;
		maxPos_end = -1;
		for (i = strokeWidth; i < input->mHeight - strokeWidth; i++)
		{
			if (rowDens->transition[i] > maxTransition*3/2 && rowDens->transition[i + strokeWidth] > maxTransition*3/2 && rowDens->transition[i] <= oldTransition)
			{
				printf("%d and %d\n", maxTransition, rowDens->transition[i]);
				maxTransition = rowDens->transition[i];
				maxPos_begin = -1;
			}
			if (rowDens->transition[i] >= maxTransition)// || rowDens->transition[i] == maxTransition + 1)
			{
				if (maxPos_begin == -1) 
				{
					maxPos_begin = i;
					maxPos_end= i;
				}
				else maxPos_end = i;
			}
		}
		maxPos = maxPos_begin + (maxPos_end - maxPos_begin)/2;
		add_horizalLine(tmpImage3, maxPos_begin);
		add_horizalLine(tmpImage3, maxPos_end);
		add_horizalLine(tmpImage3, maxPos);
	}

	int* transition = (int*)malloc(sizeof(int) * 50);
	int* count_Transition = (int*)malloc(sizeof(int) * 50);
	int count = 0;
	int max = 0;
	int upGradient, downGradient;
	upGradient = 0;
	downGradient = 0;
	
	//finding upGradient
	for (i = strokeWidth; i < maxPos_begin; i++)
	{
		int check = 0;
		for (j = 0; j < count; j++)
		{
			if (rowDens->transition[i] == transition[j])
			{
				count_Transition[j]++;
				check = 1;
				break;
			}
		}
		if (check == 0) 
		{
			transition[count] = rowDens->transition[i];
			count_Transition[count] = 1;
			count++;
		}
	}
	if (count > 0)
	{
		upGradient = transition[0]; 
		max = count_Transition[0];
		for (i = 1; i < count; i++)
		{
			if (count_Transition[i] > max) 
			{
				upGradient = transition[i];
				max = count_Transition[i];
			}
		}
	}
	printf("max upGradient at %d is %d\n", upGradient, max);
	
	//finding downGradient
	max = 0;
	count = 0;
	for (i = maxPos_end; i < input->mHeight - strokeWidth; i++)
	{
		int check = 0;
		for (j = 0; j < count; j++)
		{
			if (rowDens->transition[i] == transition[j])
			{
				count_Transition[j]++;
				check = 1;
				break;
			}
		}
		if (check == 0) 
		{
			transition[count] = rowDens->transition[i];
			count_Transition[count] = 1;
			count++;
		}
	}
	if (count > 0)
	{
		downGradient = transition[0]; 
		max = count_Transition[0];
		for (i = 1; i < count; i++)
		{
			if (count_Transition[i] > max) 
			{
				downGradient = transition[i];
				max = count_Transition[i];
			}
		}
	}
	printf("max downGradient at %d is %d\n", downGradient, max);

	int begin = -1;
	int end = -1;
	double upCompare, downCompare;
	upCompare = 0;
	upCompare += upGradient;
	downCompare = 0; 
	downCompare += downGradient;

	for (i = maxPos_begin; i >= strokeWidth; i--)
	{
		if (rowDens->transition[i] <= upCompare && rowDens->transition[i - (strokeWidth - 1)/2] <= rowDens->transition[i]) 
		{
			begin = i;
			break;
		}
	}
	for (i = maxPos_end; i < input->mHeight - strokeWidth; i++)
	{
		if (rowDens->transition[i] <= downCompare && rowDens->transition[i + (strokeWidth - 1)/2] <= rowDens->transition[i]) 
		{
			end = i;
			break;
		} 
	}
	if (begin == -1) begin = maxPos_begin;
	if (end == -1) end = maxPos_end;
	middleRow = begin + (end - begin)/2;
	add_horizalLine(tmpImage2, middleRow);
	add_horizalLine(tmpImage2, begin);
	add_horizalLine(tmpImage2, end);
	
	Image* tmpShowImage = (Image*)malloc(sizeof(Image));
	createImage(tmpShowImage, input->mHeight, input->mWidth);
	copyImage(input, tmpShowImage);
	

	//Tinh upperBaseLine va lowerBaseLine
	if (begin > 1*(end - begin)/2) 
	{
		upperBaseLine = begin;
	}
	else
	{
		for (i = begin; i >= 0; i--)
		{
			if (rowDens->density[i] == 0) 
			{
				upperBaseLine = i;
				break;
			}
		}
	}
	if (input->mHeight - end > 1*(end - begin)/2) 
	{
		lowerBaseLine = end;
	}
	else
	{
		for (i = end; i < input->mHeight; i++)
		{
			if (rowDens->density[i] == 0)
			{
				lowerBaseLine = i;
				break;
			}
		}
	}
	/*
	//check again
	if (lowerBaseLine - end > (end - middleRow)/2 && (input->mHeight - end > 2*(end - begin)/3))
	{
		lowerBaseLine = -1;
		int max = rowDens->transition[end + (input->mHeight - end)/2];
		for (i = end; i < input->mHeight - strokeWidth; i++)
		{
			if (rowDens->transition[i] <= max && lowerBaseLine == -1)
			{
				lowerBaseLine = i;
			}
			else if (rowDens->transition[i] > max + 1&& rowDens->transition[i + strokeWidth/2] > max + 1) 
			{
				lowerBaseLine = -1;
			}
		}
	}
	*/
	//if (begin - upperBaseLine > (middleRow - begin)) upperBaseLine = begin;
	if (lowerBaseLine == -1) lowerBaseLine = end;
	if (upperBaseLine == -1) upperBaseLine = begin;

	add_horizalLine(tmpShowImage, middleRow);
	add_horizalLine(tmpShowImage, lowerBaseLine);
	add_horizalLine(tmpShowImage, upperBaseLine);
	//showImage("lllllllllllllllll", tmpImage3);
	//showImage("0000000000000000000", tmpImage);
	//showImage("1111111111111111111", tmpImage2);
	//showImage("aaaaaaaaaaaaaaaaaaa", tmpShowImage);
}


int findConnectedComponent(Image* src, Image* des)
{
	int result = 2;
	int height = src->mHeight;
	int width = src->mWidth;
	leftStack = (Stack*)malloc(sizeof(Stack));
	createStack(leftStack, height*width);

	int i, j;
	for (j = 0; j < width; j++)
	{
		for (i = input->mHeight - 1; i >= 0; i--)
		{
			if (des->mData[i*width + j] == 1)
			{
				pushStack2(leftStack, i, j);
				while(leftStack->n > 0)
				{
					int curRow, curCol, nextRow, nextCol;
					curRow = *(leftStack->row + leftStack->n - 1);
					curCol = *(leftStack->col + leftStack->n  - 1);
					leftStack->n--;
					des->mData[curRow*des->mWidth + curCol] = result;
					while (findingNextPointForConnectedComponents(curRow, curCol, &nextRow, &nextCol, leftStack, des) != 0)
					{
						curRow = nextRow;
						curCol = nextCol;
						des->mData[curRow*width + curCol] = result;
					}
				}
				result++;
			}
		}
	}
	free(leftStack);
	return result;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///              segmentation to component              //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
int findConnectedComponent_segmentToComponent(Image* src, Image* des)
{
	int result = 2;
	int height = src->mHeight;
	int width = src->mWidth;
	leftStack = (Stack*)malloc(sizeof(Stack));
	createStack(leftStack, height*width);

	int i, j;
	for (j = 0; j < width; j++)
	{
		i = middleRow;
		{
			if (des->mData[i*width + j] == 1)
			{
				pushStack2(leftStack, i, j);
				while(leftStack->n > 0)
				{
					int curRow, curCol, nextRow, nextCol;
					curRow = *(leftStack->row + leftStack->n - 1);
					curCol = *(leftStack->col + leftStack->n  - 1);
					leftStack->n--;
					des->mData[curRow*des->mWidth + curCol] = result;
					while (findingNextPointForConnectedComponents(curRow, curCol, &nextRow, &nextCol, leftStack, des) != 0)
					{
						curRow = nextRow;
						curCol = nextCol;
						des->mData[curRow*width + curCol] = result;
					}
				}
				result++;
			}
		}
	}
	free(leftStack);
	return result;
}

Image* createImageFromInput_segmentToComponent(int left, int right, int value, Image* src)
{
	int width;
	//int width = right - left + 1;
	int height = input->mHeight;
	Image* result = (Image*)malloc(sizeof(Image));
	int i, j;
	if (value == 0)
	{
		printf("createImageFromInput_segmentToComponent area 1111111 \n");
		int width = right - left + 1;
		createImage(result, height, width);
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				result->mData[i*width + j] = src->mData[i*input->mWidth + j + left];
			}
		}
	}
	else
	{
		printf("createImageFromInput_segmentToComponent area 222222 \n");
		width = src->mWidth;
		createImage(result, height, width);
		printf("gia tri (width, height, value) = (%d,%d,%d) \n ",width , height,value);
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				if(i > height - 3 && j== width - 1)printf("vong lap (%d,%d) \n",i,j);
				if (src->mData[i*width + j] == value)
				{
					result->mData[i*width + j] = 1;
				}
				else
				{
					result->mData[i*width + j] = 0;
				}
				if(i > height - 3 && j== width - 1)printf("gia tri (j,src->mData[i*width + j]) = (%d,%d) \n",j,src->mData[i*width + j]);
				if(i > height - 3 && j== width - 1)printf("gia tri (j,result->mData[i*width + j]) = (%d,%d) \n",j,result->mData[i*width + j]);
				
			}
		}
	}
	printf("createImageFromInput_segmentToComponent area 3333333 \n");
	//printImage(result);
	return result;
}
void leftRight_segmentToComponent(Image *src, int* left, int* right)
{
	(*left) = -1;
	(*right) = -1;
	int height = src->mHeight;
	int width = src->mWidth;

	int col, row;
	//find (*left) boundary to cut here
	for (col = 0; col < width && (*left) == -1; col++)
	{
		for(row = 0 ; row < height; row++)
		{

			if (src->mData[width * row + col] == 1) {
				(*left) = col;
				break;
			}
		}
	}
	//find right boundary to cut here
	for(col = width - 1; col >= 0 && (*right) == -1; col--)
	{
		for(row = 0; row < height; row++)
		{
			if (src->mData[width * row + col] == 1) {
				(*right) = col;
				break;
			}
		}
	}
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///              Process each component                 //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void processingComponent(Image* comImage, int checkingI)
{
	int height = comImage->mHeight;
	int width = comImage->mWidth;
	int left, right, top, bottom;
	
	int i, j;
	int transitionAtMiddleRow = 0;
	int check = strokeWidth + 2;
	for (j = 0; j < width; j++)
	{
		check++;
		if (comImage->mData[middleRow * width + j] == 1)
		{
			if (check > strokeWidth/2)
			{
				transitionAtMiddleRow++;
			}
			while (comImage->mData[middleRow * width + j] == 1) j++;
			check = 1;
			j--;
		}
	}

	printf("transition here : %d\n", transitionAtMiddleRow);
	if (transitionAtMiddleRow == 1)
	{
		leftRightUpBottom_processingComponent(comImage, &left, &right, &top, &bottom);
		structCharacter  character = characterRecognition2(comImage, left, right, top, bottom, checkingI);
		addCharacter2(character);
	}
	else if (transitionAtMiddleRow == 2)
	{
		if (checkingI == 0)
		{
			leftRightUpBottom_processingComponent(comImage, &left, &right, &top, &bottom);
			structCharacter  character = characterRecognition2(comImage, left, right, top, bottom, checkingI);
			addCharacter2(character);
		}
		else
		{
			leftRightUpBottom_processingComponent(comImage, &left, &right, &top, &bottom);
			Image* newImage = createImageFromInput_segmentToComponent(left, right, 0, comImage);
			////printImage(newImage);
			processingComplexComponent(newImage);
		}
	}
	else
	{
		leftRightUpBottom_processingComponent(comImage, &left, &right, &top, &bottom);
		Image* newImage = createImageFromInput_segmentToComponent(left, right, 0, comImage);
		////printImage(newImage);
		processingComplexComponent(newImage);
	}
}

void leftRightUpBottom_processingComponent(Image *src, int* left, int* right, int* top, int* bottom)
{
	(*left) = -1;
	(*right) = -1;
	(*top) = -1;
	(*bottom) = -1;
	int height = src->mHeight;
	int width = src->mWidth;

	int col, row;
	//find (*left) boundary to cut here
	for (col = 0; col < width && (*left) == -1; col++)
	{
		for(row = 0 ; row < height; row++)
		{

			if (src->mData[width * row + col] != 0) {
				(*left) = col;
				break;
			}
		}
	}
	//find right boundary to cut here
	for(col = width - 1; col >= 0 && (*right) == -1; col--)
	{
		for(row = 0; row < height; row++)
		{
			if (src->mData[width * row + col] != 0) {
				(*right) = col;
				break;
			}
		}
	}
	//find top boundary to cut here
	for(row = 0; row < height && (*top) == -1; row++)
	{
		for(col = 0; col<width; col++)
		{
			if (src->mData[width * row + col] != 0)
			{
				(*top) = row;
				break;
			}
		}
	}
	//find bottom boundary to cut here
	for(row = height-1; row >= 0 && (*bottom) == -1; row--)
	{
		for(col = 0; col < width; col++)
		{
			if (src->mData[width * row + col] != 0)
			{
				(*bottom) = row;
				break;
			}
		}
	}
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///              Process complex component              //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
void processingComplexComponent(Image* comImage)
{
	segmentAreaArr* areaArr = overSegment_processingComplexComponent(comImage);
	validation_processingComplexComponent(comImage, areaArr);
}
segmentAreaArr* overSegment_processingComplexComponent(Image* comImage)
{
	Image* tempShowImage = (Image*)malloc(sizeof(Image));
	createImage(tempShowImage, comImage->mHeight, comImage->mWidth);
	copyImage(comImage, tempShowImage);
	Image* tempShowImage2 = (Image*)malloc(sizeof(Image));
	createImage(tempShowImage2, comImage->mHeight, comImage->mWidth);
	copyImage(comImage, tempShowImage2);
	Image* tempShowImage3 = (Image*)malloc(sizeof(Image));
	createImage(tempShowImage3, comImage->mHeight, comImage->mWidth);
	copyImage(comImage, tempShowImage3);


	
	Image* tmp = (Image*)malloc(sizeof(Image));
	createImage(tmp, comImage->mHeight, comImage->mWidth);
	copyImage(comImage, tmp);
	

	colDensity* colDens = findingColDensity(comImage, upperBaseLine, lowerBaseLine);
	double standard = 3.5 * (strokeWidth - 1);
	double standardDouble = 1.5*(strokeWidth - 1);

	int i, j, k;
	int head = -1;
	int tail = -1;
	
	segmentAreaArr* areaArr = (segmentAreaArr*)malloc(sizeof(segmentAreaArr));
	areaArr->area = (segmentArea*)malloc(sizeof(segmentArea) * 200); //khai bao mac dinh ban dau
	areaArr->length = 0;
	
	for (i = 0; i < comImage->mWidth; i++)
	{
		if ((colDens->transition[i] == 1 && colDens->density[i] < standard) || (colDens->transition[i] == 2 && colDens->density[i] < standardDouble) || (colDens->transition[i] == 0))
		{
			if (head == -1) head = i;
			else if (colDens->flag[i] != colDens->flag[i - 1])
			{
				tail = i - 1;
			}
			add_verticalLine(tempShowImage2, i);
		}
		else
		{
			if (head != -1) tail = i - 1;
		}
		if (head != -1 && tail != -1)
		{
			areaArr->area[areaArr->length].head = head;
			areaArr->area[areaArr->length].tail = tail;
			areaArr->length++;
			if (head == tail) printf("head = tail at %d \n", areaArr->length);
			head = -1;
			tail = -1;
		}
		if (head != -1 && i == comImage->mWidth - 1)
		{
			areaArr->area[areaArr->length].head = head;
			areaArr->area[areaArr->length].tail = comImage->mWidth - 1;
			areaArr->length++;
		}
	}
	for (i = 0; i < areaArr->length; i++)
	{
		add_verticalLine(tempShowImage3, areaArr->area[i].head);
		add_verticalLine(tempShowImage3, areaArr->area[i].tail);
	}
	add_horizalLine(tempShowImage3, middleRow);
	add_horizalLine(tempShowImage3, middleRow + (lowerBaseLine - middleRow)/3);
	//showImage("eeeeeeeeeee", tempShowImage3);

	int* indexToJoint = (int*)malloc(sizeof(int)*areaArr->length);
	int count = 0;
	double areaCriteria = (lowerBaseLine - upperBaseLine)*(strokeWidth-1)*2/3;
	//double areaCriteria = (lowerBaseLine - upperBaseLine)/2;

	int area;
	for (i = 0; i < areaArr->length - 1; i++)
	{
		area = 0;
		for (j = areaArr->area[i].tail; j <= areaArr->area[i+1].head; j++)
		{
			area += colDens->density[j];
		}
		if (area < areaCriteria && colDens->flag[areaArr->area[i].tail] <= colDens->flag[areaArr->area[i+1].tail]) 
		{
			indexToJoint[count] = i;
			printf("joint at %d : %d  <  %f\n", i, area, areaCriteria);
			count++;
		}
	}
	for (i = 0; i < count; i++)
	{
		jointSegment_processingComplexComponent(areaArr, indexToJoint[i]);
		for (j = i + 1; j < count; j++)
		{
			indexToJoint[j]--;
		}
	}


	for (i = 0; i < areaArr->length; i++)
	{
		int head = areaArr->area[i].head;
		int tail = areaArr->area[i].tail;
		areaArr->area[i].flagHead = colDens->flag2[head];
		areaArr->area[i].flagTail = colDens->flag2[tail];
		if (areaArr->area[i].flagHead == 2 && areaArr->area[i].flagTail == 1) areaArr->area[i].flagHead = areaArr->area[i].flagTail;
		printf("at head %d is %d\n", i, areaArr->area[i].flagHead);
	}
	for (i = 0; i < areaArr->length; i++)
	{
		if (areaArr->area[i].flagHead == 1 || areaArr->area[i].flagHead == 3)
		{
			int head = areaArr->area[i].head;
			int tail = areaArr->area[i].tail;
			int max = 0;
			int pos = areaArr->area[i].head;
			for (j = head; j < tail; j++)
			{
				for (k = lowerBaseLine; k >= upperBaseLine; k--)
				{
					if (comImage->mData[k*comImage->mWidth + j] == 1)
					{
						int end = k;
						while (comImage->mData[k*comImage->mWidth + j] == 1) k--;
						k++;
						int begin = k;
						if (begin + (end - begin)/2 >= max)
						{
							max = begin + (end - begin)/2;
							pos = j;
						}
						break;
					}
				}
			}
			//int newHead = pos + strokeWidth/2;
			int newHead = pos;
			while(newHead < areaArr->area[i].tail && newHead < pos + strokeWidth)
			{
				if (colDens->flag2[newHead] == 1 || colDens->flag2[newHead] == 3) newHead++;
				else break;
			}
			
			if (newHead >= tail)
			{
				areaArr->area[i].head = areaArr->area[i].tail - 1;
			}
			else
			{
				areaArr->area[i].head = newHead;
			}
		}
	}

	//xet tai diem dau tien
lable2:
	area = 0;
	for (i = 0; i <= areaArr->area[0].head; i++)
	{
		area += colDens->density[i];
	}
	if (area < areaCriteria && areaArr->length > 0) 
	{
		deleteSegment_processingComplexComponent(areaArr, 0);
		goto lable2;
	}
	//xet diem cuoi cung
lable3:
	area = 0;
	for (i = areaArr->area[areaArr->length - 1].tail; i <= comImage->mWidth - 1; i++)
	{
		area += colDens->density[i];
	}
	if (area < areaCriteria && areaArr->length > 0)
	{
		deleteSegment_processingComplexComponent(areaArr, areaArr->length - 1);
		goto lable3;
	}

	for (i = 0; i < areaArr->length; i++)
	{
		add_verticalLine(tempShowImage, areaArr->area[i].head);
		add_verticalLine(tempShowImage, areaArr->area[i].tail);
		printf("head at %d is %d and %d\n", i, areaArr->area[i].head, areaArr->area[i].flagHead);
		printf("tail at %d is %d and %d\n", i, areaArr->area[i].tail, areaArr->area[i].flagTail);
	}
	//showImage("bbbbbbbbbbbbbb", tempShowImage);
	//showImage("cccccccccccccc", tempShowImage2);
	return areaArr;
	
}

void jointSegment_processingComplexComponent(segmentAreaArr* areaArr, int index)
{
	if (index < areaArr->length - 1)
	{
		areaArr->area[index].tail = areaArr->area[index + 1].tail;
		int i;
		for (i = index + 1; i < areaArr->length - 1; i++)
		{
			areaArr->area[i].head = areaArr->area[i + 1].head;
			areaArr->area[i].tail = areaArr->area[i + 1].tail;
		}
		areaArr->length--;
	}
}
void deleteSegment_processingComplexComponent(segmentAreaArr* areaArr, int index)
{
	int i;
	for (i = index; i < areaArr->length - 1; i++)
	{
		areaArr->area[i].head = areaArr->area[i + 1].head;
		areaArr->area[i].tail = areaArr->area[i + 1].tail;
		areaArr->area[i].flagHead = areaArr->area[i + 1].flagHead;
		areaArr->area[i].flagTail = areaArr->area[i + 1].flagTail;
	}
	areaArr->length--;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///             Validation                              //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
int validation_processingComplexComponent(Image* comImage, segmentAreaArr* areaArr)
{
	Image* tempImage = (Image*)malloc(sizeof(Image));
	createImage(tempImage, comImage->mHeight, comImage->mWidth);
	copyImage(comImage, tempImage);
	int connectedComponent = findConnectedComponent_segmentToComponent(comImage, tempImage);
	int i, j;
	for (i = 0; i < tempImage->mHeight; i++)
	{
		for (j = 0; j < tempImage->mWidth; j++)
		{
			if (tempImage->mData[i * tempImage->mWidth + j] == 2) tempImage->mData[i * tempImage->mWidth + j] = 0;
		}
	}
	int iNum = findConnectedComponent(comImage, tempImage);
	int* iArr = (int*)malloc(sizeof(int)*(iNum-1));
	int head = -1;
	int tail = -1;
	int check = -1;
	int count = 0;
	for (i = 0; i < tempImage->mWidth; i++)
	{
		check = -1;
		for (j = 0; j < tempImage->mHeight; j++)
		{
			if (tempImage->mData[j * tempImage->mWidth + i] != 0 && head == -1)
			{
				head = i;
				break;
			}
			if (tempImage->mData[j * tempImage->mWidth + i] != 0 && head != -1)
			{
				tail = i;
				check = i;
				break;
			}
		}
		if (i == tempImage->mWidth - 1) check = -1;
		if (head != -1 && tail != -1 && check == -1)
		{
			iArr[count] = (tail - head)/2 + head;
			count++;
			head = -1;
			tail = -1;
		}
	}
	if (count == iNum - 2) 
	{
		iNum = count;
	}
	////printImage(tempImage);
	i = 0;
	while (recursiveValidation_processingComplexComponent(comImage, areaArr, i, iNum, iArr) == 0 && i < areaArr->length)
	{
		i++;
	}
	copyImage(comImage, tempImage);
	for (i = 0; i < areaArr->length; i++)
	{
		add_verticalLine(tempImage, areaArr->area[i].head);
		add_verticalLine(tempImage, areaArr->area[i].tail);
	}
	//showImage("dddddddddddddddd", tempImage);
	for (i = 0; i <= areaArr->length; i++)
	{
		int head, tail;
		if (i == 0 &&  i == areaArr->length)
		{
			head = 0;
			tail = comImage->mWidth - 1;
		}
		else if (i == 0)
		{
			head = 0;
			tail = areaArr->area[i].head;
		}
		else if (i == areaArr->length)
		{
			head = areaArr->area[i - 1].tail;
			tail = comImage->mWidth - 1;
		}
		else
		{
			head = areaArr->area[i - 1].tail;
			tail = areaArr->area[i].head;
		}
		int leftLeft, leftRight, leftUp, leftBottom;
		Image* leftImage = createSegmentImage_processingComplexComponent(comImage, head, tail);
		//printImage(leftImage);
		leftRightUpBottom(leftImage, &leftLeft, &leftRight, &leftUp, &leftBottom);
		structCharacter leftChar = characterRecognition2(leftImage, leftLeft, leftRight, leftUp, leftBottom, 0);
		if (leftChar.character != 'i' && leftChar.character != 'j')
		{
			for (j = 0; j < iNum; j++)
			{
				if (iArr[j] > head && iArr[j] < tail)
				{
					if (leftBottom > lowerBaseLine + strokeWidth) leftChar.character = 'j';
					else leftChar.character = 'i';
					printf("replace character i here\n");
					break;
				}
			}
		}
		int checking = 1;
		//validationCharacter(leftImage, leftChar, &checking, leftLeft, leftRight);
		if (leftChar.character != '[' && checking == 1) addCharacter2(leftChar);
	}
	return 0;
}


int recursiveValidation_processingComplexComponent(Image* comImage, segmentAreaArr* areaArr, int index, int iNum, int* iArr)
{
	int i, j, k;
	if (index > areaArr->length - 1)
	{
		if (index > areaArr->length) return 0;
		int head;
		if (index == 0) head = 0; 
		else head = areaArr->area[index - 1].tail;
		int tail = comImage->mWidth - 1;
		Image* img = createSegmentImage_processingComplexComponent(comImage, head, tail);
		////printImage(img);
		int left, right, up, bottom;
		leftRightUpBottom(img, &left, &right, &up, &bottom);
		structCharacter cha = characterRecognition2(img, left, right, up, bottom, 0);
		int checking = 0;
		if (cha.maxValue > 0.5) checking = 1;
		if ((cha.character == 'i' || cha.character == 'j') && checking == 1)
		{
			checking = 0;
			for (i = 0; i < iNum; i++)
			{
				if (iArr[i] > head && iArr[i] < tail) 
				{
					checking = 1;
					break;
				}
			}
			if (checking == 0) printf("day ko phai la ki tu i\n");
		}
		if (cha.character != 'i' && cha.character != 'j')
		{
			for (i = 0; i < iNum; i++)
			{
				if (iArr[i] > head && iArr[i] < tail) 
				{
					checking = 1;
					if (bottom > lowerBaseLine + strokeWidth) cha.character = 'j';
					else cha.character = 'i';
					printf("replace character i here\n");
					break;
				}
			}
		}
		if (checking == 1) validationCharacter(img, cha, &checking, left, right);
		if (checking == 0) printf("day ko phai la ki tu\n");
		return checking;
	}
	int tail0, head1, tail1, head2;
	if (index == 0 && areaArr->length > 1)
	{
		tail0 = 0;
		head1 = areaArr->area[0].head;
		tail1 = areaArr->area[0].tail;
		head2 = areaArr->area[1].head;
	}
	else if (index == 0 && areaArr->length == 1)
	{
		tail0 = 0;
		head1 = areaArr->area[0].head;
		tail1 = areaArr->area[0].tail;
		head2 = comImage->mWidth - 1;
	}
	else if (index == areaArr->length - 1)
	{
		tail0 = areaArr->area[index - 1].tail;
		head1 = areaArr->area[index].head;
		tail1 = areaArr->area[index].tail;
		head2 = comImage->mWidth - 1;
	}
	else
	{
		tail0 = areaArr->area[index - 1].tail;
		head1 = areaArr->area[index].head;
		tail1 = areaArr->area[index].tail;
		head2 = areaArr->area[index + 1].head;
	}
	int leftLeft, leftRight, leftUp, leftBottom;
	int rightLeft, rightRight, rightUp, rightBottom;
	int jointLeft, jointRight, jointUp, jointBottom;
	Image* leftImage = createSegmentImage_processingComplexComponent(comImage, tail0, head1);
	//printImage(leftImage);
	leftRightUpBottom(leftImage, &leftLeft, &leftRight, &leftUp, &leftBottom);
	structCharacter leftChar = characterRecognition2(leftImage, leftLeft, leftRight, leftUp, leftBottom, 0);
	int checkingLeft = 0;
	if (leftChar.maxValue > 0.5) checkingLeft = 1;
	if ((leftChar.character == 'i' || leftChar.character == 'j') && checkingLeft == 1)
	{
		checkingLeft = 0;
		for (i = 0; i < iNum; i++)
		{
			if (iArr[i] > tail0 && iArr[i] < head1) 
			{
				checkingLeft = 1;
				break;
			}
		}
		if (checkingLeft == 0) printf("day ko phai la ki tu i\n");
	}
	if ((leftChar.character != 'i' && leftChar.character != 'j') || checkingLeft == 0)
	{
		for (i = 0; i < iNum; i++)
		{
			if (iArr[i] > tail0 && iArr[i] < head1) 
			{
				checkingLeft = 1;
				if (leftBottom > lowerBaseLine + strokeWidth) leftChar.character = 'j';
				else leftChar.character = 'i';
				printf("replace character i here\n");
				break;
			}
		}
	}
	if (checkingLeft == 1) validationCharacter(leftImage, leftChar, &checkingLeft, leftLeft, leftRight);
	if (checkingLeft == 0) printf("day ko phai la ki tu\n");

	Image* rightImage = createSegmentImage_processingComplexComponent(comImage, tail1, head2);
	//printImage(rightImage);
	leftRightUpBottom(rightImage, &rightLeft, &rightRight, &rightUp, &rightBottom);
	structCharacter rightChar = characterRecognition2(rightImage, rightLeft, rightRight, rightUp, rightBottom, 0);
	int checkingRight = 0;
	if (rightChar.maxValue > 0.5) checkingRight = 1;
	if ((rightChar.character == 'i' || rightChar.character == 'j')&& checkingRight == 1)
	{
		checkingRight = 0;
		for (i = 0; i < iNum; i++)
		{
			if (iArr[i] > tail1 && iArr[i] < head2) 
			{
				checkingRight = 1;
				break;
			}
		}
		if (checkingRight == 0) printf("day ko phai la ki tu i\n");
	}
	if ((rightChar.character != 'i' && rightChar.character != 'j') || checkingRight == 0)
	{
		for (i = 0; i < iNum; i++)
		{
			if (iArr[i] > tail1 && iArr[i] < head2) 
			{
				checkingRight = 1;
				if (rightBottom > lowerBaseLine + strokeWidth) rightChar.character = 'j';
				else rightChar.character = 'i';
				printf("replace character i here\n");
				break;
			}
		}
	}
	if (checkingRight == 1) validationCharacter(rightImage, rightChar, &checkingRight, rightLeft, rightRight);
	if (checkingRight == 0) printf("day ko phai la ki tu\n");
	/*
	Image* jointImage = createSegmentImage_processingComplexComponent(comImage, tail0, head2);
	//printImage(jointImage);
	leftRightUpBottom(jointImage, &jointLeft, &jointRight, &jointUp, &jointBottom);
	structCharacter jointChar = characterRecognition2(jointImage, jointLeft, jointRight, jointUp, jointBottom, 0);
	*/
	if (leftChar.character == 'l') 
		printf("");
	int ableToConnect = 0;  //0 : khong the connect ------- 1: co the connect ------- 2:chac chan connect
	if (  (leftChar.character == 'n' && areaArr->area[index].flagHead == 2) || 
		  (leftChar.character == 'l' && areaArr->area[index].flagHead == 2) || 
		  (leftChar.character == 'f' && areaArr->area[index].flagHead == 2)) 
		ableToConnect = 2;
	else if (   (leftChar.character == 'n' && rightUp >= upperBaseLine - strokeWidth && rightBottom <= lowerBaseLine + strokeWidth)|| 
				(leftChar.character == 'u' && rightUp >= upperBaseLine - strokeWidth && rightBottom <= lowerBaseLine + strokeWidth)||
				(leftChar.character == 'v' && rightUp >= upperBaseLine - strokeWidth && rightBottom <= lowerBaseLine + strokeWidth)||
				(leftChar.character == 'l' && rightUp >= upperBaseLine - strokeWidth && rightBottom <= lowerBaseLine + strokeWidth)||
				(leftChar.character == 'f' && rightUp >= upperBaseLine - strokeWidth && rightBottom <= lowerBaseLine + strokeWidth)) 
		ableToConnect = 1;

	int checkingConnection = checkingConnection_processingComplexComponent(comImage, head1, tail1);
	if (checkingConnection == 0) printf("day co the khong phai la 1 connect\n");
	else printf("day chac chan phai la 1 connect\n");

	
	
	if (leftChar.character == 'r')
	{
		printf("");
	}

	if (checkingLeft == 1 && checkingRight == 1)
	{
		return recursiveValidation_processingComplexComponent(comImage, areaArr, index + 1, iNum, iArr);
	}
	else if (checkingLeft == 1 && checkingRight == 0)
	{
		if (ableToConnect == 2) //chac chan la se connect voi ki tu truoc do
		{
			deleteSegment_processingComplexComponent(areaArr, index);
			return recursiveValidation_processingComplexComponent(comImage, areaArr, index, iNum, iArr);
		}
		else if (ableToConnect == 1) //co kha nang la ket noi hoac ko, chia 2 truong hop
		{
			if (checkingConnection == 0) //co the khong phai la 1 connection
			{
				segmentAreaArr* newAreaArr = copySegmentAreaArr(areaArr);
				int checking = recursiveValidation_processingComplexComponent(comImage, newAreaArr, index + 1, iNum, iArr);
				if (index + 1 > areaArr->length - 1) checking = 0;
				if (checking == 0) 
				{
					segmentAreaArr* newAreaArr = copySegmentAreaArr(areaArr);
					newAreaArr->area[index].head = newAreaArr->area[index].tail;
					newAreaArr->area[index].flagHead = newAreaArr->area[index].flagTail;
					int checking2 = recursiveValidation_processingComplexComponent(comImage, newAreaArr, index, iNum, iArr);
					if (index > areaArr->length - 1) checking2 = 0;
					if (checking2 == 0)
					{
						deleteSegment_processingComplexComponent(areaArr, index);
						return recursiveValidation_processingComplexComponent(comImage, areaArr, index, iNum, iArr);
					}
					else
					{
						printf("COPY COPY COPY COPY %d and %d\n", areaArr->length, newAreaArr->length);
						copySegmentAreaArr2(newAreaArr, areaArr);
					}
				}
				else
				{
					//recursiveValidation_processingComplexComponent(comImage, areaArr, index + 1, iNum, iArr);
					printf("COPY COPY COPY COPY %d and %d\n", areaArr->length, newAreaArr->length);
					//free(areaArr);
					//areaArr = copySegmentAreaArr(newAreaArr);
					copySegmentAreaArr2(newAreaArr, areaArr);
				}
			}
			else //chac chan la 1 connection 2 case : giu lai hoac xoa han di (uu tien giu lai)
			{
				segmentAreaArr* newAreaArr = copySegmentAreaArr(areaArr);
				int checking = recursiveValidation_processingComplexComponent(comImage, newAreaArr, index + 1, iNum, iArr);
				if (index + 1 > areaArr->length - 1) checking = 0;
				if (checking == 0) 
				{
					deleteSegment_processingComplexComponent(areaArr, index);
					return recursiveValidation_processingComplexComponent(comImage, areaArr, index, iNum, iArr);
				}
				else
				{
					//recursiveValidation_processingComplexComponent(comImage, areaArr, index + 1, iNum, iArr);
					printf("COPY COPY COPY COPY %d and %d\n", areaArr->length, newAreaArr->length);
					//free(areaArr);
					//areaArr = copySegmentAreaArr(newAreaArr);
					copySegmentAreaArr2(newAreaArr, areaArr);
				}
			}
		}
		else
		{
			return recursiveValidation_processingComplexComponent(comImage, areaArr, index + 1, iNum, iArr); //giu lai
		}
	}
	else if (checkingLeft == 0 && checkingRight == 1)
	{
		printf("ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR\n");
		return 0;
	}
	else
	{
		deleteSegment_processingComplexComponent(areaArr, index);
		return recursiveValidation_processingComplexComponent(comImage, areaArr, index, iNum, iArr);
	}
	return 1;
}

Image* createSegmentImage_processingComplexComponent(Image* comImage, int left, int right)
{
	Image* des = (Image*)malloc(sizeof(Image));
	createImage(des, comImage->mHeight, comImage->mWidth);
	copyImage(comImage, des);
	int i,j,k;
	for (i = lowerBaseLine + strokeWidth; i >= upperBaseLine - strokeWidth; i--)
	{
		if (i > comImage->mHeight - 1) continue;
		if (i < 0) break;
		if (des->mData[i * des->mWidth + left] == 1)
		{
			while(des->mData[i * des->mWidth + left] == 1 && i >= upperBaseLine - strokeWidth)
			{
				des->mData[i * des->mWidth + left] = 0;
				i--;
			}
			i++;
			//break;
		}
	}
	for (i = lowerBaseLine + strokeWidth; i >= upperBaseLine - strokeWidth; i--)
	{
		if (i > comImage->mHeight - 1) continue;
		if (i < 0) break;
		if (des->mData[i * des->mWidth + right] == 1)
		{
			while(des->mData[i * des->mWidth + right] == 1 && i >= upperBaseLine - strokeWidth)
			{
				des->mData[i * des->mWidth + right] = 0;
				i--;
			}
			i++;
			//break;
		}
	}
	////printImage(des);
	int result = 2;
	int height = des->mHeight;
	int width = des->mWidth;
	leftStack = (Stack*)malloc(sizeof(Stack));
	createStack(leftStack, height*width);
	for (j = left + 1; j < right; j++)
	{
		i = middleRow;
		{
			if (des->mData[i*width + j] == 1)
			{
				pushStack2(leftStack, i, j);
				while(leftStack->n > 0)
				{
					int curRow, curCol, nextRow, nextCol;
					curRow = *(leftStack->row + leftStack->n - 1);
					curCol = *(leftStack->col + leftStack->n  - 1);
					leftStack->n--;
					des->mData[curRow*des->mWidth + curCol] = result;
					while (findingNextPointForConnectedComponents(curRow, curCol, &nextRow, &nextCol, leftStack, des) != 0)
					{
						curRow = nextRow;
						curCol = nextCol;
						des->mData[curRow*width + curCol] = result;
					}
				}
				result++;
			}
		}
	}
	for (i = 0; i < des->mHeight; i++)
		for (j = 0; j < des->mWidth; j++)
			if (des->mData[i * width + j] == 1) des->mData[i * width + j] = 0;
			else if (des->mData[i * width + j] == 2) des->mData[i * width + j] = 1;
	free(leftStack);
	return des;
}

int checkingConnection_processingComplexComponent(Image* comImage, int left, int right)
{
	int i, j;
	int countTransition = 0;
	for (i = lowerBaseLine; i >= upperBaseLine; i--)
		for (j = left; j <= right; j++)
		{
			if (comImage->mData[i*comImage->mWidth + j] == 1)
			{
				countTransition++;
				break;
			}
		}
	if (countTransition > (lowerBaseLine - upperBaseLine)*2/3) return 0;
	else return 1;
}

void validationCharacter(Image* img, structCharacter character, int* checking, int left, int right)
{
	if (character.character == 'b') 
	{
		
		Image* newImage = (Image*)malloc(sizeof(Image));
		createImage(newImage, img->mHeight, img->mWidth);
		copyImage(img, newImage);
		add_horizalLine(newImage, middleRow);
		//showImage("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", newImage);
	}
	if (character.character == 'h' || character.character == 'n' || character.character == 'u' || character.character == 'y'|| character.character == 'g'
		|| character.character == 'p' || character.character == 'q' || character.character == 'b' || character.character == 'd'
		|| character.character == 'o')
	{
		int begin, end;
		*checking = 0;
		int i;
		begin = -1;
		end = -1;
		for (i = left; i <= right && (*checking) == 0; i++)
		{
			if (img->mData[middleRow * img->mWidth + i] == 1)
			{
				
				if (begin != -1) end = i;
				while (img->mData[middleRow * img->mWidth + i] == 1) i++;
				i--;
				if (begin == -1) begin = i;
				if (begin != -1 && end != -1)
				{
					if (end - begin >= strokeWidth) (*checking) = 1;
					else
					{
						begin = i;
						end = -1;
					}
				}
			}
		}
	}
	else if (character.character == 'v')
	{
		int begin, end;
		*checking = 0;
		int i;
		begin = -1;
		end = -1;
		for (i = left; i <= right && (*checking) == 0; i++)
		{
			if (img->mData[middleRow * img->mWidth + i] == 1)
			{
				if (begin != -1) end = i;
				while (img->mData[middleRow * img->mWidth + i] == 1) i++;
				i--;
				if (begin == -1) begin = i;
				if (begin != -1 && end != -1)
				{
					if (end - begin >= strokeWidth) (*checking) = 1;
					else
					{
						begin = i;
						end = -1;
					}
				}
			}
		}
	}
	else if (character.character == 'k' || character.character == 'r' || character.character == 's' || character.character == 'x' || character.character == 'z'
		 || character.character == 'c')
	{
		*checking = 0;
		if (right - left >= 4*(strokeWidth - 1)) (*checking) = 1;
	}
	else if (character.character == 'm')
	{
		int begin, end;
		*checking = 0;
		int i;
		begin = -1;
		end = -1;
		int count = 0;
		for (i = left; i <= right && (*checking) == 0; i++)
		{
			if (img->mData[middleRow * img->mWidth + i] == 1)
			{
				
				if (begin != -1) end = i;
				while (img->mData[middleRow * img->mWidth + i] == 1) i++;
				i--;
				if (begin == -1) begin = i;

				if (begin != -1 && end != -1)
				{
					if (end - begin > strokeWidth) 
					{

						count++;  //moi duyet net dau
						begin = i;
						end = -1;
					}
					else
					{
						begin = i;
						end = -1;
					}
				}
			}
		}
		if (count == 2) (*checking) = 1;
	}
	else if (character.character == 'w')
	{
		int begin, end;
		*checking = 0;
		int i;
		begin = -1;
		end = -1;
		int count = 0;
		for (i = left; i <= right && (*checking) == 0; i++)
		{
			if (img->mData[middleRow * img->mWidth + i] == 1)
			{
				if (begin != -1) end = i;
				while (img->mData[middleRow * img->mWidth + i] == 1) i++;
				i--;
				if (begin == -1) begin = i;

				if (begin != -1 && end != -1)
				{
					if (end - begin > strokeWidth) 
					{

						count++;  //moi duyet net dau
						begin = i;
						end = -1;
					}
					else
					{
						begin = i;
						end = -1;
					}
				}
			}
		}
		if (count == 2 || count == 3) (*checking) = 1;
	}
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///              SUPPORT FUNCTION                       //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
segmentAreaArr* copySegmentAreaArr(segmentAreaArr* src)
{
	segmentAreaArr* result = (segmentAreaArr*)malloc(sizeof(segmentAreaArr));
	result->length = src->length;
	result->area = (segmentArea*)malloc(sizeof(segmentArea) * result->length);
	int i;
	for (i = 0; i < result->length; i++)
	{
		result->area[i].head = src->area[i].head;
		result->area[i].tail = src->area[i].tail;
		result->area[i].flagHead = src->area[i].flagHead;
		result->area[i].flagTail = src->area[i].flagTail;
	}
	return result;
}

void copySegmentAreaArr2(segmentAreaArr* src, segmentAreaArr* des)
{
	des->length = src->length;
	int i;
	for (i = 0; i < des->length; i++)
	{
		des->area[i].head = src->area[i].head;
		des->area[i].tail = src->area[i].tail;
		des->area[i].flagHead = src->area[i].flagHead;
		des->area[i].flagTail = src->area[i].flagTail;
	}
}


void addCharacter2(structCharacter character)
{
	resultWord[countCharacter] = character.character;
	countCharacter++;
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///              MAIN FUNCTION                          //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

char* wordRecognition2(Image* inImage)
{
	input = (Image*)malloc(sizeof(Image));
	cutImage(inImage, input, 6);
	Image* tmp = (Image*)malloc(sizeof(Image));
	createImage(tmp, input->mHeight, input->mWidth);
	copyImage(input, tmp);
	thinningOfInput = (Image*)malloc(sizeof(Image));
	thinning(tmp, thinningOfInput);

	//init data
	int width = input->mWidth;
	int height = input->mHeight;
	averageCharacterWidth = 0;
	strokeWidth = 0;
	lowerBaseLine = -1;
	upperBaseLine = -1;
	upCharacter = 0;
	downCharacter = 0;

	//init dictionary
	countCharacter = 0;
	countError = 0;
	//resultWord = (char*)malloc(MAX_WORD_LENGTH); 
	headIndex = indexOfDictionary[convertCharacterToIndex1('a')][0];
	endIndex = countOfDictionary;
	ErrorCheck = 0;

	//init mVerticalDensity
	mVerticalDensity = (int*)malloc(input->mWidth*sizeof(int));
	int sum = 0;
	int i, j;
	for ( j = 0; j < width; j++)
	{
		for ( i = 0; i < height; i++)
		{
			if (input->mData[i*width + j] == 1)
			{
				sum++;
			}
		}
		mVerticalDensity[j] = sum;
		sum = 0;
	}
	slantCorrection(); 
	calculateStrokeWidth();

	printf("stroke width is %d\n", strokeWidth);
	printf("before baseline2 \n");
	baseline2();
	printf("finish baseline2 \n");
	averageCharacterWidth = (lowerBaseLine - upperBaseLine)/2;
	printf("word_recog area 111111111 \n");
	//segmentation
	Image* comImage = (Image*)malloc(sizeof(Image));
	createImage(comImage, input->mHeight, input->mWidth);
	copyImage(input, comImage);
	printf("word_recog area 222222222 \n");
	int numConnectedComponent = findConnectedComponent_segmentToComponent(input, comImage);
	////printImage(comImage);
	printf("word_recog area 3333333333 \n");
	int k;
	//Image** componentSegmentImage = (Image**)malloc(sizeof(Image*)*(numConnectedComponent - 2 - 1)); //maybe error here
	Image** componentSegmentImage = (Image**)malloc(sizeof(Image*)*(numConnectedComponent - 2)); //Haiiiiiii
	//int* checkingI = (int*)malloc(sizeof(int)*(numConnectedComponent - 2 - 1));
	int* checkingI = (int*)malloc(sizeof(int)*(numConnectedComponent - 2));
	///////
	printf("gia tri numConnectedComponent = %d \n",numConnectedComponent);
	printf("ma tran comImage->mHeight = %d, mWidth = %d  \n", comImage->mHeight, comImage->mWidth);
	//printImage(comImage);
	///////
	for (i = 2; i < numConnectedComponent; i++)
	{
		int left, right;
		if(i == 2) printf("word_recog area 333333.11111111 \n");
		//Image* tmp = (Image*)malloc(sizeof(Image));
		if(i == 2) printf("word_recog area 333333.AAAAAAAA \n");
		componentSegmentImage[i - 2] = (Image*) malloc(sizeof(Image));
		if(i == 2) printf("word_recog area 333333.BBBBBBBB \n");
		componentSegmentImage[i - 2] = createImageFromInput_segmentToComponent(0, 0, i, comImage);
		//createImageFromInput_segmentToComponent(0, 0, i, comImage); //ham sua lai
		if(i == 2) printf("word_recog area 333333.222222 \n");
		checkingI[i - 2] = 0;
		if(i == 2) printf("word_recog area 333333.33333333 \n");
		leftRight_segmentToComponent(componentSegmentImage[i - 2], &left, &right);
		if(i == 2) printf("word_recog area 333333.aaaaaaaa \n");
		for (j = 0; j < input->mHeight; j++)
		{
			for (k = left; k <= right; k++)
			{
				if (comImage->mData[j * width + k] == 1)
				{
					componentSegmentImage[i - 2]->mData[j * width + k] = 1;
					if (checkingI[i - 2] == 0) checkingI[i - 2] = 1;
				}
			}
		}
		////printImage(componentSegmentImage[i - 2]);
		printf("\n\n");
		if(i == 2) printf("word_recog area 333333.bbbbbbbb \n");
		processingComponent(componentSegmentImage[i - 2], checkingI[i - 2]);
		if(i == 2) printf("word_recog area 333333.ccccccccc \n");
	}
	printf("word_recog area 444444444444 \n");
	printf("ket qua nhan dang word \n");
	for (i = 0; i < countCharacter; i++)
	{
		printf("%c",resultWord[i]);
	}
	printf("\n");
	
	return NULL;
}	
structCharacter characterRecognition2(Image* src, int left, int right, int top, int bottom, int checkingI)
{
	structCharacter result;
	if (checkingI == 1)
	{
		//result = characterRecognitionGroup10(src);
		if (bottom > lowerBaseLine + strokeWidth)
		{
			result.character = 'j';
			result.maxValue = 0.9;
		}
		else
		{
			result.character = 'i';
			result.maxValue = 0.9;
		}
		printf("AAAAAAAAAAAAAAaaa    i here\n");
	}
	else
	{
		if (left < 0 || right < 0 || top < 0 || bottom < 0)
		{
			structCharacter result;
			result.maxValue = 0;
			result.character = '[';
			return result;
		}
		if (top < upperBaseLine - strokeWidth)  //chu cao phia tren
		{
			result = characterRecognitionGroup3(src);
		}
		else
		{
			if (bottom > lowerBaseLine + strokeWidth)
			{
				result = characterRecognitionGroup2(src);
			}
			else
			{
				result = characterRecognitionGroup1(src);
				/*
				structCharacter check = characterRecognitionGroup10(src);
				if (check.maxValue > 0.5)
				{
					if (check.character == 'b' || check.character == 'd' || check.character == 'g' ||
						check.character == 'h' || check.character == 'k' || check.character == 'p' ||
						check.character == 'q' || check.character == 'y')
					{
						printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa BASELINE ERROR\n\n");
						result.character = check.character;
						result.maxValue = check.maxValue;
					}
					else if (check.character == 't')
					{
						result.character = check.character;
						result.maxValue = check.maxValue;
					}
					else if (check.maxValue > result.maxValue)
					{
						result.character = check.character;
						result.maxValue = check.maxValue;
					}
				}
				*/
			}
		}
	}
	return result;
}

int cvRound(double a){
	int a1 = (int) a;
	int a2 = (int) a + 1;
	double t1 = a - a1;
	double t2 = a2 - a;
	if(t1<t2) return a1;
	else return a2;
	
}