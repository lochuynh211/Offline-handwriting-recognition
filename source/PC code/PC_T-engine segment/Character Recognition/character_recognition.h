#include "feature_extraction.h"
#include "pre_processing.h"
#include <iostream>
#include <fstream>
#include <io.h>

using std::cout;
using std::ifstream;
using std::ofstream;



#define		STANDARD_MAX		0.5

#define		NUM_INPUT_GROUP1	120
#define		NUM_INPUT_GROUP2	180
#define		NUM_INPUT_GROUP3	180
#define		NUM_INPUT_GROUP4	180
#define		NUM_INPUT_GROUP5	180
#define		NUM_INPUT_GROUP6	144
#define		NUM_INPUT_GROUP7	144
#define		NUM_INPUT_GROUP8	180
#define		NUM_INPUT_GROUP9	180
#define		NUM_INPUT_GROUP10	144

#define		NUM_HIDDEN_GROUP1	100
#define		NUM_HIDDEN_GROUP2	50
#define		NUM_HIDDEN_GROUP3	50
#define		NUM_HIDDEN_GROUP4	100
#define		NUM_HIDDEN_GROUP5	50
#define		NUM_HIDDEN_GROUP6	100
#define		NUM_HIDDEN_GROUP7	50
#define		NUM_HIDDEN_GROUP8	50
#define		NUM_HIDDEN_GROUP9	100
#define		NUM_HIDDEN_GROUP10	100

#define NUM_OUTPUT_GROUP1		14		//chu~ nho
#define NUM_OUTPUT_GROUP2		5		//chu thuong duoi'  y
#define NUM_OUTPUT_GROUP3		7		//chu thuong tren	d
#define NUM_OUTPUT_GROUP4		26		//chu hoa
#define NUM_OUTPUT_GROUP5		10		//so'
#define NUM_OUTPUT_GROUP6		52		//chu~ cho character
#define NUM_OUTPUT_GROUP7		10		//so cho character
#define NUM_OUTPUT_GROUP8		43		//phan biet cho group 3 4 5 // chi xay dung tap mau~, 
										//se sua doi trong training thanh 3
#define NUM_OUTPUT_GROUP8_REAL	3

#define NUM_OUTPUT_GROUP9		43
#define NUM_OUTPUT_GROUP10		26


//so characters tren moi ki tu
#define NUM_CHARACTER_GROUP1	2000	//cho word
#define NUM_CHARACTER_GROUP2	2000
#define NUM_CHARACTER_GROUP3	2000
#define NUM_CHARACTER_GROUP4	2000
#define NUM_CHARACTER_GROUP5	2000
#define NUM_CHARACTER_GROUP6	2000    //cho character
#define NUM_CHARACTER_GROUP7	2000
#define NUM_CHARACTER_GROUP8	2000
#define NUM_CHARACTER_GROUP9	2000
#define NUM_CHARACTER_GROUP10	2000

typedef struct _structCharacter{
	char character;
	double	maxValue;
}structCharacter;

void initAll();
structCharacter characterRecognitionGroup1(Image* inImage);
structCharacter characterRecognitionGroup2(Image* inImage);
structCharacter characterRecognitionGroup3(Image* inImage);
structCharacter characterRecognitionGroup4(Image* inImage);
structCharacter characterRecognitionGroup5(Image* inImage);
structCharacter characterRecognitionGroup6(Image* inImage);
structCharacter characterRecognitionGroup7(Image* inImage);
structCharacter characterRecognitionGroup8(Image* inImage);
structCharacter characterRecognitionGroup9(Image* inImage);
structCharacter characterRecognitionGroup10(Image* inImage);

char convertLocationGroup1(int location);
char convertLocationGroup2(int location);
char convertLocationGroup3(int location);
char convertLocationGroup4(int location);
char convertLocationGroup5(int location);
char convertLocationGroup6(int location);
char convertLocationGroup7(int location);
char convertLocationGroup8(int location);
char convertLocationGroup9(int location);
char convertLocationGroup10(int location);


