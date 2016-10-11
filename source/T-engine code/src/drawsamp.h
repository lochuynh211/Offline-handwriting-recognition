/*
 *    line drawing sample program: common header file
 *
 *    Copyright (C) 2002 by Personal Media Corporation
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <device/screen.h>
#include "basic_class.h"
#include<sys/pinfo.h>

/* main.c */

#define PRI1 80
/* my code  */
#define CHARACTER  1
#define WORD  2
#define PARAGRAPH  3

#define HOME  1
#define MENU  2
/* my code  */

#define BEFORE_DRAW 0
#define DRAWING 1
#define FINISH_DRAW 2
#define RECOGNIZING 3
#define FINISH_RECOGNIZE 4

#define ONE_MS 16666

#define ALPHABETIC 1
#define NUMBER 2
#define ALPHANUMERIC	 3
#define UPPERCASE 4
#define LOWERCASE 5


#define NO_SELECT 1
#define SELECTING 2
#define FINISH_SELECT 3

#define DISABLE 0
#define ENABLE 1

IMPORT void task( INT, VP );
IMPORT void cychdr(VP exinf);
/* drawsamp.c */

IMPORT void exitproc( void );


/* mem.c */

IMPORT void ini_stk( void );
IMPORT void psh_stk( H );
IMPORT H pop_stk( void );

//////
typedef struct _output_line{
	char str[30];
	int upper_line;
	int lower_line;
	int length;
	int real_length;
}output_line;

typedef struct _output_panel{
	output_line line[4];
	int length;
}output_panel;

typedef struct _cursor{
	int line;
	int pos; 
}cursor;

void cal_RECT_coordinate(RECT* object, int width, int height, int left, int bottom);
void init_rect();


void ab_btn_proc(int status);
void btn_12_proc(int status);
void a2_btn_proc(int status);
void keyboard_btn_proc();
void charac_box_proc(int* scr_status, int* input_status);
void word_box_proc(int* scr_status, int* input_status);
void para_box_proc(int* scr_status, int* input_status);
void delay(double milisecond);
void exit_menu_update_home(int status);

void exit_menu();
void delay_1ms();

void Init_output_line(output_line* out);
void Init_out_panel(output_panel* out);
void out_line0_proc(int x);
void out_line1_proc(int x);
void out_line2_proc(int x);
void out_line3_proc(int x);
void Insert_char(output_panel* panel, int line, int pos, char a); //insert char a to output_panel at pos of line

void fill_rect_v2(int x, int y,int width, int height,int c, BMP *p_bmp );
void Clear_char(int line, int pos); //clear char in the screen at line and position pos
void draw_cursor(int line, int pos);
void Init_cursor();
void space_proc();
int Is_end_line();
int At_final_line();

void shift_1_character(int line, int pos);
void print_out_panel();

void Clear_line(int line);
void Clear_out_panel();

void Shift_line(int line, int pos);
void enter_proc();

void recognize();
void Clear_draw_panel();
void init_draw_matrix();
void copy_matrix(int src[150][210], Image* des);

void print_img(Image* des);
void print_char(int line, int pos);
void Clear_many_lines_in_array(int begin_line, int begin_pos, int end_line, int end_pos);
//clear char from begin_pos of begin_line to end_pos of end_line
void clear_char_in_array(int line, int pos);

void clear_proc();
int Is_begin_of_line();
int At_begin_line();
void Increase_cursor();
void cal_begin_block_pos(int x,int y);
void update_end_block_pos(x,y);

void highlight_char(int line, int pos, int c);
void highlight_line(int line, int c);
void highlight_block(int begin_line, int begin_pos, int end_line, int end_pos, int c);
void clear_line_in_array(int line);
void select_proc();
void call_highlight_block(int x);
void update_old_end_block();


void clear_selected_block();
void Unselect_active_block();
void insert_char_in_cursor(char* str, int length);

void shift_line_many_pos(int line, int pos, int num);
int cal_distance_from_cursor_to_next_char();
int is_enough_space_for_insert_word(int word_length, int blank_distance);
void insert_word_result(int num, char* str);
int is_no_char_to_end_line();
void move_cursor_to_begin_of_next_line();
void copy_line_to_line(int src_line, int des_line);
void shift_all_line(int line);
void copy_rest_of_current_line_to_line(int src_line, int src_pos, int des_line);