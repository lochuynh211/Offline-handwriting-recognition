#include "basic_class.h"
		
#define			MAX_OF_TRANSITION	3				

void getFeatureExtraction(Image* input, double* output, int group);

//finding starting point
void findingStartingPoint(Image* input, int* maxOfLengthLine); //input cung la output dua ra
void reduceStack(Stack* stack, Image* input);
int findingNextPoint(Stack* stack, Image* input, int curRow, int curCol, int* nextRow, int* nextCol);
unsigned char convertToDirection(int curRow, int curCol, int nextRow, int nextCol);
int checkCorner(Image* input, unsigned char previousDirection, int curRow, int curCol);
int findingNextPointOfCheckCorner(Image* input, int curRow, int curCol, int* nextRow, int* nextCol);

//normalize image 
void normalizeImageDirection(Image* input, Image* tempImage, int maxOfLengthLine);
void reduceStackForNomarlization(Image* input, Image* tempImage, Stack* stack);
int findingNextPointForNormalization(Image* input, Image* tempImage, Stack* stack, int curRow, int curCol, int* nextRow, int* nextCol);
void modifyDirection(Image* input, Stack* list, unsigned char value);
unsigned char findingValueToModify(int countingDirection2, int countingDirection3, int countingDirection4, int countingDirection5);


//get the LT and DT
void getMDFFeatureVector(Image* input, double* output, int RESAMPLE_SIZE_HEIGHT, int RESAMPLE_SIZE_WIDTH);