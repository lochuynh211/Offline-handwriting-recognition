#include "character_recognition.h"

typedef struct _structError{
	structCharacter charLeft;
	structCharacter charJoint;
}structError;

typedef struct _rowDensity{
	int* transition;
	int* density; 
}rowDensity;
typedef struct _colDensity{
	int* transition;
	int* density;
	int* flag;
	int* flag2;
}colDensity;

typedef struct _boundary{
	int left;
	int right;
	int top;
	int bottom;
}boundary;
typedef struct _resultSegment{
	int left;
	int right;
}resultSegment;

typedef struct _int_array{
	int* arr;
	int  length; 
	int real_length; //count of element have value (!= -1)
}int_array;

typedef struct _cont_area{ //continous area of pixel
	int head;
	int tail; 
}cont_area;

typedef struct _cont_arr{ //continous area of pixel
	cont_area*  arr;
	int  length; 
	int real_length; //count of element have value (!= -1)
}cont_arr;

typedef struct _seg_point{ //segment point
	int pos;
	int flag; //=0: dis_point, =1: black_seg, =2: white_seg
}seg_point;

typedef struct _seg_point_arr{ //continous area of pixel
	seg_point*  arr;
	int  length; 
	int real_length; //count of element have value (!= -1)
}seg_point_arr;


#define SEG_CRITERIA_RATE	1.5
#define HOLE_RATE			2.0
#define MAX_WORD_LENGTH		30
#define MY_ERROR			-1

//main function
char* wordRecognition(Image* input, int* countOfResultOfWord);

//using for dictionary
void createDictionary();
int convertCharacterToIndex1(char cha);
int convertCharacterToIndex2(char cha);
int checkingOneCharacter(char character); 
int checkingTwoCharacter(char character1, char character2);
int checkingThreeCharacter(char character1_1, char character1_2, char character2);
int checkingConnectedCharacter(char left, char right);
int addCharacter(structCharacter character, int error, structCharacter characterJoint);
int compareTwoCharacter(char cha1, char cha2);


//support for Image
void add_over_seg_point(Image* src, seg_point_arr * arr);
void add_baseline(Image *src, int upper, int lower);
void add_horizalLine(Image* src, int row);
void add_verticalLine(Image* src, int col);
//IplImage* invert(Image* src);
void showImage(char* name, Image* src);

//slant correction
void slantCorrection();
int slantDectection();
int findMaxVertical(Image* src);
Image* rotateSlant(Image* input, int phi);
int findMaxHorizon(Image* src); 

//using for calculate strokeWidth
void calculateStrokeWidth();
void findingForStrokeWidth(int value, int* result, int* countResult, int* count); //stroke width

//finding baseline
void baseline();
rowDensity* findingRowDensity(Image* input);
colDensity* findingColDensity(Image* input, int begin, int end);
int findMiddleRow(rowDensity* row);
int findUpperBaseLine(int middleRow, rowDensity* row);
int findLowerBaseLine(int middleRow, rowDensity* row);
void density_his(Image *src , int *his);
void density_deviation(int* his, int* devi_his, int length);
int find_max_element(int* arr, int head, int tail, int flag);

//finding segment
void segment();

//oversegment
seg_point_arr* over_segment(Image* src, int stroke, int upper, int lower);
seg_point_arr* over_segment2(Image* src, int stroke, int upper, int lower);
void pixel_density(Image *src, int upper, int lower, int* densityArr);
void init_over_seg_arr(int* arr, int width);
void find_seg_point(Image* src, int * density_arr, int* over_seg_arr, int* over_seg_den, double seg_criteria, int* length);
int_array* create_int_array(int length);
cont_arr* create_cont_array(int length);
void seg_point_classification(int* over_seg_arr, int length, int* over_seg_den,int_array* dis_point_arr, cont_arr* cont_black_point, cont_arr* cont_white_point);
void update_cont_arr(cont_arr* arr, int pos, int head, int tail );
void cal_white_segment(cont_arr* cont_white, int_array * segment);
void cal_white_segment2(cont_arr* cont_white, int_array * segment);
int cal_black_segment_length(cont_arr* cont_black_point, int stroke);
void cal_black_segment(cont_arr* cont_black_point, int stroke, int_array * segment);
void cal_black_segment2(cont_arr* cont_black_point, int stroke, int_array * segment);
void cal_over_seg_arr(int_array* over_seg, int_array* dis_arr, int_array* black_seg, int_array* white_seg);
void cal_over_seg_arr2(seg_point_arr* over_seg, int_array* dis_arr, int_array* black_seg, int_array* white_seg);
void sort_arr2(seg_point_arr* arr);

//STFPC
void STFPC2(Image *src, seg_point_arr* over_arr, int stroke, int upper, int lower);
void STFPC3(Image *src, seg_point_arr* over_arr, int stroke, int upper, int lower);
void post_STFPC(Image* src, seg_point_arr* over_arr, int stroke, int upper, int lower);
void post_STFPC2(Image* src, seg_point_arr* over_arr, int stroke, int upper, int lower);
int segment_pixel_density(Image *src, int head, int tail, int upper, int lower);
void remove_array_element(seg_point_arr* arr, int index);
seg_point_arr* create_seg_point_array(int length);

//hole detection
int holeDetection(int col, int* countTransition);
int holeDetection2(int col, int* countTransition);
int* endLeftContour(int row, int* lengOfResult, Image* input, int type);
int* endRightContour(int row, int* lengOfResult, Image* input, int type);
void reduceStackForHoleDetection(Image* input, Stack* stack);
int findingNextPoint1(int curRow, int curCol, int* nextRow, int* nextCol, Stack* stack, Image* input);

//findoutComponent
int findoutConnectedComponent(Image* src, Image* des);
int findoutConnectedComponent2(int left, int right);
int findingNextPointForConnectedComponents(int curRow, int curCol, int* nextRow, int* nextCol, Stack* stack, Image* input);

//supported function for validation
Image* createImageFromInput(int left, int right, int value, Image* src);	//value using for component image default : 0
void leftRightUpBottom(Image* src, int* left, int* right, int* top, int* bottom);
structCharacter characterRecognition(Image* src, int left, int right, int top, int bottom);

//validation
int recursiveValidation();
int validation(int left_1, int right_1, int left_2, int right_2);
int checkPlausible(structCharacter leftCha, structCharacter rightCha, structCharacter jointCha, int* checkLeft, int* checkRight, int* checkJoint);
int findoutConn$ectedComponent2(int left, int right);

//add in
void removeFaultSegment(seg_point_arr* seg);
int checkingFaultSegment(int col);

//giai thuat moi here//giai thuat moi here//giai thuat moi here
//giai thuat moi here//giai thuat moi here//giai thuat moi here
//giai thuat moi here//giai thuat moi here//giai thuat moi here
//giai thuat moi here//giai thuat moi here//giai thuat moi here
//giai thuat moi here//giai thuat moi here//giai thuat moi here
//giai thuat moi here//giai thuat moi here//giai thuat moi here



typedef struct _segmentArea
{
	int head;
	int tail;
	int flagHead;  //co` ve vi tri cat  0: ko cat 1: cat duoi 2: cat tren
	int flagTail;  
}segmentArea;
typedef struct _segmentAreaArr
{
	segmentArea* area;
	int length;
}segmentAreaArr;




//main function
char* wordRecognition2(Image* inImage);
structCharacter characterRecognition2(Image* src, int left, int right, int top, int bottom, int checkingI);

//strokeWidth nhu cu~
//baseline
void baseline2();
void findBaseLine(rowDensity* rowDens);
int findConnectedComponent(Image* src, Image* des);

//segmentation to component
int findConnectedComponent_segmentToComponent(Image* src, Image* des);
Image* createImageFromInput_segmentToComponent(int left, int right, int value, Image* src);
void leftRight_segmentToComponent(Image *src, int* left, int* right);

//using to process each component
void processingComponent(Image* comImage, int checkingI);
void leftRightUpBottom_processingComponent(Image* src, int* left, int* right, int* top, int* bottom);

//using to process complex component 
void processingComplexComponent(Image* comImage);
segmentAreaArr* overSegment_processingComplexComponent(Image* comImage);
void jointSegment_processingComplexComponent(segmentAreaArr* areaArr, int index);
void deleteSegment_processingComplexComponent(segmentAreaArr* areaArr, int index);
//cac chu~ gep tren : b, f, o, v, w
//cac chu~ gep duoi : a, c, d, e, g, h, i, j, k, l, m, n, p, q, r, s, t, u, x, y, z

//validation
int validation_processingComplexComponent(Image* comImage, segmentAreaArr* areaArr);
int recursiveValidation_processingComplexComponent(Image* comImage, segmentAreaArr* areaArr, int index, int iNum, int* iArr);
Image* createSegmentImage_processingComplexComponent(Image* comImage, int left, int right);
int checkingConnection_processingComplexComponent(Image* comImage, int left, int right);
void validationCharacter(Image* img, structCharacter character, int* checking, int left, int right); 


//support function
segmentAreaArr* copySegmentAreaArr(segmentAreaArr* src);
void copySegmentAreaArr(segmentAreaArr* src, segmentAreaArr* des);
void addCharacter2(structCharacter character);

