#include "basic_class.h"

#define					EXPAND_WIDTH		2
#define					EXPAND_HEIGHT		2

//create san image trong ham
void cutImage(Image* input, Image* output, int group);

//using for thinning
int checkCond1(Image* input, Image* output, int row, int col);
int checkCond2(Image* input, Image* output, int row, int col);
void thinning(Image* input, Image* output);  //create san image trong ham

void preProcessAll(Image* input, Image* output, int group);  //create san image trong ham
