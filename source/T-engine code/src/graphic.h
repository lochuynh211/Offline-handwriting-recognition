/*
 *    line drawing sample program: drawing functions
 *
 *    Copyright (C) 2002-2003 by Personal Media Corporation
 */

#include <device/screen.h>

#define BLACK 255
#define WHITE 0
#define GRAY 247
IMPORT const COLOR colormap[256];

IMPORT int in_rect( int, int, RECT* );
IMPORT void draw_line( int, int, int, int, int, BMP* );
IMPORT void fill_rect( int, int, int, int, int, BMP* );
IMPORT void draw_text( char*, int, int, int, BMP* );
IMPORT void draw_btn( RECT*, char *, int, BMP* );
IMPORT void draw_btn( RECT *r, char *s, int psh, BMP *p_bmp );
IMPORT void draw( int x, int y, long c, BMP *p_bmp);
IMPORT void show_BMP(char* file_name,int x,int y,BMP *p_bmp );

/* hàm moi duoc bo sung
*/

IMPORT void draw_textbox(int, int, int, int, int, BMP*);
IMPORT void draw_line2( int, int, int, int, int, BMP*, int[][], int, int );
//IMPORT void draw_line2( int, int, int, int, int, BMP*, int[][], int, int );

//IMPORT void update_array(int , int , int *, int, int);
IMPORT void update_array(int , int , int [][], int, int);

IMPORT void draw_update(int t,int u,int c,BMP * p_bmp,int array[][], int box_x, int box_y);

IMPORT void neighbour(int t,int u,int c,BMP * p_bmp,int array[][], int box_x, int box_y);


