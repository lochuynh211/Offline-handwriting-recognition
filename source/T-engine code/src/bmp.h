#include <stdlib.h>
#include <string.h>
#include <device/screen.h>
#include <math.h>
#include <stdio.h>

typedef struct {
  char bfType[2];
  unsigned char bfSize[4];
  unsigned char bfReserved1[2];
  unsigned char bfReserved2[2];
  unsigned char bfOffBits[4];
} bmfh;
typedef struct {
  unsigned char biSize[4];
  unsigned char biWidth[4];
  unsigned char biHeight[4];
  unsigned char biPlanes[2];
  unsigned char biBitCount[2];
  unsigned char stuff1[16];
  unsigned char biClrUsed[4];
  unsigned char biClrImportant[4];
} bmih;

struct ColorList
{
	unsigned int color;			//gia ti mau
	int freq;					//so lan mau dc su dung trong anh
	struct ColorList *next;		//ptu tiep theo
}; 
struct ColorBuffer
{
	unsigned int color;
	struct ColorBuffer *next;
};

IMPORT void insert_ColorList(unsigned int color_value, struct ColorList *root);
IMPORT void sort(struct ColorList *root);
IMPORT void insert_ColorBuffer(unsigned int color_value, struct ColorBuffer *root);
IMPORT void clear_ColorList(struct ColorList *root);
IMPORT void clear_ColorBuffer(struct ColorBuffer *root);

IMPORT int charArray2Int(unsigned char * b, int s, int f);
IMPORT unsigned int return32bit(long c, struct ColorBuffer * image_buffer);
IMPORT void create_colormap(struct ColorList *image_colorlist, unsigned int *newcolormap, int index);
IMPORT void long2charArray(long l, char * CHAR);
IMPORT int check_add_8bit(int i);
IMPORT int check_add_24bit(int i);
IMPORT unsigned char FindIndex(unsigned int color, unsigned int *newcolormap);
IMPORT void convert_bgr(unsigned int v, char *dest);


