/*
 *    line drawing sample program
 *
 *    Copyright (C) 2002-2004 by Personal Media Corporation
 */

#include "drawsamp.h"
#include <tk/tkernel.h>
#include <device/screen.h>
#include <device/kbpd.h>
#include <btron/event.h>
#include <string.h>
#include <stdlib.h>
#include "graphic.h"


#include <stdio.h>


#include "character_recognition.h"
#include "word_recognition.h"

//EXPORT int temp_var;
//IMPORT int temp_var;
//int temp_var;
//extern int temp_var;

/* old key scan code */
#define O_SW1_1 0x8c
#define O_SW1_2 0x8b
#define O_SW1_3 0x89
#define O_SW1_4 0x8a
#define O_SW1_5 0x1c
#define O_SW2 0x04
#define O_SW3 0x02
#define O_KEY_EXIT O_SW3
#define O_KEY_CLEAR O_SW2
#define O_KEY_UNDO O_SW1_5

/* new key scan code */
#define SW1_1 0x6c
#define SW1_2 0x6b
#define SW1_3 0x69
#define SW1_4 0x6a
#define SW1_5 0x6d
#define SW2 0x6e
#define SW3 0x6f
#define KEY_EXIT SW3
#define KEY_CLEAR SW2
#define KEY_UNDO SW1_5

///////cyc handler
ID cyctskid = -1;
//////

LOCAL RECT r_ext, r_clr, r_udo;
LOCAL int b0, x0, y0, exit_flag;
LOCAL BMP devbmp;
LOCAL UW PrevKeyStat;
LOCAL RECT r_upd;

///////
RECT* ptr_rect;
RECT r_ab, r_12, r_a2, r_enter, r_keyboard, r_space, r_select, r_clear, r_close, r_banner,r_draw_panel;
RECT r_output_panel, r_charac_word_menu, r_charac_box, r_word_box, r_para_box, r_draw_panel_in_menu;
RECT r_out_line0, r_out_line1, r_out_line2, r_out_line3, r_out_panel;
output_panel out_panel;
cursor cur, old_cur;
cursor begin_of_block;
cursor end_of_block, old_end_of_block;
cursor active_block;

int input_type_status; // =1: character, =2:word, =3:paragraph
int drawing_input = LOWERCASE; //=1:alphabetic, =2:number, =3:alphanumeric
//int screen_status = HOME; //=1: HOME, =2: MENU
int screen_status = HOME; //=1: HOME, =2: MENU
int from_menu_to_home_status = 0; //=1: change status from menu status to home status
int draw_status = BEFORE_DRAW;
int select_status = NO_SELECT;
int draw_timer;
int cursor_flag = 1; //=1:GRAY cursor, =0:BLACK

/////
int menu_press_count = 0;
int exit_menu_flag = 0;
int select_tool_active = DISABLE; //=0: DISABLE, =1: ENABLE
int select_direction = -1;

int error_after_finish_select = 0;
int recognize_word_after_exit_menu = 1; //=0:not recognize ; =1:recognize

//LOCAL int txt_box1[80][60];
LOCAL int txt_box1[150][210];
LOCAL const int  txt_box1_x1 = 0;
LOCAL const int  txt_box1_y1 = 140;
LOCAL const int  txt_box1_x2 = 210;
LOCAL const int  txt_box1_y2 = 290;

const int txt_box1_width = 60;
const int txt_box1_height = 80;
Image* src;
Image* temp_img;
int enter_function = 0;
//////
temp_var = 2;
//////
/*
 * initialization
 */
LOCAL void init( void )
{
    /* fill total screen in white */
    fill_rect( devbmp.bounds.c.left, devbmp.bounds.c.top,
               devbmp.bounds.c.right, devbmp.bounds.c.bottom,
               WHITE, &devbmp );
	//show_BMP("/SYS/car1.bmp", 50 , 10, &devbmp);	
	show_BMP("/SYS/recog_home.bmp", 0 , 0, &devbmp);
	show_BMP("/SYS/draw_panel.bmp",r_draw_panel.c.left ,r_draw_panel.c.top, &devbmp);

    /* draw frame */
    r_clr.c.left = devbmp.bounds.c.left;
    r_clr.c.right = r_udo.c.left = (2 * devbmp.bounds.c.left + devbmp.bounds.c.right) / 3;
    r_udo.c.right = r_ext.c.left = (devbmp.bounds.c.left + 2 * devbmp.bounds.c.right) / 3;
    r_ext.c.right = devbmp.bounds.c.right;
    r_clr.c.bottom = r_udo.c.bottom = r_ext.c.bottom = devbmp.bounds.c.bottom;
    r_clr.c.top = r_udo.c.top = r_ext.c.top = devbmp.bounds.c.bottom - 16 * 2;

	Init_cursor();
	init_draw_matrix();
	////////
	////printf("toa do cua r_ext la (%d,%d,%d,%d)\n", r_ext.c.left,r_ext.c.bottom, r_ext.c.right, r_ext.c.top);
	
	//draw_text("easy to write \n",10,50,WHITE,&devbmp);
	//draw_text(" to write",42,73,WHITE,&devbmp);
	//draw_text("012345678901234567890123456789",0,96,WHITE,&devbmp);
	//draw_text("a",232,96,WHITE,&devbmp);
	//draw_text(mystr,10,119,WHITE,&devbmp);
	
	
	//draw_text("wertyuiopasdfghjklzcxvbnm",0,49,WHITE,&devbmp);
	//char temp_c = 'a';
	/*
	draw_text(out_panel.line[0].str,0,49,WHITE,&devbmp);
	draw_text(out_panel.line[1].str,0,72,WHITE,&devbmp);
	draw_text(out_panel.line[2].str,0,95,WHITE,&devbmp);
	draw_text(out_panel.line[3].str,0,118,WHITE,&devbmp);
	*/
	
	out_panel.line[1].str[29] = ' ';
	out_panel.line[1].str[28] = ' ';
	//Clear_out_panel();
	print_out_panel();
	//Clear_line(0);

	/*
	fill_rect_v2(16,49,8,16,BLACK,&devbmp);
	fill_rect_v2(56,49,8,16,BLACK,&devbmp);
	fill_rect_v2(72,72,8,16,BLACK,&devbmp);
	fill_rect_v2(232,72,8,16,BLACK,&devbmp);
	fill_rect_v2(72,95,8,16,BLACK,&devbmp);
	fill_rect_v2(96,95,8,16,BLACK,&devbmp);
	fill_rect_v2(72,117,8,16,BLACK,&devbmp);
	fill_rect_v2(96,117,8,16,BLACK,&devbmp);
	fill_rect_v2(120,118,8,16,BLACK,&devbmp);
	fill_rect_v2(160,118,8,16,BLACK,&devbmp);
	*/
	////////

    /* initialize memory */
    ini_stk();

    /* update area is to be all */
    r_upd = devbmp.bounds;
}

/*
 * termination handling
 */
EXPORT void exitproc( void )
{
    /* erase screen */
    fill_rect( 0, 0, devbmp.bounds.c.right, devbmp.bounds.c.bottom,
               WHITE, &devbmp );
    /* set exit flag */
    exit_flag = 1;

    /* update area is to be all */
    r_upd = devbmp.bounds;
}

/*
 * Undo handling
 */
LOCAL void undo( void )
{
    int x1, y1, x2, y2;

    for(x2 = y2 = -1; ; x2 = x1, y2 = y1 ) {
        x1 = pop_stk();
        if (x1 < 0) break;
        else y1 = pop_stk();
        if (x2 == -1) { x2 = x1; y2 = y1; }
        draw_line( x1, y1, x2, y2, WHITE, &devbmp );
    }

    /* update area is to be all */
    r_upd = devbmp.bounds;
}

/*
 * PD handling
 */
LOCAL void pdproc( EVENT *evt )
{
    int x, y, b;

    if (evt -> type == EM_NULL) {
        x = evt -> pos.x;                     /*  current coordinate x */
        y = evt -> pos.y;                     /*  current coordinate y */
        b = (evt -> stat & ES_BUT) != 0;      /* whether PD button is pressed or not */
											//b0=0: first point (push the first time)
        if (!in_rect( x, y, &(devbmp.bounds))) { x = y = 0; }
		switch(screen_status)
		{
			case HOME :
				if(select_status == SELECTING && b && !in_rect( x, y, &r_out_panel )){
					//printf("select_status == SELECTING && b && !in_rect( x, y, &r_out_panel ) \n");
					select_status = FINISH_SELECT;
					error_after_finish_select = 1;
				}
				if (in_rect( x, y, &r_ab )) { 
					if (!b0 && b) //press button the first time
						ab_btn_proc(input_type_status);
				} else if (in_rect( x, y, &r_12 )) { 
					if (!b0 && b)
						btn_12_proc(input_type_status);
				} else if (in_rect( x, y, &r_a2 )) { 
					if (!b0 && b)
						a2_btn_proc(input_type_status);
				} else if (in_rect( x, y, &r_enter )) { 
					if (!b0 && b)
						enter_proc();
				} else if (in_rect( x, y, &r_keyboard )) { 
					if (!b0 && b)
						keyboard_btn_proc();
				} else if (in_rect( x, y, &r_space )) { 
					if (!b0 && b)
						space_proc();
				} else if (in_rect( x, y, &r_select )) { 
					if (!b0 && b)
						select_proc();
				} else if (in_rect( x, y, &r_clear )) { 
					if (!b0 && b)
						clear_proc();
				}else if(in_rect( x, y, &r_out_panel )) {
					//////
					////printf("r_out_panel area \n");
					if(select_tool_active == DISABLE){
						if (in_rect( x, y, &r_out_line0 )) { 
							if (!b0 && b)
							//if (b)
								out_line0_proc(x);
						} else if (in_rect( x, y, &r_out_line1 )) { 
							if (!b0 && b)
							//if (b)
								out_line1_proc(x);
						} else if (in_rect( x, y, &r_out_line2 )) { 
							if (!b0 && b)
							//if (b)
								out_line2_proc(x);
						} else if (in_rect( x, y, &r_out_line3 )) { 
							if (!b0 && b)
							//if (b)
								out_line3_proc(x);
						}
					}
					else if(select_tool_active == ENABLE){
						if(b){
							if (in_rect( x, y, &r_out_line0 )) { 
									out_line0_proc(x);
							} else if (in_rect( x, y, &r_out_line1 )) {
									out_line1_proc(x);
							} else if (in_rect( x, y, &r_out_line2 )) {
									out_line2_proc(x);
							} else if (in_rect( x, y, &r_out_line3 )) {
									out_line3_proc(x);
							}
						}
						switch(select_status){
							case NO_SELECT:
								////printf("NO_SELECT \n");
								old_end_of_block.line = -1; 
								//if(b&&in_rect( x, y, &r_out_panel )){
								if(b){
									select_status = SELECTING;
									cal_begin_block_pos(x,y);
								}
								break;
							case SELECTING:
								////printf("SELECTING \n");
								//if(b&&in_rect( x, y, &r_out_panel )){
								if(b){
									update_end_block_pos(x,y);
									call_highlight_block(x);
									update_old_end_block();
									//save old_end_block
								}
								else select_status = FINISH_SELECT;
								break;
							case FINISH_SELECT:
								////printf("FINISH_SELECT \n");
								if(b){
									//clear_selected_block();
									Unselect_active_block();
									select_status = NO_SELECT;
								}
								break;
						}
					}
					//////
				} else if (in_rect( x, y, &r_close )) { 
					if (!b0 && b){
						exitproc();
						//printf("r_close area \n");
					}
				} else if (in_rect( x, y, &r_draw_panel ) && b) {
					if(error_after_finish_select); //printf("r_draw_panel area with error_after_finish_select \n ");
					if(from_menu_to_home_status){
						from_menu_to_home_status = 0;
						recognize_word_after_exit_menu = 0;
					}
					else{
					//if (!b0 && b)
						if(error_after_finish_select){
							x0 = x; y0 = y;
							error_after_finish_select = 0;
						}
						if (!b0) {
							// begin line if PD is not pushed last time 
							////printf("aaaaaaaaa \n");
							x0 = x; y0 = y;
							psh_stk( -1 );
						}
						if (!b0 || x != x0 || y != y0) { //maybe error here
							////printf("bbbbbbbbbbb \n");
							////printf("gia tri (x0,y0) = (%d,%d) \n" );
							// draw line from previous position to current position 
							//draw_line( x0, y0, x, y, BLACK, &devbmp );
							//draw_line2( x0, y0, x, y, BLACK, &devbmp, txt_box1, txt_box1_x1,  txt_box1_y1);
							//draw_line2( x0, y0, x, y, BLACK, &devbmp, txt_box1, r_draw_panel.c.left,  r_draw_panel.c.top);
							draw_line3( x0, y0, x, y, BLACK, &devbmp, txt_box1, r_draw_panel.c.left,  r_draw_panel.c.top);
							//printf("finish draw line 3 \n");
							//////
							//if(menu_press_count != 9)
								////printf("draw_line here\n ");
							//menu_press_count = 9;
							///////
							psh_stk( y ); psh_stk( x );

					   // set update area 
					r_upd.c.left = (x0 < x) ? x0 : x;
					r_upd.c.right = ((x0 < x) ? x : x0) + 1;
					r_upd.c.top = (y0 < y) ? y0 : y;
					r_upd.c.bottom = ((y0 < y) ? y : y0) + 1;
						}
					}
					x0 = x; y0 = y;
					//b0 = b;
					
				}
				//b0 = b;
				if (in_rect( x, y, &r_draw_panel )) {
					////printf("draw_status = %d \n", draw_status);
					switch(draw_status){
							case BEFORE_DRAW:
								////printf("BEFORE_DRAW status \n");
								if(b == 1) draw_status = DRAWING;
								break;
							case DRAWING:
								////printf("DRAWING status \n");
								if(b == 0){
								draw_status = FINISH_DRAW;
								draw_timer = 0;
								}
								break;
							case FINISH_DRAW:
								////printf("FINISH_DRAW status \n");
								////printf("draw_timer = %d",draw_timer);
								if(draw_timer == 40) draw_status = RECOGNIZING;
								if(b == 0) draw_timer++;
								else draw_status = DRAWING;
								break;
							case RECOGNIZING:
								recognize();
								Clear_draw_panel();
								draw_status = BEFORE_DRAW;
								break;
							default :
								break;
						}
				}
				break;
			case MENU:
				////printf("menu area \n");
				if (in_rect( x, y, &r_keyboard) && b) {
				}
				else if (in_rect( x, y, &r_charac_box )&& !b0 && b)
					charac_box_proc(&screen_status, &input_type_status);
					//charac_box_proc_temp();
				else if (in_rect( x, y, &r_word_box )&& !b0 && b) 
					word_box_proc(&screen_status, &input_type_status);
				else if (in_rect( x, y, &r_para_box )&& !b0 && b) 
					para_box_proc(&screen_status, &input_type_status);
				else if(!b0 && b){
					exit_menu();
				}
				x0 = x; y0 = y; 
				break;
			default :
				break;
		}
		b0 = b;
		exit_menu_flag = b0;
    }
}

/*
 * key handling
 */
LOCAL void keyproc( int dd )
{
    UB KeyMap[KEYMAX/8];
    INT asiz, ercd, keystat;

#define	KeyOn(n)	(KeyMap[(n) >> 3] & (0x80 >> ((n) & 7)))

    /* read current key status */
    ercd = tk_srea_dev( dd, DN_KEYMAP, KeyMap, KEYMAX/8, &asiz );
    if (ercd < E_OK || asiz != KEYMAX/8) return;

    /* test for button press */
    keystat = 0;
    if (KeyOn(KEY_EXIT) || KeyOn(O_KEY_EXIT)) keystat |= 0x1;	/* Exit */
    if (KeyOn(KEY_CLEAR) || KeyOn(O_KEY_CLEAR)) keystat |= 0x2;	/* Clear */
    if (KeyOn(KEY_UNDO) || KeyOn(O_KEY_UNDO)) keystat |= 0x4;	/* Undo */

    /* button handling: Handle it if it is pressed in this time and not pressed in last time */
    if ((keystat & 0x1) && !(PrevKeyStat & 0x1)) exitproc();	/* Exit */
    else if ((keystat & 0x2) && !(PrevKeyStat & 0x2)) init();	/* Clear */
    else if ((keystat & 0x4) && !(PrevKeyStat & 0x4)) undo();	/* Undo */

    /* save current key status */
    PrevKeyStat = keystat;
}

/*
 * task
 */
EXPORT void task( INT stacd, VP exinf )
{
    UB      devnm[8];         /* device name */
    ID      dds, ddb;         /* device descriptor */
    PdRange range;            /* screen range */
    EVENT   evt;              /* event */
    INT     asiz;             /* read/write size */
    ER      ercd;             /* error code */

    /* open SCREEN device */
    strcpy( devnm, "SCREEN" );
    dds = tk_opn_dev( devnm, TD_UPDATE ); /* open device in update mode */
    if (dds < E_OK) goto ext;           /* fail to open */

    /* read device specific image area */
    ercd = tk_srea_dev( dds, DN_SCRBMP, &devbmp, sizeof(BMP), &asiz );
    if (ercd < E_OK || asiz != sizeof(BMP)) goto ext;

    /* color map setting */
    switch (devbmp.pixbits >> 8) { /* bit count per 1 pixel */
    case 8 :
        ercd = tk_swri_dev( dds, DN_SCRCOLOR, (VP)colormap, sizeof(colormap), &asiz );
        if (ercd < E_OK || asiz != sizeof(colormap)) goto ext;
        break;
    }

    /* open KBPD device */
    strcpy( devnm, "kbpd" );
    ddb = tk_opn_dev( devnm, TD_UPDATE );/* open device */
    if (ddb < E_OK) goto ext;            /* fail to open */

    /* write PD range */
    range.xmax = devbmp.bounds.c.right;
    range.ymax = devbmp.bounds.c.bottom;
    ercd = tk_swri_dev( ddb, DN_PDRANGE, &range, sizeof(PdRange), &asiz );
    if (ercd < E_OK || asiz != sizeof(PdRange)) goto ext;

    /* initialize screen */
	printf("recog program \n");
	createDictionary();
	initAll();
	init_rect();
	Init_out_panel(&out_panel);
    init();
    tk_swri_dev(dds, DN_SCRUPDRECT, &r_upd, sizeof(RECT), &asiz);
    x0 = y0 = 0; b0 = 0;
    PrevKeyStat = 0;

	//////cyclic 
	 T_CCYC cyctsk;
	cyctsk.exinf = (VP)0x00000000;
	//cyctsk.cycatr = TA_HLNG;
	cyctsk.cycatr = TA_HLNG | TA_RNG0;
	//  ctsk.tskatr  = TA_HLNG | TA_RNG0 | TA_FPU; 
	cyctsk.cychdr = cychdr;
	cyctsk.cyctim = 450;
	cyctsk.cycphs = 0;
	cyctskid = tk_cre_cyc(&cyctsk);
	//printf("tk_cre_cyc: (cyctskid = %d)\n", cyctskid);
	if (cyctskid < E_OK)
	{
	  tk_del_tsk(cyctskid);
	  cyctskid = -1;
	  goto ext;
	}

	//printf("This is cychandler, going for 20 time\n");
	ercd = tk_sta_cyc(cyctskid);
	if (ercd < 0)
	{
	  //printf("tk_sta_cyc error = %x\n", ercd);
	  ercd = tk_del_cyc(cyctskid);
	  cyctskid = -1;
	}
	///////

    /* event loop */
    for(exit_flag = 0; !exit_flag; ) {
		/////
		//draw_cursor(cur.line,cur.pos);
		//////
	r_upd.c.left = r_upd.c.right = r_upd.c.top = r_upd.c.bottom = 0;
        ercd = get_evt( EM_ALL, &evt, CLR );
        if (ercd >= 0) pdproc( &evt );
        keyproc( ddb );

       	/* update screen */
	if (r_upd.c.left < r_upd.c.right && r_upd.c.top < r_upd.c.bottom)
	    tk_swri_dev(dds, DN_SCRUPDRECT, &r_upd, sizeof(RECT), &asiz);

        tk_dly_tsk(10);
    }

    /* close kbpd device */
    ercd = tk_cls_dev( ddb, 0 );
    if (ercd < E_OK) goto ext;          /* fail to close */

    /* close SCREEN device */
    ercd = tk_cls_dev( dds, 0 );
    if (ercd < E_OK) goto ext;          /* fail to close */

    /* terminate and delete task */
ext:
	tk_stp_cyc(cyctskid); 
	tk_del_cyc(cyctskid);
    tk_exd_tsk();

}

EXPORT void cychdr(VP exinf)
{
	draw_cursor(cur.line,cur.pos);
	return;
}


//////////
void cal_RECT_coordinate(RECT* object, int width, int height, int left, int top){
	object->c.left = left;
	object->c.top = top;
	object->c.right = left + width;
	object->c.bottom = top + height;
}
void init_rect(){
	//RECT r_ab, r_12, r_a2, r_enter, r_keyboard, r_space, r_select, r_clear, r_close, r_banner,r_draw_panel;
	//RECT r_output_panel, r_charac_word_menu;
	//ptr_rect = &r_ab;
	cal_RECT_coordinate(&r_ab, 30, 30, 210, 140);
	cal_RECT_coordinate(&r_12, 30, 30, 210, 170);
	cal_RECT_coordinate(&r_a2, 30, 30, 210, 200);
	cal_RECT_coordinate(&r_enter, 30, 30, 210, 290);
	cal_RECT_coordinate(&r_keyboard, 30, 60, 210, 230);
	cal_RECT_coordinate(&r_space, 120, 30, 30, 290);
	cal_RECT_coordinate(&r_select, 30, 30, 0, 290);
	cal_RECT_coordinate(&r_clear, 60, 30, 150, 290);
	cal_RECT_coordinate(&r_close, 30, 30, 210, 0);
	cal_RECT_coordinate(&r_draw_panel, 210, 150, 0, 140);
	cal_RECT_coordinate(&r_charac_word_menu,115,90,125,140);
	cal_RECT_coordinate(&r_charac_box,115,30,125,140);
	cal_RECT_coordinate(&r_word_box,115,30,125,170);
	cal_RECT_coordinate(&r_para_box,115,30,125,200);
	cal_RECT_coordinate(&r_draw_panel_in_menu,85,90,125,140);
	cal_RECT_coordinate(&r_out_line0,240,23,0,43);
	cal_RECT_coordinate(&r_out_line1,240,23,0,66);
	cal_RECT_coordinate(&r_out_line2,240,22,0,89);
	cal_RECT_coordinate(&r_out_line3,240,23,0,111);
	cal_RECT_coordinate(&r_out_panel,240,91,0,43);

	input_type_status = CHARACTER;
	

	//////
	//printf("toa do cua r_12 la (%d,%d,%d,%d)\n", r_12.c.left,r_12.c.bottom, r_12.c.right, r_12.c.top);
	//printf("toa do cua r_a2 la (%d,%d,%d,%d)\n", r_a2.c.left,r_a2.c.bottom, r_a2.c.right, r_a2.c.top);
	//printf("toa do cua r_a2 la (%d,%d,%d,%d)\n", r_a2.c.left,r_a2.c.bottom, r_a2.c.right, r_a2.c.top);
	//////
	
}

void ab_btn_proc(int status){
	//if(status == CHARACTER){
	//printf("ab_btn_proc \n");
	if(status == 1){
		if(drawing_input != LOWERCASE){
		show_BMP("/SYS/ab_active.bmp",r_ab.c.left ,r_ab.c.top, &devbmp);
		//show_BMP("/SYS/12_passive.bmp",r_12.c.left ,r_12.c.top, &devbmp);
		//show_BMP("/SYS/Ab2_passive.bmp",r_a2.c.left ,r_a2.c.top, &devbmp);
		drawing_input = LOWERCASE;
		}
		else{
			show_BMP("/SYS/AB_act.bmp",r_ab.c.left ,r_ab.c.top, &devbmp);
			drawing_input = UPPERCASE;
		}
		show_BMP("/SYS/12_passive.bmp",r_12.c.left ,r_12.c.top, &devbmp);
		show_BMP("/SYS/Ab2_passive.bmp",r_a2.c.left ,r_a2.c.top, &devbmp);
	}
}

void btn_12_proc(int status){
	//printf("12_btn_proc \n");
	if(status == 1){
		if(drawing_input == UPPERCASE)
			show_BMP("/SYS/AB_pass.bmp",r_ab.c.left ,r_ab.c.top, &devbmp);
		else
			show_BMP("/SYS/ab_passive.bmp",r_ab.c.left ,r_ab.c.top, &devbmp);
		show_BMP("/SYS/12_active.bmp",r_12.c.left ,r_12.c.top, &devbmp);
		show_BMP("/SYS/Ab2_passive.bmp",r_a2.c.left ,r_a2.c.top, &devbmp);
		drawing_input = NUMBER;
	}
}

void a2_btn_proc(int status){
	//printf("a2_btn_proc \n");
	if(status == 1){
		//show_BMP("/SYS/ab_passive.bmp",r_ab.c.left ,r_ab.c.top, &devbmp);
		if(drawing_input == UPPERCASE)
			show_BMP("/SYS/AB_pass.bmp",r_ab.c.left ,r_ab.c.top, &devbmp);
		else
			show_BMP("/SYS/ab_passive.bmp",r_ab.c.left ,r_ab.c.top, &devbmp);
		show_BMP("/SYS/12_passive.bmp",r_12.c.left ,r_12.c.top, &devbmp);
		show_BMP("/SYS/Ab2_active.bmp",r_a2.c.left ,r_a2.c.top, &devbmp);
	}
	drawing_input = ALPHANUMERIC;
}

void keyboard_btn_proc(){
	//printf("keyboard_btn_proc \n");
	//show_BMP("/SYS/menu_chrac.bmp",r_charac_word_menu.c.left ,r_charac_word_menu.c.top, &devbmp);
	show_BMP("/SYS/menu_3.bmp",r_charac_word_menu.c.left ,r_charac_word_menu.c.top, &devbmp);
	/*
	if(input_type_status == CHARACTER)
		show_BMP("/SYS/menu_chrac.bmp",r_charac_word_menu.c.left ,r_charac_word_menu.c.top, &devbmp);
	else if (input_type_status == WORD)
		show_BMP("/SYS/menu_word.bmp",r_charac_word_menu.c.left ,r_charac_word_menu.c.top, &devbmp);
	else if (input_type_status == PARAGRAPH)
		show_BMP("/SYS/menu_para.bmp",r_charac_word_menu.c.left ,r_charac_word_menu.c.top, &devbmp);
	//menu_press_count = 0;
	*/
	screen_status = MENU;
}

void charac_box_proc(int* scr_status, int* input_status){
	if(!exit_menu_flag){
	//printf("charac_box_proc() area \n");
		*input_status = CHARACTER;
		show_BMP("/SYS/menu_charac.bmp",r_charac_word_menu.c.left ,r_charac_word_menu.c.top, &devbmp);
		exit_menu_update_home(*input_status);
		*scr_status = HOME;
		drawing_input = LOWERCASE;
		 from_menu_to_home_status = 1;
	}
}

void word_box_proc(int* scr_status, int* input_status){
	if(!exit_menu_flag){
	//printf("word_box_proc() area \n");
		*input_status = WORD;
		show_BMP("/SYS/menu_word.bmp",r_charac_word_menu.c.left ,r_charac_word_menu.c.top, &devbmp);
		exit_menu_update_home(*input_status);
		*scr_status = HOME;
		drawing_input = ALPHANUMERIC;
		 from_menu_to_home_status = 1;
	}
}

void para_box_proc(int* scr_status, int* input_status){
	if(!exit_menu_flag){
	//printf("para_box_proc() area \n");
		*input_status = PARAGRAPH;
		show_BMP("/SYS/menu_para.bmp",r_charac_word_menu.c.left ,r_charac_word_menu.c.top, &devbmp);
		exit_menu_update_home(*input_status);
		*scr_status = HOME;
		 from_menu_to_home_status = 1;
	}
}


void exit_menu(){
	//printf("exit_menu area \n");
	exit_menu_update_home(input_type_status);
	screen_status = HOME;
}


void delay(double milisecond){
	double i;
	for(i=0;i<=milisecond;i++) delay_1ms();
}

void delay_1ms(){
	int i,temp;
	for(i=0;i<=16666;i++)temp=0;
}
void exit_menu_update_home(int status){
	//printf("exit_menu_update_home \n");
	//printf("gia tri cua status trong exit_menu_update_home la %d \n",status);
	if(status == CHARACTER){
		show_BMP("/SYS/ab_active.bmp",r_ab.c.left ,r_ab.c.top, &devbmp);
		show_BMP("/SYS/12_passive.bmp",r_12.c.left ,r_12.c.top, &devbmp);
		show_BMP("/SYS/Ab2_passive.bmp",r_a2.c.left ,r_a2.c.top, &devbmp);
	}
	else if(status == WORD || status == PARAGRAPH){
		show_BMP("/SYS/ab_passive.bmp",r_ab.c.left ,r_ab.c.top, &devbmp);
		show_BMP("/SYS/12_passive.bmp",r_12.c.left ,r_12.c.top, &devbmp);
		show_BMP("/SYS/Ab2_active.bmp",r_a2.c.left ,r_a2.c.top, &devbmp);	
	}
	show_BMP("/SYS/draw_menu.bmp",r_draw_panel_in_menu.c.left ,r_draw_panel_in_menu.c.top, &devbmp);	
	
}


void Init_output_line(output_line* out){
	int i;
	out->length = 30;
	out->real_length = 0;
	char temp = 'a';
	//for(i=0;i<out->length;i++){
	for(i=0;i<out->length;i++){
		out->str[i] = ' ';
	}
	//out->str[29] = ' ';
}


void Init_out_panel(output_panel* out){
	int i;
	out->length = 4;
	for(i=0;i<out->length;i++){
		Init_output_line(&(out->line[i]));
	}
	out->line[0].lower_line = 65;
	out->line[0].upper_line = 49;
	out->line[1].lower_line = 88;
	out->line[1].upper_line = 72;
	out->line[2].lower_line = 111;
	out->line[2].upper_line = 95;
	out->line[3].lower_line = 134;
	out->line[3].upper_line = 118;
}
void out_line0_proc(int x){
	////printf("out_line0_proc area \n");
	int pos = x/8;
	cur.line = 0;
	cur.pos = pos;
}

void out_line1_proc(int x){
	////printf("out_line1_proc area \n");
	int pos = x/8;
	//Clear_char(1,pos);
	cur.line = 1;
	cur.pos = pos;
	//Insert_char(&out_panel, 1, pos,'z');
	//draw_text(out_panel.line[1].str,0,73,WHITE,&devbmp);
}

void out_line2_proc(int x){
	////printf("out_line2_proc area \n");
	int pos = x/8;
	cur.line = 2;
	cur.pos = pos;
}

void out_line3_proc(int x){
	////printf("out_line3_proc area \n");
	int pos = x/8;
	cur.line = 3;
	cur.pos = pos;
}

void Insert_char(output_panel* panel, int line, int pos, char a){
	panel->line[line].str[pos] = a;
}

void Clear_char(int line, int pos){ 
	//out_panel.line[line].str[pos] = ' ';
	fill_rect_v2(8*pos,out_panel.line[line].upper_line - 3,8,19,BLACK,&devbmp);
}

void fill_rect_v2(int x, int y,int width, int height,int c, BMP *p_bmp ){
	fill_rect(x, y, x + width, y + height, c, p_bmp );
}

void draw_cursor(int line, int pos){
	int i;
	////printf("draw_cursor area \n");

	//draw_line( pos*8,out_panel.line[line].upper_line - 2, pos*8, out_panel.line[line].lower_line, GRAY, &devbmp );

	//delay(2000);
	//draw_line( pos*8,out_panel.line[line].upper_line - 2, pos*8, out_panel.line[line].lower_line, BLACK, &devbmp );
	//char temp_char = (char) out_panel.line[line].str[pos];
	//draw_text(&out_panel.line[line].str[pos],pos*8, out_panel.line[line].upper_line, WHITE, &devbmp );
	//delay(2000);
	if(cursor_flag){
		////printf("GRAY cursor \n");
		draw_line( pos*8,out_panel.line[line].upper_line - 2, pos*8, out_panel.line[line].lower_line - 2, GRAY, &devbmp );
		//update screen for old cursor
		if(old_cur.line != cur.line || old_cur.pos != cur.pos ){
			draw_line( old_cur.pos*8,out_panel.line[old_cur.line].upper_line - 2, old_cur.pos*8, out_panel.line[old_cur.line].lower_line - 2, BLACK, &devbmp );
			char temp_char = (char) out_panel.line[old_cur.line].str[old_cur.pos];
			draw_text(&out_panel.line[old_cur.line].str[old_cur.pos],old_cur.pos*8, out_panel.line[old_cur.line].upper_line, WHITE, &devbmp );
		}
		cursor_flag = 0;
	}
	else{
		////printf("BLACK cursor \n");
		draw_line( pos*8,out_panel.line[line].upper_line - 2, pos*8, out_panel.line[line].lower_line - 2, BLACK, &devbmp );
	    char temp_char = (char) out_panel.line[line].str[pos];
		draw_text(&out_panel.line[line].str[pos],pos*8, out_panel.line[line].upper_line, WHITE, &devbmp );
		////
		if(old_cur.line != cur.line || old_cur.pos != cur.pos ){
			draw_line( old_cur.pos*8,out_panel.line[old_cur.line].upper_line - 2, old_cur.pos*8, out_panel.line[old_cur.line].lower_line - 2, BLACK, &devbmp );
			char temp_char = (char) out_panel.line[old_cur.line].str[old_cur.pos];
			draw_text(&out_panel.line[old_cur.line].str[old_cur.pos],old_cur.pos*8, out_panel.line[old_cur.line].upper_line, WHITE, &devbmp );
		}
		cursor_flag = 1;
	}
	old_cur.line = cur.line;
	old_cur.pos = cur.pos;
}

void Init_cursor(){
	cur.line = 0;
	cur.pos = 0;
	old_cur.line = 0;
	old_cur.pos = 0;
}


void space_proc(){
	//printf("space_proc area \n");
	if(select_tool_active == ENABLE){
		if(select_status == FINISH_SELECT){
				////printf("FINISH_SELECT : select_status == %d \n",select_status);
				Unselect_active_block();
				clear_selected_block();
				select_status = NO_SELECT;
			}
	}
	if(Is_end_line()){
		////printf("gia tri (line,pos,At_final_line)= (%d,%d,%d)  \n",cur.line, cur.pos,At_final_line());
		if(At_final_line()); //printf("At_final_line \n");
		else{
			Shift_line(cur.line,cur.pos);
			Clear_out_panel();
			print_out_panel();
			cur.line++;
			cur.pos = 0;
		}
	}
	else{
		Shift_line(cur.line,cur.pos);
		Clear_out_panel();
		print_out_panel();
		cur.pos++;
	}
}

int Is_end_line(){
	if(cur.pos == out_panel.line[0].length - 1) return 1;
	else return 0;
}	
int At_final_line(){
	if(cur.line == out_panel.length - 1) return 1;
	else return 0;
}

void shift_1_character(int line, int pos){
	int i;
	for(i=out_panel.line[line].length - 1;i>pos;i--){
		out_panel.line[line].str[i] = out_panel.line[line].str[i-1];
	}
	out_panel.line[line].str[pos] = ' ';
}

void print_out_panel(){
	////printf("print_out_panel() area \n");
	draw_text(out_panel.line[0].str,0,49,WHITE,&devbmp);
	draw_text(out_panel.line[1].str,0,72,WHITE,&devbmp);
	draw_text(out_panel.line[2].str,0,95,WHITE,&devbmp);
	draw_text(out_panel.line[3].str,0,118,WHITE,&devbmp);
}

void Clear_line(int line){
	//printf("Clear_line area \n");
	int i;
	////printf("out_panel.line[line].length = %d \n", out_panel.line[line].length);
	for(i=0;i<out_panel.line[line].length;i++){
		Clear_char(line,i);
	}
}

void Clear_out_panel(){
	int i;
	for(i=0;i<out_panel.length;i++)
		Clear_line(i);
}

void enter_proc(){
	printf("enter_proc area \n");
	//temp_val = 10;
	//printf("gia tri cua temp_val la %d \n",temp_val);
	///////
	//Shift_line(0,20);
	//Clear_out_panel();
	//print_out_panel();
	if(select_tool_active == ENABLE){
		if(select_status == FINISH_SELECT){
				////printf("FINISH_SELECT : select_status == %d \n",select_status);
				Unselect_active_block();
				clear_selected_block();
				select_status = NO_SELECT;
			}
	}
	//printf("block selected is from (%d,%d) to (%d,%d) \n",begin_of_block.line, begin_of_block.pos, active_block.line,active_block.pos);
	////printf("In array ra man hinh \n");
	//print_img(src);
	
	
	if(enter_function == 1)print_Image_to_screen(temp_img);
	else if(enter_function == 2){
		//shift_line_many_pos(cur.line,cur.pos,4);
		/*
		if(is_enough_space_for_insert_word(4,cal_distance_from_cursor_to_next_char()))
			//printf("enough space \n");
		else //printf("lack of space \n");
		*/
		if(is_no_char_to_end_line()); //printf("no_char_to_end_line \n");
		else; //printf("have char_to_end_line \n");
	}
	else if(enter_function == 4){
		//printf("enter_function == 4 : shift_1_character \n");
		shift_1_character(cur.line, cur.pos);
	}
	else{ //enter normal function
		if(!At_final_line()){
			shift_all_line(cur.line + 1);
			if(is_no_char_to_end_line()){
				//printf("enter_proc 1111111111 \n");
				clear_line_in_array(cur.line + 1);
			}
			else{
				//copy(cursot->end,line + 1)
				//printf("enter_proc 22222222222 \n");
				copy_rest_of_current_line_to_line(cur.line,cur.pos,cur.line+1);
			}
			cur.line++;
			cur.pos = 0;
			Clear_out_panel();
			print_out_panel();
		}
	}
}

void Shift_line(int line, int pos){
	if(line == 3){
		shift_1_character(line,pos);
	}
	else{
		if(out_panel.line[line].str[29] == ' ') shift_1_character(line,pos);
		else{ 
			Shift_line(line+1,0);
			out_panel.line[line+1].str[0] = out_panel.line[line].str[29];
			//out_panel.line[line+1].str[1] = ' ';
			shift_1_character(line,pos);
		}
	}
}

void recognize(){
	src = (Image*)malloc(sizeof(Image));
	createImage(src,150,210);
	copy_matrix(txt_box1, src);
	temp_img = (Image*)malloc(sizeof(Image));
	//int num_of_character;
	//char* word_result;
	//print_img(src);
//	characterRecognitionGroup1(src);
	/////

	
	if(input_type_status == CHARACTER){
		char result;
		//printf("recognize_char area \n");
		if(drawing_input == LOWERCASE) result = (char) characterRecognitionGroup10(src).character;
		else if(drawing_input == UPPERCASE) result = (char) characterRecognitionGroup4(src).character;
		else if(drawing_input == NUMBER) result = (char) characterRecognitionGroup5(src).character;
		else if(drawing_input == ALPHANUMERIC) result = (char) characterRecognitionGroup6(src).character;
		
		if(select_tool_active == ENABLE){
			if(select_status == FINISH_SELECT){
				////printf("FINISH_SELECT : select_status == %d \n",select_status);
				Unselect_active_block();
				clear_selected_block();
				select_status = NO_SELECT;
			}
			//else //printf("NO_SELECT : select_status == %d \n",select_status);;
		}
		
		Insert_char(&out_panel, cur.line, cur.pos, result);
		print_char(cur.line, cur.pos);
		//cur.line++;
		//cur.pos++;
		Increase_cursor();
		//printf("recognize_char ket qua = %c \n ",result);
	}
	else if(input_type_status == WORD){
		int i;
		//printf("recognize_word area \n");
		if(recognize_word_after_exit_menu){
			countCharacter = 0;
			//resultWord = wordRecognition(src, &countCharacter, temp_img);
			printf("ma tran src \n");
			//printImage(src);
			wordRecognition2(src);
			//printf("countCharacter = %d \n", countCharacter);
			////printf("gia tri resultWord = %c \n", resultWord[0]);
			///////
			if(select_tool_active == ENABLE){
				if(select_status == FINISH_SELECT){
					////printf("FINISH_SELECT : select_status == %d \n",select_status);
					Unselect_active_block();
					clear_selected_block();
					select_status = NO_SELECT;
				}
			}
			
			/////
			if(countCharacter){
				//**kiem tra cursor co du cho khong
				insert_word_result(countCharacter,resultWord);
			}
			else ; //printf("no recognize word result \n");
		}
		else{
			printf("no recognize_word_after_exit_menu \n");
			recognize_word_after_exit_menu = 1;
		}
	}
}

void Clear_draw_panel(){
	//printf("Clear_draw_panel() area \n");
	init_draw_matrix();
	show_BMP("/SYS/draw_panel.bmp",r_draw_panel.c.left ,r_draw_panel.c.top, &devbmp);
}

void init_draw_matrix(){
	int i,j;
	for(i=0;i<150;i++){
		for(j=0;j<210;j++) txt_box1[i][j] = 0;
	}
}

void copy_matrix(int src[150][210], Image* des){
	int i,j;
	unsigned char* ptr_des;
	for(i=0;i<150;i++){
		ptr_des = (unsigned char*) des->mData + i* des->mWidth;
		for(j=0;j<210;j++)
			ptr_des[j] = (unsigned char) src[i][j];
	}
}

//LOCAL int txt_box1[150][210];

void print_img(Image* des){
	int i,j,k;
	//printf(" 012345678901234567890123456789 \n");
	for(i=0;i<des->mHeight;i++){
	//for(i=80;i<150;i++){
		k = i%10; 
		//printf("\n");
		//printf("%d",k);
		for(j=0;j<des->mWidth;j++){
		//for(j=140;j<210;j++){
			//printf("%d",(unsigned char)*(des->mData + i*des->mWidth + j));
		}
	}
	//printf("\n");
}

/*
typedef struct _Image{
		int mHeight;
		int mWidth;
		unsigned char* mData;
}Image;*/


void print_char(int line, int pos){
	int coordinate = pos*8;
	Clear_char(line,pos);
	draw_text(&out_panel.line[line].str[pos],coordinate,out_panel.line[line].upper_line,WHITE,&devbmp);
	//printf("print_char: out_panel.line[line].upper_line = %d \n",out_panel.line[line].upper_line);
}

void Clear_many_lines_in_array(int begin_line, int begin_pos, int end_line, int end_pos){
	if(end_line == begin_line){
			int i;
			for(i=begin_pos;i<=end_pos;i++)
				clear_char_in_array(begin_line,i);
	}
	else{
		int cur_line = begin_line;
		while(cur_line!=end_line){
			
			if(cur_line == begin_line){
				int i;
				for(i=begin_pos;i<out_panel.line[begin_line].length;i++)
					clear_char_in_array(cur_line,i);
			}
			else
				clear_line_in_array(cur_line);
			cur_line++;
			
		}
		
		if(cur_line==end_line){
			int i;
			for(i=0;i<=end_pos;i++)
				clear_char_in_array(cur_line,i);
		}
	}
	/*
	int cur_line = begin_line;
	while(cur_line!=end_line){
		if(cur_line == begin_line){
			int i;
			for(i=begin_pos;i<out_panel.line[begin_line].length;i++)
				clear_char_in_array(cur_line,i);
		}
		else
			clear_line_in_array(cur_line);
		cur_line++;
	}
	if(cur_line==end_line){
		int i;
		for(i=0;i<=end_pos;i++)
			clear_char_in_array(cur_line,i);
	}
	*/
}

void clear_line_in_array(int line){
	int i;
	for(i=0;i<out_panel.line[line].length;i++){
		clear_char_in_array(line,i);
	}
}

void clear_char_in_array(int line, int pos){
	out_panel.line[line].str[pos] = ' ';
}

void clear_proc(){
	//printf("clear_proc() area \n");
	//printf("(select_tool_active,select_status) = (%d,%d) \n",select_tool_active,select_status);
	if(select_tool_active == ENABLE && select_status == FINISH_SELECT){
		//printf("block selected is from (%d,%d) to (%d,%d) \n",begin_of_block.line, begin_of_block.pos, active_block.line,active_block.pos);
		Unselect_active_block();
		clear_selected_block();
		select_status == NO_SELECT;
	}
	else{
		if(Is_begin_of_line()){
			if(At_begin_line());
			else{ 
				cur.line--;
				cur.pos=29;
				clear_char_in_array(cur.line,cur.pos);
				Clear_char(cur.line,cur.pos);
				}
		}
		else{
			cur.pos--;
			clear_char_in_array(cur.line,cur.pos);
			Clear_char(cur.line,cur.pos);
		}
	}
}

int Is_begin_of_line(){
	if(cur.pos==0) return 1;
	else return 0;
}

int At_begin_line(){
	if(cur.line == 0) return 1;
	else return 0;
}

int get_pos_in_line(int x){
	return x/8;
}

void select_proc(){
	//printf("select_proc \n");
	if(select_tool_active == DISABLE){ 
		select_tool_active = ENABLE;
		show_BMP("/SYS/select_active.bmp", r_select.c.left , r_select.c.top, &devbmp);
		//printf("ENABLE select tool \n");
	}
	else {
		Unselect_active_block();
		select_tool_active = DISABLE;
		show_BMP("/SYS/select_pass.bmp", r_select.c.left , r_select.c.top, &devbmp);
		//printf("DISABLE select tool \n");
	}
}

void Increase_cursor(){
	if(Is_end_line()){
		if(At_final_line());
		else{
		cur.line++;
		cur.pos =0;
		}
	}
	else{
		cur.pos++;
	}
}

void cal_begin_block_pos(int x,int y){
	////printf("cal_begin_block_pos area \n");
	if (in_rect( x, y, &r_out_line0 )){
		begin_of_block.line = 0;
		//begin_of_block.pos = get_pos_in_line();
	}
	else if (in_rect( x, y, &r_out_line1 )){
		begin_of_block.line = 1;
	}
	else if (in_rect( x, y, &r_out_line2 ))
		begin_of_block.line = 2;
	else if (in_rect( x, y, &r_out_line3 ))
		begin_of_block.line = 3;
	begin_of_block.pos = get_pos_in_line(x);
}

void update_end_block_pos(x,y){
	////printf("update_end_block_pos area \n");
	if (in_rect( x, y, &r_out_line0 )){
		end_of_block.line = 0;
		//begin_of_block.pos = get_pos_in_line();
	}
	else if (in_rect( x, y, &r_out_line1 )){
		end_of_block.line = 1;
	}
	else if (in_rect( x, y, &r_out_line2 ))
		end_of_block.line = 2;
	else if (in_rect( x, y, &r_out_line3 ))
		end_of_block.line = 3;
	end_of_block.pos = get_pos_in_line(x);
}



void highlight_char(int line, int pos, int c){
	//fill_rect_v2(8*pos,out_panel.line[line].upper_line - 3,8,19,c,&devbmp);
	fill_rect_v2(8*pos,out_panel.line[line].upper_line - 3 ,8,19,c,&devbmp);
}




void highlight_line(int line, int c){
	//printf("highlight_line area with color = %d \n",c);
	int i;
	////printf("out_panel.line[line].length = %d \n", out_panel.line[line].length);
	for(i=0;i<out_panel.line[line].length;i++){
		highlight_char(line,i, c);
	}
}


void highlight_block(int begin_line, int begin_pos, int end_line, int end_pos, int c){
	if(end_line == begin_line){
			int i;
			for(i=begin_pos;i<=end_pos;i++)
				highlight_char(begin_line,i,c);
	}
	else{
		int cur_line = begin_line;
		while(cur_line!=end_line){
			
			if(cur_line == begin_line){
				int i;
				for(i=begin_pos;i<out_panel.line[begin_line].length;i++)
					highlight_char(cur_line,i,c);
			}
			else
				highlight_line(cur_line,c);
			cur_line++;
			
		}
		
		if(cur_line==end_line){
			int i;
			for(i=0;i<=end_pos;i++)
				highlight_char(cur_line,i,c);
		}
	}
	print_out_panel();
}


void call_highlight_block(int x){
	////printf("call_highlight_block area \n");
	if(old_end_of_block.line != -1){
		//cursor active_block;
		active_block.line = end_of_block.line;
		if(x>=8*end_of_block.pos+4) active_block.pos = end_of_block.pos;
		else active_block.pos = end_of_block.pos - 1;
		if(begin_of_block.line == active_block.line){
			if(begin_of_block.pos<=active_block.pos){ //1
				////printf("highlight_block case 1 \n");
				select_direction = 1;
				highlight_block(begin_of_block.line, begin_of_block.pos, active_block.line, active_block.pos, GRAY);
				if(old_end_of_block.line == active_block.line){
					if(old_end_of_block.pos>active_block.pos) 
						highlight_block(active_block.line,active_block.pos,old_end_of_block.line,old_end_of_block.pos,BLACK);
				}
				else if(old_end_of_block.line > active_block.line ){
					//printf("black_block case 1.1111111 \n");
					highlight_block(active_block.line, active_block.pos,active_block.line, out_panel.line[0].length - 1, BLACK);
					highlight_block(old_end_of_block.line, 0 ,old_end_of_block.line, old_end_of_block.pos, BLACK);
				}
				else if(old_end_of_block.line < active_block.line){
					//printf("black_block case 1.22222222 \n");
					highlight_block(old_end_of_block.line, old_end_of_block.pos,old_end_of_block.line, out_panel.line[0].length - 1, BLACK);
					highlight_block(begin_of_block.line, 0 ,begin_of_block.line, begin_of_block.pos, BLACK);
				}
					
			}
			else{ //2
				////printf("highlight_block case 2 \n");
				select_direction = 2;
				if(active_block.pos!=old_end_of_block.pos)
					//printf("(old.line,old.pos,act.line,act.pos)= (%d,%d,%d,%d) \n",old_end_of_block.line,old_end_of_block.pos,active_block.line,active_block.pos);
				highlight_block(active_block.line, active_block.pos,begin_of_block.line, begin_of_block.pos, GRAY);
				if(old_end_of_block.line == active_block.line){
					if(old_end_of_block.pos<active_block.pos){
						//printf("black_block case 2 \n");
						////printf("(old.line,old.pos,act.line,act.pos)= (%d,%d,%d,%d) \n",old_end_of_block.line,old_end_of_block.pos,active_block.line,active_block.pos);
						highlight_block(old_end_of_block.line,old_end_of_block.pos,active_block.line,active_block.pos,BLACK);
					}
				}
				else if(old_end_of_block.line < active_block.line ){
					//printf("black_block case 2.1111111 \n");
					highlight_block(old_end_of_block.line, old_end_of_block.pos,old_end_of_block.line, out_panel.line[0].length - 1, BLACK);
					highlight_block(active_block.line, 0 ,active_block.line, active_block.pos, BLACK);
				}
				else if(old_end_of_block.line > active_block.line){
					//printf("black_block case 2.22222222 \n");
					highlight_block(begin_of_block.line, begin_of_block.pos,begin_of_block.line, out_panel.line[0].length - 1, BLACK);
					highlight_block(old_end_of_block.line, 0 ,old_end_of_block.line, old_end_of_block.pos, BLACK);
				}
			}
		}	
		else if(begin_of_block.line < active_block.line){ //3
			////printf("highlight_block case 3 \n");
			select_direction = 3;
			highlight_block(begin_of_block.line, begin_of_block.pos, active_block.line, active_block.pos, GRAY);
			
			if(old_end_of_block.line == active_block.line && old_end_of_block.pos > active_block.pos)
				highlight_block(active_block.line,active_block.pos,old_end_of_block.line,old_end_of_block.pos,BLACK);
			else if(old_end_of_block.line > active_block.line){
				highlight_block(active_block.line,active_block.pos,active_block.line,out_panel.line[0].length - 1,BLACK);
				highlight_block(old_end_of_block.line,0,old_end_of_block.line,old_end_of_block.pos,BLACK);
			}
			
				
		}
		else if(begin_of_block.line > active_block.line){ //4
			////printf("highlight_block case 4 \n");
			select_direction = 4;
			highlight_block(active_block.line, active_block.pos,begin_of_block.line, begin_of_block.pos, GRAY);
			if(active_block.pos!=old_end_of_block.pos ||active_block.line!=old_end_of_block.line)
					//printf("(old.line,old.pos,act.line,act.pos)= (%d,%d,%d,%d) \n",old_end_of_block.line,old_end_of_block.pos,active_block.line,active_block.pos);
			if(active_block.line == old_end_of_block.line && active_block.pos > old_end_of_block.pos){
				//printf("black_block case 4.1111111 \n");
				highlight_block(old_end_of_block.line,old_end_of_block.pos,active_block.line,active_block.pos,BLACK);
			}
			else if(active_block.line > old_end_of_block.line){
				//printf("black_block case 4.22222222 \n");
				highlight_block(old_end_of_block.line,old_end_of_block.pos,old_end_of_block.line,out_panel.line[0].length - 1,BLACK);
				highlight_block(active_block.line,0,active_block.line,active_block.pos,BLACK);
			}
		}
	}
	else{
		active_block.line = end_of_block.line;
		active_block.pos = end_of_block.pos;
	}
}


void update_old_end_block(){
	////printf("update_old_end_block \n");
	//old_end_of_block.line = end_of_block.line;
	//old_end_of_block.pos = end_of_block.pos;
	old_end_of_block.line = active_block.line;
	old_end_of_block.pos = active_block.pos;
}



void clear_selected_block(){
	//printf("clear_selected_block area \n");
	if(select_direction == 1 || select_direction == 3){
		Clear_many_lines_in_array(begin_of_block.line,begin_of_block.pos,active_block.line,active_block.pos);
		Clear_out_panel();
		print_out_panel();
		cur.line = begin_of_block.line;
		cur.pos = begin_of_block.pos;
	}
	else if(select_direction == 2 || select_direction == 4){
		Clear_many_lines_in_array(active_block.line,active_block.pos,begin_of_block.line,begin_of_block.pos);
		Clear_out_panel();
		print_out_panel();
		cur.line = active_block.line;
		cur.pos = active_block.pos;
	}
}

void Unselect_active_block(){
	//printf("Unselect_active_block area \n");
	//printf("block selected is from (%d,%d) to (%d,%d) \n",begin_of_block.line, begin_of_block.pos, active_block.line,active_block.pos);
	/*
	if(select_direction == 1 || select_direction == 3){
		highlight_block(begin_of_block.line,begin_of_block.pos,active_block.line,active_block.pos,BLACK);
	}
	else if(select_direction == 2 || select_direction == 4){
		highlight_block(active_block.line,active_block.pos,begin_of_block.line,begin_of_block.pos,BLACK);
	}
	*/
	Clear_out_panel();
	print_out_panel();
}


void insert_many_char(int line, int pos, char* str, int length){
	//printf("insert_many_char area \n");
	int i;
	
	for(i=0;i<length;i++){
		//out_panel.line[line].str[pos+i] = str[i];
		Insert_char(&out_panel, line, pos+i,str[i]);
	}
	
}

void insert_char_in_cursor(char* str, int length){
	int i;
	for(i=0;i<length;i++){
		Insert_char(&out_panel, cur.line, cur.pos,str[i]);
		Increase_cursor();
	}
}

void clear_many_char_in_screen(int line, int pos, int num){
	int i;
	for(i=pos;i<=pos+num;i++){
		Clear_char(line,i);
	}
}

void print_Image_to_screen(Image *img){
	int i,j;
	unsigned char* ptr;
	for(i=0;i<img->mHeight;i++){
		ptr = (unsigned char*)img->mData + i*img->mWidth;
		for(j=0;j<img->mWidth;j++){
			if(ptr[j]==1){
				draw_pixel(j+r_draw_panel.c.left, i+r_draw_panel.c.top,BLACK,&devbmp);
			}
		}
	}


}
void increase_cursor_many_pos(int num){
	int i;
	for(i=0;i<=num;i++){
		Increase_cursor();
	}
}

void shift_line_many_pos(int line, int pos, int num){
	//printf("shift_line_many_pos");
	int i;
	for(i=0;i<num;i++)
		Shift_line(line,pos);
	Clear_out_panel();
	print_out_panel();
}

int cal_distance_from_cursor_to_next_char(){
	int result;
	int old_cursor_pos = cur.pos;
	while(!Is_end_line() && out_panel.line[cur.line].str[cur.pos] == ' '){
		cur.pos++;
	}
	result = cur.pos - old_cursor_pos;
	cur.pos = old_cursor_pos;
	return result;
}

int is_enough_space_for_insert_word(int word_length, int blank_distance){
	if(word_length > blank_distance) return 0;
	else return 1;
}

void insert_word_result(int num, char* str){
	//printf("insert_word_result area \n");
	if(is_enough_space_for_insert_word(num, cal_distance_from_cursor_to_next_char())){
		if(is_no_char_to_end_line()){
			//printf("case 1111111111 \n");
			shift_line_many_pos(cur.line,cur.pos,num + 1);
			insert_many_char(cur.line,cur.pos,str,num);
			Insert_char(&out_panel,cur.line,cur.pos + num, ' ');
			cur.pos += num+1;
		}
		else{
			//printf("case 2222222222 \n");
			shift_line_many_pos(cur.line,cur.pos,num + 1);
			insert_many_char(cur.line,cur.pos,str,num);
			Insert_char(&out_panel,cur.line,cur.pos + num, ' ');
			cur.pos += num+1;
		}

	}
	else{
		if(is_no_char_to_end_line()){
			//printf("case 3333333333 \n");
			move_cursor_to_begin_of_next_line();
			shift_line_many_pos(cur.line,cur.pos,num);
			insert_many_char(cur.line,cur.pos,str,num);
			Insert_char(&out_panel,cur.line,cur.pos + num, ' ');
			cur.pos += num+1;
		}
		else{
			//printf("case 444444444 \n");
			shift_line_many_pos(cur.line,cur.pos,num + 1);
			if(is_enough_space_for_insert_word(num, cal_distance_from_cursor_to_next_char())){
				//printf("case 444444444.1111111 \n");
				insert_many_char(cur.line,cur.pos,str,num); //error here
				Insert_char(&out_panel,cur.line,cur.pos + num, ' ');
				cur.pos += num+1;
			}
			else{
				//printf("case 444444444.22222222 \n");
				move_cursor_to_begin_of_next_line();
				shift_line_many_pos(cur.line,cur.pos,num);
				insert_many_char(cur.line,cur.pos,str,num);
				Insert_char(&out_panel,cur.line,cur.pos + num, ' ');
				cur.pos += num+1;
			}
			
		}
	}
	Clear_out_panel();
	print_out_panel();
}

int is_no_char_to_end_line(){
	int current_pos = cur.pos;
	while(out_panel.line[cur.line].str[current_pos] == ' '){
		if(current_pos == out_panel.line[cur.line].length - 1){
			return 1;
		}
		else current_pos++;
	}
	return 0;
}

void move_cursor_to_begin_of_next_line(){
	if(At_final_line()){
		cur.line = 0;
	}
	else{
		cur.line++;
	}
	cur.pos = 0;
}

void copy_line_to_line(int src_line, int des_line){
	int i;
	for(i=0;i<out_panel.line[src_line].length;i++){
		out_panel.line[des_line].str[i] = out_panel.line[src_line].str[i];
	}	
}

void shift_all_line(int line){
	//printf("shift_all_line \n");
	int current_line = out_panel.length - 1;
	while(current_line != line){
		copy_line_to_line(current_line - 1, current_line);
		current_line--;
	}
}

void copy_rest_of_current_line_to_line(int src_line, int src_pos, int des_line){
	//printf("copy_rest_of_current_line_to_line \n");
	int i,des_pos = 0;
	for(i=src_pos; i<out_panel.line[src_line].length;i++){
		out_panel.line[des_line].str[des_pos]= out_panel.line[src_line].str[i];
		out_panel.line[src_line].str[i] = ' '; //clear rest of src_line
		des_pos++;
	}
	for(i=des_pos;i<out_panel.line[des_line].length;i++){
		out_panel.line[des_line].str[i] = ' ';
	}
}

void clear_many_char_in_array(int line, int pos, int num){
	int i;
	for(i=0;i<num;i++){
		clear_char_in_array(line,pos+i);
	}
}