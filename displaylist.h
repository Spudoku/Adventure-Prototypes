#ifndef DISPLAYLIST_H_
#define DISPLAYLIST_H_

#include <atari.h>

char DisplayList[] = {
    // 24 blank lines
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
    
    
    //eventually work up to DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2)))
    //for the fine scrolling
    //LMS will be needed for each dl entry
    //and a mem address for the stream
    
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    //second group of 2

    
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,


    //loop the display list
    DL_JVB,
    0x00,0x00 //dl loc
    };


#endif