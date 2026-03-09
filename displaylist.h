#ifndef DISPLAYLIST_H_
#define DISPLAYLIST_H_

#include <atari.h>


//using antic mode 2 here, 20x12 char map.
//each char being 8x8 pixels, rendered as 8 color cycles by 16 lines
//where a color cycle is ROUGHLY the same length as two lines


//it helps to think of coarse scrolling as how many whole chars are needed to
//move



//1 HSCROL unit per pixel in this antic mode
//which means 2 chars per HSCROLL rollover; coarse scroll every second char
//i.e. if pixel x coord == 
#define HSCROL_PIXEL_RANGE 16   //2^4
#define HSCROL_PIX_R_2POW 4

//2 HSCROL units per pixel in this antic mode
#define VSCROL_PIXEL_RANGE 8    //2^3
#define VSCROL_PIX_R_2POW 3

//they are ignorable as both V and H cover atleast 8 pixels (one char)
#define COARSE_SCROLL_IGNORABLE_BITS 3

//for clearing pixel coords covered within 1 HSCROL rollover
#define IGNORE_HSCROL_PIXELS_BITMASK  0xFFF0
#define ONLY_HSCROL_PIXELS_BITMASK 0x000F

//these are different because two vscrol values is one pixel
//bitshift the pixel coord left by 1 to get vscrol value to set
#define ONLY_VSCROL_PIXELS_BITMASK  0x0007
#define IGNORE_VSCROL_PIXELS_BITMASK  0xFFF8

//x must be offset by 1 to make this work "naturally" with hscrol
//hscroll essentially moves the playspace rightwards as it decrements. 
//but +x is in the right
//we use an int here because cc65 forgets this is a signed int after applying
//the mask
#define X_PIXEL_TO_COARSE(x)                                          \
  ((int)((x - 1) & IGNORE_HSCROL_PIXELS_BITMASK) >> COARSE_SCROLL_IGNORABLE_BITS) \

#define Y_PIXEL_TO_COARSE(y) (y >> COARSE_SCROLL_IGNORABLE_BITS)


//for now lets hardcode the length
extern char DisplayList[42];

void InitDisplayList();

#endif