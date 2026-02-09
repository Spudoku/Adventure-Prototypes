#ifndef SCREEN_MEMORY_H
#define SCREEN_MEMORY_H
#include "player_missile.h"  // for screen bounds
// methods for accessing and filling screen memory, mostly for testing


void get_row(unsigned int index); 
void get_column(unsigned int index);

void fill_row(unsigned int index, unsigned char character);
void fill_column(unsigned int index, unsigned char character);

// these ones put the "fun" in "function"
void fill_rect(unsigned int vert_pos,unsigned int horiz_pos,unsigned int l, unsigned int w, bool hollow);
void fill_circle(unsigned int vert_center,unsigned int horiz_center, unsigned int radius, bool hollow);


#endif