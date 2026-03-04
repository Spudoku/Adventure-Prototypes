#ifndef DISPLAYLIST_H_
#define DISPLAYLIST_H_

#include <atari.h>

#define HSCROL_PIXEL_RANGE 16   //2^4
#define HSCROL_PIX_R_2POW 4
 
#define VSCROL_PIXEL_RANGE 8    //2^3
#define VSCROL_PIX_R_2POW 3

#define HSCROL_PR_INT_BITMASK 0x000F
#define HSCROL_PR_INT_BITMASK_INV  0xFFF0

//these are different because two vscrol values is one pixel
//bitshift the pixel coord left by 1 to get vscrol value to set
#define VSCROL_PR_INT_BITMASK 0x0007
#define VSCROL_PR_INT_BITMASK_INV  0xFFF8

//macro to make this clearer
//the bitshift left 1 is to allow increments of two
#define X_PIXEL_TO_COARSE(x) (((x - 1) >> HSCROL_PIX_R_2POW) << 1)
#define Y_PIXEL_TO_COARSE(y) (y >> VSCROL_PIX_R_2POW)


//for now lets hardcode the length
extern char DisplayList[42];

void InitDisplayList();

#endif