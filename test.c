#include <atari.h>
#include <_atarios.h>
#include <peekpoke.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <joystick.h>
#include <unistd.h>
// #include <tgi.h>

// #define SCREEN_HEIGHT 24

// these addresses were from Superbug
#define SCREEN_ADDR   ((unsigned char*)0xB400)

// Memory locations
#define DISPLAY_LIST 0x230    // location that the display list must be pushed to



// function declarations
void fix_displayList();
unsigned char ScreenMemory[760];

// variable declarations
int i;

char DisplayList[] = {
    // 24 blank lines
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
    // TODO: tell ANTIC to load Graphics 2 at Screen Address, total 24 times
    DL_LMS(DL_GRAPHICS2),
    // these 2 bytes will store the location of the screen memory
    0x00,0x00,
    DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
    DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
    DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
    DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
    DL_JVB,
    // These two bytes store the location of the dispalyList itself
    0x00,0x00
    };

int main() {
   fix_displayList();


    for (i = 33; i < 33 + 26; i++) {
 ScreenMemory[i - 33] = i;
    }
   
    while (true) {
        sleep(1);
    }
}

void fix_displayList() {
    // location of screen memory
    unsigned int scr_addr = (unsigned int)ScreenMemory; // address of screen memory array
    unsigned int dl_addr = (unsigned int)DisplayList; // address of display list

    // inject screenmemory address into lms instruction
    DisplayList[4] = scr_addr & 0xFF;
    DisplayList[5] = (scr_addr >> 8) & 0xFF;

    // inject dlistAddr into the indices 28 and 29
   DisplayList[28] = dl_addr & 0xFF;   // this injects the 8 most significant bits
   DisplayList[29] = (dl_addr >> 8) & 0xFF; // this injects the 8 least signifcant bits
   POKEW(DISPLAY_LIST,dl_addr);
    // return dl_addr;
}


// to compile with debug info
// cl65 --debug-info -Wl --dbgfile,test.dbg -t atari -O -g -Ln game.lbl -o test.xex test.c
