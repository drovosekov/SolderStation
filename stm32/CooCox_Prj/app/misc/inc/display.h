#ifndef _DISPLAY_SLD
#define _DISPLAY_SLD

#include "main.h"

void draw_logo(void);
void printSolderInfoLCD(u16 *solderT);
void printFenInfoLCD(u16 *airT);
void init_user_chars(void);

void print_temp_val(u16 *temp, u16 *presetTemp, u8 cursor);

void print_dust_clck(u8 row);

#endif
