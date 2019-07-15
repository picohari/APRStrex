#ifndef _MENU_H_
#define _MENU_H_



#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include <cfg/compiler.h>





//void menu_navigate(menu_item_t* const new_menu, const Bitmap *bm, bool update);

void statusbar_display(void);


void menu_home(void);

void menu_process(void);

void menu_init(void);


//extern menu_item_t* ml_aprs_menu_items[5];


#if !CPU_HARVARD
void aprs_basis_info_display(void);
void aprs_position_info_display(void);
void gps_basis_info_display(void);
void demo_menu_info_display(void);
#endif


#endif
