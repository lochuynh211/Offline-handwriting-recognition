/*
 *    line drawing sample program: drawing functions
 *
 *    Copyright (C) 2002-2003 by Personal Media Corporation
 */
///////the,m vao
#include <tk/tkernel.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<queue.h>
#include<math.h>
#include<unix/unistd.h>
#include <unix/dirent.h>
#include <unix/fcntl.h>
#include <errno.h>

//#include"graphic.h"
//#include"input.h"
//#include"main.h"
//#include "tree.h"
#include"bmp.h"
//////
//#include "drawsamp.c"
//khai bao ten cac buffer can 

#define SELECT_BUFFER	1
#define BOUND_BUFFER	2
#define PLACE_BUFFER	3
#define PLACE_BUFFER2	4

#define BMP8BIT		1
#define BMP24BIT	2

#define FLIP_HORIZONTAL		1
#define FLIP_VERTICAL 		2

#define BUFFER_UNDO0	0
#define BUFFER_UNDO1	1
#define BUFFER_UNDO2	2

#define UNDO_VALUE0	0
#define UNDO_VALUE1	1

#define FILE1	1
#define FILE2	2
#define FILE3	3
#define FILE4	4
#define FILE5	5

#define TOOL_ROTATE		1
#define TOOL_SELECT		2
#define TOOL_EARSE		3
#define TOOL_FILL		4
#define TOOL_GETCOLOR	5
#define TOOL_ZOOM		6
#define TOOL_PEN		7
#define TOOL_BRUSH		8
#define TOOL_LINE		9
#define TOOL_TEXT		10
#define TOOL_RECT		11
#define TOOL_ELLIPSE	12
#define TOOL_ROUNDRECT	13
#define TOOL_POLYGON	14
#define TOOL_PASTE		15

EXPORT ID input_id = -1;
EXPORT ID mbf_paint_id;
EXPORT ID tskid = -1;
///////het them vao

#include "drawsamp.h"
#include <stdlib.h>
#include <string.h>
#include <device/screen.h>
#include "graphic.h"

/*
 * check that point (x,y) is in rectangular r
 */
EXPORT int in_rect( int x, int y, RECT *r ) {
    return r -> c.left <= x && x < r -> c.right
        && r -> c.top <= y && y < r -> c.bottom;
}

/*
 * draw one pixel
 *  draw one pixel at position (x,y) with color c
 */
EXPORT void draw_pixel( int x, int y, int c, BMP *p_bmp )
{
    switch (p_bmp -> pixbits >> 8) {
    case 8 :
        ((UB*)((p_bmp -> baseaddr)[0]))[p_bmp -> rowbytes * y + x] = c;
        break;
    case 16 :
        c = colormap[c]; /* color map number -> BMP value */
        c = ((c >> 16) & 0xff) >> 3 << 11 /* red */
          | ((c >> 8) & 0xff) >> 2 << 5   /* edge */
          | (c & 0xff) >> 3;              /* blue */
        ((UH*)((p_bmp -> baseaddr)[0]))[p_bmp -> rowbytes / 2 * y + x] = c;
        break;
    case 32 :
        c = colormap[c]; /* color map number -> BMP value */
        ((UW*)((p_bmp -> baseaddr)[0]))[p_bmp -> rowbytes / 4 * y + x] = c;
        break;
    }
}

/*
 * draw line
 *  (x1,y1) to (x2,y2) draw line with color c
 */
EXPORT void draw_line( int x1, int y1, int x2, int y2, int c, BMP *p_bmp )
{
    int t, u, a, b, z;

    if (!in_rect( x1, y1, &(p_bmp -> bounds)) ||
        !in_rect( x2, y2, &(p_bmp -> bounds) )) {

    } else if ((a = abs( x2 - x1 )) > (b = abs( y2 - y1 ))) {
        if (x1 > x2) { // set (x1,y1) la diem co hoanh do nho 
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        u = y1; z = a >> 1; //???
        if (y1 < y2) {
            for (t = x1; t <= x2; t++) {
                draw_pixel( t, u, c, p_bmp );
                z += b;
                if (z > a) { u++; z -= a; }
            }
        } else {
            for (t = x1; t <= x2; t++) {
                draw_pixel( t, u, c, p_bmp );
                z += b;
                if (z > a) { u--; z -= a; }
            }
        }
    } else if (y2 != y1) {
        if (y1 > y2) {
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        u = x1; z = b >> 1;
        if (x1 < x2) {
            for (t = y1; t <= y2; t++) {
                draw_pixel( u, t, c, p_bmp );
                z += a;
                if (z > b) { u++; z -= b; }
            }
        } else {
            for (t = y1; t <= y2; t++) {
                draw_pixel( u, t, c, p_bmp );
                z += a;
                if (z > b) { u--; z -= b; }
            }
        }
    } else {
        draw_pixel( x1, y1, c, p_bmp );
    }
}

/*
 * fill rectangle
 *  fill rectangle (x1,y1)to(x2-1,y2-1) with color c
 */
EXPORT void fill_rect( int x1, int y1, int x2, int y2, int c, BMP *p_bmp )
{
    int u, v;
    if (x1 <= x2 && y1 <= y2 && in_rect( x1, y1, &(p_bmp -> bounds) )
        && in_rect( x2 - 1, y2 - 1, &(p_bmp -> bounds) )) {
        for(v = y1; v < y2; v++) {
            for(u = x1; u < x2; u++) {
                draw_pixel( u, v, c, p_bmp );
            }
        }
    }
}

/*
 * bitmap image of characters (8 x 16 pixels)
 */
LOCAL UB img[94][16] = {
 {0x00,0x10,0x38,0x38,0x38,0x38,0x38,0x10,0x10,0x10,0x10,0x00,0x10,0x38,0x10,0x00} /* ! */
,{0x00,0x28,0x28,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} /* " */
,{0x00,0x00,0x28,0x28,0x2e,0x38,0xe8,0x28,0x2e,0x38,0xe8,0x28,0x28,0x28,0x00,0x00} /* # */
,{0x10,0x10,0x38,0x54,0x92,0x92,0x50,0x30,0x18,0x14,0x92,0x92,0x54,0x38,0x10,0x10} /* $ */
,{0x00,0x42,0xa2,0xa4,0xa4,0xa8,0x48,0x10,0x10,0x24,0x2a,0x4a,0x4a,0x8a,0x84,0x00} /* % */
,{0x00,0x38,0x44,0x44,0x44,0x44,0x28,0x10,0x2a,0x4a,0x44,0x84,0x84,0x8a,0x72,0x00} /* & */
,{0x00,0x20,0x20,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} /* ' */
,{0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x08,0x08,0x04,0x02} /* ( */
,{0x80,0x40,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x40,0x80} /* ) */
,{0x00,0x00,0x00,0x10,0x10,0x92,0x54,0x38,0x54,0x92,0x10,0x10,0x00,0x00,0x00,0x00} /* * */
,{0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x7c,0x10,0x10,0x10,0x10,0x00,0x00,0x00,0x00} /* + */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x20,0x20,0x40} /* , */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} /* - */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00} /* . */
,{0x00,0x02,0x02,0x04,0x04,0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x40,0x80,0x80,0x00} /* / */
,{0x00,0x38,0x44,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x44,0x38,0x00} /* 0 */
,{0x00,0x10,0x30,0x50,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7c,0x00} /* 1 */
,{0x00,0x38,0x44,0x82,0x82,0x02,0x04,0x04,0x08,0x10,0x10,0x20,0x42,0x82,0xfe,0x00} /* 2 */
,{0x00,0x38,0x44,0x82,0x02,0x02,0x04,0x38,0x04,0x02,0x02,0x02,0x82,0x44,0x38,0x00} /* 3 */
,{0x00,0x0c,0x0c,0x14,0x14,0x24,0x24,0x44,0x44,0x84,0x84,0xfe,0x04,0x04,0x04,0x00} /* 4 */
,{0x00,0xfc,0x80,0x80,0x80,0x80,0xb8,0xc4,0x02,0x02,0x02,0x02,0x02,0x84,0x78,0x00} /* 5 */
,{0x00,0x3c,0x42,0x80,0x80,0x80,0xb8,0xc4,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00} /* 6 */
,{0x00,0xfe,0x82,0x82,0x84,0x04,0x04,0x08,0x08,0x08,0x08,0x10,0x10,0x10,0x10,0x00} /* 7 */
,{0x00,0x38,0x44,0x82,0x82,0x82,0x44,0x38,0x44,0x82,0x82,0x82,0x82,0x44,0x38,0x00} /* 8 */
,{0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x46,0x3a,0x02,0x02,0x02,0x84,0x78,0x00} /* 9 */
,{0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00} /* : */
,{0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x30,0x30,0x10,0x10,0x20} /* ; */
,{0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x00} /* < */
,{0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0x00,0x00,0xfe,0x00,0x00,0x00,0x00,0x00,0x00} /* = */
,{0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x00} /* > */
,{0x00,0x38,0x44,0x82,0x82,0x02,0x04,0x08,0x10,0x10,0x10,0x00,0x00,0x10,0x10,0x00} /* ? */
,{0x00,0x38,0x44,0x82,0x9a,0xaa,0xaa,0xaa,0xaa,0xaa,0xac,0x90,0x80,0x42,0x3c,0x00} /* @ */
,{0x00,0x10,0x10,0x28,0x28,0x28,0x44,0x44,0x44,0x7c,0x82,0x82,0x82,0x82,0x82,0x00} /* A */
,{0x00,0xf8,0x84,0x82,0x82,0x82,0x84,0xf8,0x84,0x82,0x82,0x82,0x82,0x84,0xf8,0x00} /* B */
,{0x00,0x38,0x44,0x82,0x82,0x80,0x80,0x80,0x80,0x80,0x80,0x82,0x82,0x44,0x38,0x00} /* C */
,{0x00,0xf8,0x84,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x84,0xf8,0x00} /* D */
,{0x00,0xfe,0x80,0x80,0x80,0x80,0x80,0xfc,0x80,0x80,0x80,0x80,0x80,0x80,0xfe,0x00} /* E */
,{0x00,0xfe,0x80,0x80,0x80,0x80,0x80,0xfc,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00} /* F */
,{0x00,0x38,0x44,0x82,0x82,0x80,0x80,0x80,0x8e,0x82,0x82,0x82,0x82,0x44,0x38,0x00} /* G */
,{0x00,0x82,0x82,0x82,0x82,0x82,0x82,0xfe,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x00} /* H */
,{0x00,0x7c,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7c,0x00} /* I */
,{0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x82,0x84,0x78,0x00} /* J */
,{0x00,0x82,0x82,0x84,0x88,0x90,0xa0,0xd0,0x90,0x88,0x88,0x84,0x84,0x82,0x82,0x00} /* K */
,{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xfe,0x00} /* L */
,{0x00,0x82,0x82,0xc6,0xc6,0xaa,0xaa,0xaa,0x92,0x92,0x92,0x82,0x82,0x82,0x82,0x00} /* M */
,{0x00,0x82,0x82,0xc2,0xc2,0xa2,0xa2,0x92,0x92,0x8a,0x8a,0x86,0x86,0x82,0x82,0x00} /* N */
,{0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00} /* O */
,{0x00,0xf8,0x84,0x82,0x82,0x82,0x82,0x84,0xf8,0x80,0x80,0x80,0x80,0x80,0x80,0x00} /* P */
,{0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xba,0xc6,0x44,0x3a,0x00} /* Q */
,{0x00,0xf8,0x84,0x82,0x82,0x82,0x84,0xf8,0x88,0x88,0x84,0x84,0x84,0x82,0x82,0x00} /* R */
,{0x00,0x38,0x44,0x82,0x82,0x80,0x40,0x30,0x0c,0x02,0x02,0x82,0x82,0x44,0x38,0x00} /* S */
,{0x00,0xfe,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00} /* T */
,{0x00,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00} /* U */
,{0x00,0x82,0x82,0x82,0x82,0x82,0x44,0x44,0x44,0x44,0x28,0x28,0x28,0x10,0x10,0x00} /* V */
,{0x00,0x82,0x82,0x92,0x92,0x92,0x92,0xaa,0xaa,0xaa,0xaa,0x44,0x44,0x44,0x44,0x00} /* W */
,{0x00,0x82,0x82,0x44,0x44,0x28,0x28,0x10,0x10,0x28,0x28,0x44,0x44,0x82,0x82,0x00} /* X */
,{0x00,0x82,0x82,0x82,0x44,0x44,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x10,0x00} /* Y */
,{0x00,0xfe,0x82,0x84,0x04,0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x42,0x82,0xfe,0x00} /* Z */
,{0x00,0x1e,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1e,0x00} /* [ */
,{0x00,0x82,0x82,0x44,0x44,0x28,0x28,0xfe,0x10,0x10,0xfe,0x10,0x10,0x10,0x10,0x00} /* \ */
,{0x00,0x78,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x00} /* ] */
,{0x18,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} /* ^ */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe} /* _ */
,{0x30,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} /* ` */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x84,0x04,0x1c,0x64,0x84,0x84,0x8c,0x72,0x00} /* a */
,{0x00,0x80,0x80,0x80,0x80,0x80,0xb8,0xc4,0x82,0x82,0x82,0x82,0x82,0xc4,0xb8,0x00} /* b */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x42,0x82,0x80,0x80,0x80,0x80,0x42,0x3c,0x00} /* c */
,{0x00,0x02,0x02,0x02,0x02,0x02,0x3a,0x46,0x82,0x82,0x82,0x82,0x82,0x46,0x3a,0x00} /* d */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x82,0xfe,0x80,0x80,0x80,0x42,0x3c,0x00} /* e */
,{0x00,0x1c,0x22,0x20,0x20,0x20,0xfc,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00} /* f */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x7a,0x84,0x84,0x78,0x40,0x80,0x7c,0x82,0x82,0x7c} /* g */
,{0x00,0x80,0x80,0x80,0x80,0x80,0xb8,0xc4,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x00} /* h */
,{0x00,0x10,0x10,0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00} /* i */
,{0x00,0x04,0x04,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x84,0x78} /* j */
,{0x00,0x80,0x80,0x80,0x80,0x80,0x84,0x88,0x90,0xa0,0xe0,0x90,0x88,0x84,0x82,0x00} /* k */
,{0x00,0x30,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00} /* l */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x6c,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x00} /* m */
,{0x00,0x00,0x00,0x00,0x00,0x00,0xb8,0xc4,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x00} /* n */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00} /* o */
,{0x00,0x00,0x00,0x00,0x00,0x00,0xb8,0xc4,0x82,0x82,0x82,0x82,0xc4,0xb8,0x80,0x80} /* p */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x3a,0x46,0x82,0x82,0x82,0x82,0x46,0x3a,0x02,0x02} /* q */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x98,0xa4,0xc0,0x80,0x80,0x80,0x80,0x80,0x80,0x00} /* r */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0x82,0x82,0x60,0x1c,0x02,0x82,0x82,0x7c,0x00} /* s */
,{0x00,0x00,0x20,0x20,0x20,0x20,0xfc,0x20,0x20,0x20,0x20,0x20,0x20,0x22,0x1c,0x00} /* t */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x46,0x3a,0x00} /* u */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x82,0x82,0x44,0x44,0x28,0x28,0x10,0x10,0x00} /* v */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x92,0x92,0x92,0xaa,0xaa,0xaa,0x44,0x44,0x00} /* w */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x82,0x44,0x28,0x10,0x28,0x44,0x82,0x82,0x00} /* x */
,{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x82,0x44,0x44,0x28,0x28,0x10,0x10,0x20,0xc0} /* y */
,{0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0xfe,0x00} /* z */
,{0x00,0x04,0x08,0x08,0x08,0x08,0x08,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x04,0x00} /* { */
,{0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00} /* | */
,{0x00,0x40,0x20,0x20,0x20,0x20,0x20,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x40,0x00} /* } */
,{0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} /* ~ */
};

/*
 * draw string
 *  draw string str at (x,y) with color c
 */
EXPORT void draw_text( char *str, int x, int y, int c, BMP *p_bmp )
{
    int k, h, b, m;
    for(; *str != '\0'; str++) {
        k = *str - '!';
        if (k < 0 || k >= 94) {
            x += 8; continue;
        }
        for(h = 0; h < 16; h++) {
            b = img[k][h];
            for(m = 7; m >= 0; m--) {
                if (b & 1) {
                    draw_pixel( x + m, y + h, c, p_bmp );
                }
                b >>= 1;
            }
        }
        x += 8;
    }
}

/*
 * draw button
 */
#define RD 4
EXPORT void draw_btn( RECT *r, char *s, int psh, BMP *p_bmp ) {
    fill_rect( r -> c.left, r -> c.top, r -> c.right, r -> c.bottom,
           GRAY, p_bmp );

    draw_line( r -> c.left, r -> c.bottom - 1 -RD, r -> c.left, r -> c.top +RD, psh ? BLACK : WHITE, p_bmp );
    draw_line( r -> c.left, r -> c.top +RD, r -> c.left +RD, r -> c.top, psh ? BLACK : WHITE, p_bmp );
    draw_line( r -> c.left +RD, r -> c.top, r -> c.right - 1 -RD, r -> c.top, psh ? BLACK : WHITE, p_bmp );
    draw_line( r -> c.right - 1 -RD, r -> c.top, r -> c.right - 1, r -> c.top +RD, psh ? WHITE : BLACK, p_bmp );
    draw_line( r -> c.right - 1, r -> c.top +RD, r -> c.right - 1, r -> c.bottom - 1 -RD, psh ? WHITE : BLACK, p_bmp );
    draw_line( r -> c.right - 1, r -> c.bottom - 1 -RD, r -> c.right - 1 -RD, r -> c.bottom - 1, psh ? WHITE : BLACK, p_bmp );
    draw_line( r -> c.right - 1 -RD, r -> c.bottom - 1, r -> c.left +RD, r -> c.bottom - 1, psh ? WHITE : BLACK, p_bmp );
    draw_line( r -> c.left +RD, r -> c.bottom - 1, r -> c.left, r -> c.bottom - 1 -RD, psh ? WHITE : BLACK, p_bmp );

    draw_text( s, (r -> c.left + r -> c.right - 8 * strlen(s)) / 2,
           (r -> c.top + r -> c.bottom - 16) / 2, BLACK, p_bmp );
}

///////ham bo sung moi
EXPORT void draw( int x, int y, long c, BMP *p_bmp)
{
	c = ((c >> 16) & 0xff) >> 3 << 11 /* red */
          | ((c >> 8) & 0xff) >> 2 << 5   /* edge */
          | (c & 0xff) >> 3;              /* blue */
        ((UH*)((p_bmp -> baseaddr)[0]))[p_bmp -> rowbytes / 2 * y + x] = c;
}

//show_BMP()
 EXPORT void show_BMP(char* file_name,int x,int y,BMP *p_bmp )
{
	unsigned long ** pixels = NULL;
	FILE *  bfile;
	bfile = fopen(file_name, "r");	
	if (bfile == NULL) 
	{
		//printf("Khong doc file duoc %s \n",file_name);
		Vfree(pixels);
		fclose(bfile);
		return ;
	}

	bmfh * hdr = (bmfh *)Vmalloc(sizeof(bmfh));
  
	// Read BMP 
	int nread;
	nread = fread(hdr, 1, sizeof(bmfh), bfile);
	if (nread != sizeof(bmfh))
	{   
		Vfree(pixels);return ;
	}
	int size;
	int offset;
	size = charArray2Int(hdr->bfSize, 0, 3);
	offset = charArray2Int(hdr->bfOffBits, 0, 3);

	bmih * info = (bmih *)Vmalloc(sizeof(bmih)); 
	nread = fread(info, 1, sizeof(bmih), bfile);
	if (nread != sizeof(bmih)) 
	{   
		Vfree(pixels);
		fclose(bfile);
		return ;
	}
	int biSize;
	int height;
	int width;
	int bitsInPixel;
	int biClrUsed;
	biSize = charArray2Int(info->biSize, 0, 3);
	height = charArray2Int(info->biHeight, 0, 3);
	width = charArray2Int(info->biWidth, 0, 3);
	bitsInPixel = charArray2Int(info->biBitCount, 0, 1);
	biClrUsed = charArray2Int(info->biClrUsed, 0, 3);
 
	int i, j;
	unsigned long * rgbColorTable = NULL;
	if (bitsInPixel < 24) 
	{
		if (biClrUsed == 0) 
		{
			biClrUsed = 1 << bitsInPixel;
		}
		rgbColorTable = (unsigned long *)Vmalloc(biClrUsed * sizeof(int));
		for (i = 0; i < biClrUsed; i++)
		{
			nread = fread(&(rgbColorTable[i]), 1, 4, bfile);      
		}
	}

	if (bitsInPixel != 24 && bitsInPixel != 8) 
	{
		Vfree(pixels);
		fclose(bfile);
		return ;
	}

	int bytesPerPixel = bitsInPixel / 8;
	int bytesPerRow = width * bytesPerPixel;
	bytesPerRow = (bytesPerRow & 0x3) ?  ((bytesPerRow >> 2) + 1) << 2 : bytesPerRow;
	int pad = bytesPerRow - (width * bytesPerPixel);


	pixels = (unsigned long **)Vmalloc(height * sizeof(unsigned long *));
	unsigned char pixelbytes[4];
  
	for (i = 0; i < height; i++) 
	{	    
		pixels[i] = (unsigned long *)Vmalloc(width * sizeof(unsigned long));
		for (j = 0; j < width; j++)
		{
			nread = fread(pixelbytes, 1, bytesPerPixel, bfile);
			if (nread != bytesPerPixel) 
			{
				Vfree(pixels);
				fclose(bfile);
				return ;
			}
			if (bytesPerPixel == 3) 
			{
				pixels[i][j] = (unsigned long)charArray2Int(pixelbytes, 0, 2);
			} 
			else 
			{
				if (pixelbytes[0] >= biClrUsed) 
				{	  
					Vfree(pixels);
					fclose(bfile);
					return ;
				}
				pixels[i][j] = rgbColorTable[pixelbytes[0]];
			}
		}
		nread = fread(pixelbytes,1, pad, bfile);
	}
  	
	
   	if(pixels!=NULL)
 	{
  		for(i=0;i<height;i++)
  		{
  			for(j=x;j<width+x;j++)
			{
				
				int c=pixels[i][j-x];
				draw(j,height+y-i,c,p_bmp);
			}
  		}
		Vfree(pixels);
		fclose(bfile);
		return;
	}
}


EXPORT void draw_textbox( int x1, int y1, int x2, int y2, int c, BMP *p_bmp ){
/// draw textbox from (x1,y1) to (x2,y2) with color c
	draw_line(x1,y1,x1,y2,c,p_bmp);
	draw_line(x1,y2,x2,y2,c,p_bmp);
	draw_line(x2,y2,x2,y1,c,p_bmp);
	draw_line(x2,y1,x1,y1,c,p_bmp);
}

EXPORT void draw_line2( int x1, int y1, int x2, int y2, int c, BMP *p_bmp, int array[][] , int box_x, int box_y)
{
    int t, u, a, b, z;

    if (!in_rect( x1, y1, &(p_bmp -> bounds)) ||
        !in_rect( x2, y2, &(p_bmp -> bounds) )) {

    } else if ((a = abs( x2 - x1 )) > (b = abs( y2 - y1 ))) {
        if (x1 > x2) { // set (x1,y1) la diem co hoanh do nho 
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        u = y1; z = a >> 1; //???
        if (y1 < y2) {
            for (t = x1; t <= x2; t++) {
                draw_pixel( t, u, c, p_bmp );
				////printf("draw pixel (%d,%d) \n",t,u);
				update_array(t, u, array, box_x, box_y);
                z += b;
                if (z > a) { u++; z -= a; }
            }
        } else {
            for (t = x1; t <= x2; t++) {
                draw_pixel( t, u, c, p_bmp );
				////printf("draw pixel (%d,%d) \n",t,u);
				update_array(t, u, array, box_x, box_y);
                z += b;
                if (z > a) { u--; z -= a; }
            }
        }
    } else if (y2 != y1) {
        if (y1 > y2) {
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        u = x1; z = b >> 1;
        if (x1 < x2) {
            for (t = y1; t <= y2; t++) {
                draw_pixel( u, t, c, p_bmp );
				
				update_array(u, t, array, box_x, box_y);
                z += a;
                if (z > b) { u++; z -= b; }
            }
        } else {
            for (t = y1; t <= y2; t++) {
                draw_pixel( u, t, c, p_bmp );
				////printf("draw pixel (%d,%d) \n",u-20,t-20);
				update_array(u, t, array, box_x, box_y);
                z += a;
                if (z > b) { u--; z -= b; }
            }
        }
    } else {
        draw_pixel( x1, y1, c, p_bmp );
		////printf("draw pixel (%d,%d) \n",u-20,t-20);
		update_array(x1, y1, array, box_x, box_y);
    }
}


EXPORT void draw_line3( int x1, int y1, int x2, int y2, int c, BMP *p_bmp, int array[][] , int box_x, int box_y)
{
    int t, u, a, b, z;

    if (!in_rect( x1, y1, &(p_bmp -> bounds)) ||
        !in_rect( x2, y2, &(p_bmp -> bounds) )) {

    } else if ((a = abs( x2 - x1 )) > (b = abs( y2 - y1 ))) {
        if (x1 > x2) { // set (x1,y1) la diem co hoanh do nho 
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        u = y1; z = a >> 1; //???
        if (y1 < y2) {
            for (t = x1; t <= x2; t++) {
                draw_pixel( t, u, c, p_bmp );
				////printf("draw pixel (%d,%d) \n",t,u);
				update_array(t, u, array, box_x, box_y);
				neighbour(t,u,c, p_bmp,array, box_x, box_y);
                z += b;
                if (z > a) { u++; z -= a; }
            }
        } else {
            for (t = x1; t <= x2; t++) {
                draw_pixel( t, u, c, p_bmp );
				////printf("draw pixel (%d,%d) \n",t,u);
				update_array(t, u, array, box_x, box_y);
				neighbour(t,u,c, p_bmp,array, box_x, box_y);
                z += b;
                if (z > a) { u--; z -= a; }
            }
        }
    } else if (y2 != y1) {
        if (y1 > y2) {
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        u = x1; z = b >> 1;
        if (x1 < x2) {
            for (t = y1; t <= y2; t++) {
                draw_pixel( u, t, c, p_bmp );
				
				update_array(u, t, array, box_x, box_y);
				neighbour(u,t,c, p_bmp,array, box_x, box_y);
                z += a;
                if (z > b) { u++; z -= b; }
            }
        } else {
            for (t = y1; t <= y2; t++) {
                draw_pixel( u, t, c, p_bmp );
				////printf("draw pixel (%d,%d) \n",u-20,t-20);
				update_array(u, t, array, box_x, box_y);
				neighbour(u,t,c, p_bmp,array, box_x, box_y);
                z += a;
                if (z > b) { u--; z -= b; }
            }
        }
    } else {
        draw_pixel( x1, y1, c, p_bmp );
		////printf("draw pixel (%d,%d) \n",u-20,t-20);
		update_array(x1, y1, array, box_x, box_y);
		neighbour(x1,y1,c, p_bmp,array, box_x, box_y);
    }
}

EXPORT void update_array(int x, int y, int array[150][210], int box_x, int box_y){
//cap nhat toa do (x,y) vao mang
	int x_index, y_index, i, j;
			x_index = x - box_x;
			y_index = y - box_y;
			if(y_index>=0 && y_index<150 && x_index>=0 && x_index<210){
				//array[x_index][y_index] = 1;
				//*(array + 60*x_index + y_index) = 1;
				////printf("cap nhat pixel tiep theo (%d,%d) co gia tri %d \n", x_index,y_index,*(array + 60*x_index + y_index) );
				//if(box_x == 120 && box_y == 120) //printf("update cho txtbox2 \n");
				array[y_index][x_index] = 1;
				/*
				if(x_index == 12){
					//printf("gia tri cua array 2 chieu (%d,%d) la %d \n",y_index,x_index,array[y_index][x_index]);
					//printf("gia tri cua phan tu 1 chieu (%d,%d) la %d \n",y_index,x_index,*(*array + 60*y_index + x_index));
					//printf("dia chi cua mang array la %d \n",array);
					//printf("dia chi cua mang *array la %d \n",*array);
					//printf("dia chi cua phan tu  array[%d][%d] la %d \n",y_index,x_index,&(array[y_index][x_index]));
					//printf("dia chi cua phan tu  1 chieu la (%d,%d,%d), ket qua (*array) %d \n",*array,60*y_index, x_index, (*array) + (60*y_index) + x_index);
					//printf("dia chi cua phan tu  1 chieu la (%d,%d,%d), ket qua *array %d \n",*array,60*y_index, x_index, *array + (60*y_index) + x_index);
					//printf("dia chi cua phan tu  array + 60*y_index + x_index %d \n",array + 60*y_index + x_index);
					//printf("dia chi cua phan tu  (array) + 60*y_index + x_index %d \n",(array) + 60*y_index + x_index);
				}
				*/
				////printf("update array (%d,%d) co gia tri %d \n", y_index,x_index,array[y_index][x_index] );
			}
}


EXPORT void draw_update(int t,int u,int c,BMP * p_bmp,int array[][], int box_x, int box_y){
	draw_pixel( t, u, c, p_bmp );
	update_array(t, u, array, box_x, box_y);
}

EXPORT void neighbour(int t,int u,int c,BMP * p_bmp,int array[][], int box_x, int box_y){
	draw_update(t,u-1,c, p_bmp,array, box_x, box_y);
	draw_update(t,u+1,c, p_bmp,array, box_x, box_y);
	draw_update(t-1,u,c, p_bmp,array, box_x, box_y);
	draw_update(t+1,u,c, p_bmp,array, box_x, box_y);
}
/* 256 colors standard color map */
const COLOR colormap[] = {
	0x00ffffff, 0x0000009f, 0x0000ef00, 0x0000efef,
	0x00ff0000, 0x00ef00ff, 0x00dfdf00, 0x007f7f9f,
	0x00dfdfdf, 0x007f9fff, 0x00bfffaf, 0x00cfffff,
	0x00ff6f6f, 0x00ef8fff, 0x00efff9f, 0x00010101,
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


