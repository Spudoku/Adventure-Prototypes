#include <atari.h>
#include <_antic.h>
#include <_atarios.h>
#include <peekpoke.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <joystick.h>
#include <unistd.h>
#include <tgi.h>

#define SCREEN_HEIGHT 24

// these addresses were from Superbug
#define SCREEN_ADDR   ((unsigned char*)0xB400)


#define BLANK_8 0x70  // 8 blank lines

// Memory locations
#define DISPLAY_LIST 0x230    // location that the display list must be pushed to

//#define BLANK_LINE 0x70
int i;

// unsigned char ScreenMemory[100];
// void DisplayList = {
//     DL_BLK8,
//     DL_BLK8,
//     DL_BLK8,
//     DL_LMS[LD_GRAPHICS2],
//     ScreenMemory,

// }
void fix_displayList();
unsigned char ScreenMemory[760];

char DisplayList[] = {
    // 24 blank lines
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
    // TODO: tell ANTIC to load Graphics 2 at Screen Address, total 24 times
    DL_LMS(DL_GRAPHICS2),
    0x00,0x00,
    DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
    DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
    DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
    DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
    DL_JVB,
    // Reference of DisplayList
    0x00,0x00
    };

void install_display_list();
int main() {
   fix_displayList();
    // install_display_list();
    
    // printf("hello world");
    ScreenMemory[0] = 33;
    while (true) {
        sleep(1);
    }
}

void fix_displayList() {
    // location of screen memory
    unsigned int scr_addr = (unsigned int)ScreenMemory;
    unsigned int dl_addr = (unsigned int)DisplayList;

    // inject screenmemory address into lms instruction
    DisplayList[4] = scr_addr & 0xFF;
    DisplayList[5] = (scr_addr >> 8) & 0xFF;

    // inject dlistAddr into the indices 28 and 29
   DisplayList[28] = dl_addr & 0xFF;
   DisplayList[29] = (dl_addr >> 8) & 0xFF;
   POKEW(DISPLAY_LIST,dl_addr);
    // return dl_addr;
}


// to compile with debug info
// cl65 --debug-info -Wl --dbgfile,test.dbg -t atari -O -g -Ln game.lbl -o test.xex test.c


// Stole this code from TankCombat.c
    // unsigned int *DLIST_ADDRESS  = (OS.sdlstl + OS.sdlsth*256);
    // unsigned char *DLIST = (unsigned char *)DLIST_ADDRESS;
    // unsigned char displayList[] = {
    //         DL_BLK8,  // 8 blank lines
    //         DL_BLK8,
    //         DL_BLK8,
    //         DL_BLK8,  // 8 blank lines
    //         // DL_BLK8,
    //         // DL_BLK8,
    //         // DL_BLK8,  // 8 blank lines
    //         // DL_BLK8,
    //         // DL_BLK8,
    //         // DL_BLK8,  // 8 blank lines

            
    // };

    // for (i = 0; i < 4; i++) {
    //     POKE(DLIST + i, displayList[i]);
    // }
