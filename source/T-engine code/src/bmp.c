#include "bmp.h"
#include <math.h>

unsigned int  color[256] = {
	0x00ffffff, 0x00FF0000, 0x00ff7700, 0x00FFFF00,
	0x00996600, 0x0011ee00, 0x00007700, 0x000000FF,
	0x0011eeFF, 0x00FF00FF, 0x00995599, 0x00bbbbbb,
	0x00FF0099, 0x00ef8fff, 0x00efff9f, 0x00010101,
	0x00ffffcc, 0x00ffff66, 0x00ffff33, 0x00ffff00,
	0x00ffccff, 0x00ffcccc, 0x00ffcc99, 0x00ffcc66,
	0x00ffcc33, 0x00ffcc00, 0x00ff99cc, 0x00ff9999,
	0x00ff9966, 0x00ff9933, 0x00ff9900, 0x00ff66ff,
	0x00ff66cc, 0x00ff6699, 0x00ff6633, 0x00ff6600,
	0x00ff33ff, 0x00ff33cc, 0x00ff3399, 0x00ff3366,
	0x00ff3333, 0x00ff3300, 0x00ff00cc, 0x00ff0099,
	0x00ff0066, 0x00ff0033, 0x00ccff99, 0x00ccff66,
	0x00ccff33, 0x00ccff00, 0x00ccccff, 0x00cccc99,
	0x00cccc66, 0x00cccc33, 0x00cc99ff, 0x00cc99cc,
	0x00cc9999, 0x00cc9966, 0x00cc9933, 0x00cc9900,
	0x00cc66ff, 0x00cc66cc, 0x00cc6699, 0x00cc6666,
	0x00cc6633, 0x00cc6600, 0x00cc33ff, 0x00cc33cc,
	0x00cc3399, 0x00cc3366, 0x00cc3333, 0x00cc3300,
	0x00cc00ff, 0x00cc00cc, 0x00cc0099, 0x00cc0066,
	0x00cc0033, 0x00cc0000, 0x0099ffff, 0x0099ffcc,
	0x0099ff99, 0x0099ff66, 0x0099ff33, 0x0099ff00,
	0x0099ccff, 0x0099cccc, 0x0099cc99, 0x0099cc66,
	0x0099cc33, 0x0099cc00, 0x009999ff, 0x009999cc,
	0x00999999, 0x00999966, 0x00999933, 0x00999900,
	0x009966ff, 0x009966cc, 0x00996699, 0x00996666,
	0x00996633, 0x00996600, 0x009933ff, 0x009933cc,
	0x00993399, 0x00993366, 0x00993333, 0x00993300,
	0x009900ff, 0x009900cc, 0x00990099, 0x00990066,
	0x00990033, 0x00990000, 0x0066ffff, 0x0066ffcc,
	0x0066ff99, 0x0066ff66, 0x0066ff33, 0x0066ff00,
	0x0066ccff, 0x0066cccc, 0x0066cc99, 0x0066cc66,
	0x0066cc33, 0x0066cc00, 0x006699cc, 0x00669999,
	0x00669966, 0x00669933, 0x00669900, 0x006666ff,
	0x006666cc, 0x00666666, 0x00666633, 0x00666600,
	0x006633ff, 0x006633cc, 0x00663399, 0x00663366,
	0x00663333, 0x00663300, 0x006600ff, 0x006600cc,
	0x00660099, 0x00660066, 0x00660033, 0x00660000,
	0x0033ffff, 0x0033ffcc, 0x0033ff99, 0x0033ff66,
	0x0033ff33, 0x0033ff00, 0x0033ccff, 0x0033cccc,
	0x0033cc99, 0x0033cc66, 0x0033cc33, 0x0033cc00,
	0x003399ff, 0x003399cc, 0x00339999, 0x00339966,
	0x00339933, 0x00339900, 0x003366ff, 0x003366cc,
	0x00336699, 0x00336666, 0x00336633, 0x00336600,
	0x003333ff, 0x003333cc, 0x00333399, 0x00333366,
	0x00333333, 0x00333300, 0x003300ff, 0x003300cc,
	0x00330099, 0x00330066, 0x00330033, 0x00330000,
	0x0000ffcc, 0x0000ff99, 0x0000ff66, 0x0000ff33,
	0x0000ff00, 0x0000ccff, 0x0000cccc, 0x0000cc99,
	0x0000cc66, 0x0000cc33, 0x0000cc00, 0x000099ff,
	0x000099cc, 0x00009999, 0x00009966, 0x00009933,
	0x00009900, 0x000066ff, 0x000066cc, 0x00006699,
	0x00006666, 0x00006633, 0x00006600, 0x000033ff,
	0x000033cc, 0x00003399, 0x00003366, 0x00003333,
	0x00003300, 0x000000ff, 0x000000cc, 0x00000066,
	0x00000033, 0x00ee0000, 0x00dd0000, 0x00bb0000,
	0x00aa0000, 0x00880000, 0x00770000, 0x00550000,
	0x00440000, 0x00220000, 0x00110000, 0x0000dd00,
	0x0000bb00, 0x0000aa00, 0x00008800, 0x00007700,
	0x00005500, 0x00004400, 0x00002200, 0x00001100,
	0x000000ee, 0x000000dd, 0x000000bb, 0x000000aa,
	0x00000088, 0x00000077, 0x00000055, 0x00000044,
	0x00000022, 0x00eeeeee, 0x00cccccc, 0x00bbbbbb,
	0x00aaaaaa, 0x00888888, 0x00777777, 0x00555555,
	0x00444444, 0x00222222, 0x00111111, 0x00000000
};
EXPORT void insert_ColorList(unsigned int color_value, struct ColorList *root)
{
	int flag = 0;
	int f;
	if(root->color == 0xFFFFFFFF){
		root->color = color_value;
		root->freq = 1;
		root->next = NULL;
		return;
	}
	struct ColorList *temp = root;
	struct ColorList *pretemp = root;
	while(temp != NULL && flag == 0){
		if(temp->color == color_value){
			flag = 1;
		}else{
			pretemp = temp;
			temp = temp->next;
		}
	}
	if(flag == 1){
		f = temp->freq;
		temp->freq = f+1;
	}else{
		struct ColorList *newnode = malloc(sizeof(struct ColorList));
		newnode->color = color_value;
		newnode->freq = 1;
		newnode->next = NULL;
		pretemp->next = newnode;	
	}
}

EXPORT void sort(struct ColorList *root)
{
	int max;
	struct ColorList *temp = root->next;
	struct ColorList *maxfreq = root;
	if(root->next == NULL){
		return;
	}
	max = root->freq;
	while(temp != NULL){
		if(temp->freq > max){
			max = temp->freq;
			maxfreq = temp;
		}else{
			temp = temp->next;
		}
	}
	if(maxfreq != root){
		unsigned int root_color = root->color;
		int root_freq = root->freq;
		root->color = maxfreq->color;
		root->freq = maxfreq->freq;
		maxfreq->color = root_color;
		maxfreq->freq = root_freq;
	}
	sort(root->next);
}
EXPORT void insert_ColorBuffer(unsigned int color_value, struct ColorBuffer *root)
{
	int flag = 0;
	int n = 0;
	if(root->color == 0xFFFFFFFF){
		root->color = color_value;
		root->next = NULL;
		return;
	}
	struct ColorBuffer *temp = root;
	struct ColorBuffer *pretemp = root;
	while(temp != NULL && flag == 0){
		if(temp->color == color_value){
			flag = 1;
		}else{
			n++;
			pretemp = temp;
			temp = temp->next;
		}
	}
	if(flag == 0){
		struct ColorBuffer *newnode = malloc(sizeof(struct ColorBuffer));
		newnode->color = color_value;
		newnode->next = NULL;
		pretemp->next = newnode;
	}
	//printf("\t%d", n);
}
EXPORT void clear_ColorList(struct ColorList *root)
{
	struct ColorList *temp = root;
	struct ColorList *pretemp = root;
	if(root->next != NULL){
		while(temp->next != NULL){
			pretemp = temp;
			temp = temp->next;
		}
		free(temp);
		pretemp->next = NULL;
		clear_ColorList(root);
	}
}
EXPORT void clear_ColorBuffer(struct ColorBuffer *root)
{
	struct ColorBuffer *temp = root;
	struct ColorBuffer *pretemp = root;
	if(root->next != NULL){
		while(temp->next != NULL){
			pretemp = temp;
			temp = temp->next;
		}
		free(temp);
		pretemp->next = NULL;
		clear_ColorBuffer(root);
	}
}

EXPORT int charArray2Int(unsigned char * b, int s, int f)
{
  int ret = 0;
  int i;
  int shift = 0;

  for (i = s; i <= f; i++) 
  {
    ret = ret | ((b[i] & 0xff) << shift);
    shift += 8;
  }
  return ret;
}

EXPORT unsigned int return32bit(long c, struct ColorBuffer * image_buffer)
{
	int i = 0;
	int flag = 0;
	unsigned int t;
	struct ColorBuffer *temp = image_buffer;
	while(temp != NULL && flag == 0){
		t  = ((temp->color >> 16) & 0xff) >> 3 << 11 /* red */
          | ((temp->color >> 8) & 0xff) >> 2 << 5   /* edge */
          | (temp->color & 0xff) >> 3;              /* blue */
		if(c == t){
			flag = 1;
		}else{
			temp = temp->next;
		}
	}
	if(flag == 1){
		return temp->color;
	}
	while(i<256 && flag == 0){
		t = ((color[i] >> 16) & 0xff) >> 3 << 11 /* red */
          | ((color[i] >> 8) & 0xff) >> 2 << 5   /* edge */
          | (color[i] & 0xff) >> 3;              /* blue */
		if(c == t){
			flag = 1;
		}else{
			i++;
		}
	}
	if(flag == 1){
		return color[i];
	}
}
EXPORT void create_colormap(struct ColorList *image_colorlist, unsigned int *newcolormap, int index)
{
	int i;
	struct ColorList *temp = image_colorlist;
	for(i = 0; i<255; i++){
		if(i < index){
			newcolormap[i] = temp->color;
			temp = temp->next;
		}else{
			newcolormap[i] = 0x00FFFFFF;
		}
	}
	newcolormap[255] = 0x00FFFFFF;
}

EXPORT void long2charArray(long l, char * CHAR)
{
	int i;
	for(i = 3; i>=0; i--){
		int temp = (int)(l/pow(2, i*8));
		CHAR[i] = (char)(temp);
	}
}
EXPORT int check_add_8bit(int i){
	if((i%4) == 0){
		return i;
	}else{
		i = i+1;
		check_add_8bit(i);
	}
}
EXPORT int check_add_24bit(int i)
{
	int n = 0;
	while((3*i+n) %4 != 0){
		n++;
	}
	printf("\n3*i+n=%d", (3*i+n));
	return n;
}
EXPORT unsigned char FindIndex(unsigned int color, unsigned int *newcolormap)
{
	int i = 0;
	int flag = 0;
	while(i<256 && flag == 0){
		if(color == newcolormap[i]){
			flag = 1;
		}else{
			i++;
		}
	}
	if(flag == 1){
		return i;
	}else{
		return 255;
	}
}
EXPORT void convert_bgr(unsigned int v, char *dest)
{
	dest[0] = (char)(v & 0x000000FF);
	dest[1] = (char)((v & 0x0000FF00) >> 8);
	dest[2] = (char)((v & 0x00FF0000) >> 16);
}

