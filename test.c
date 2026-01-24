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


// Memory locations
#define DISPLAY_LIST 0x230    // location that the display list must be pushed to
#define CHARSET_PTR 0x2F4     // character base

#define CHARSET_ADDR 0x3C00
// according to Ed, 0x3C00 is chosen since its 4 pages from the screen memory buffer he allocated (0x4000). S
// According to Mapping the Atari, cannot set 756 to any odd number or else we will have screen garbage.
// additionally, need to start on a page boundary (any value in the form $XX00), such as $C000
// DL_CHR40x8x1

//  #define CHARSET_ADDR // location of character set

// function declarations
void fix_displayList();
void init_charset();

// memory allocations
unsigned char ScreenMemory[760];
// unsigned char CharMap[1024];     // allocate 1024 bytes for character set



char DisplayList[] = {
    // 24 blank lines
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
    // TODO: tell ANTIC to load Graphics 2 at Screen Address, total 24 times
    DL_LMS(DL_GRAPHICS2),
    // was DL_GRAPHICS2, now DL_GRAPHICS0
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

    // variable declarations
int i;
int charStart = 0;

int main() {
   fix_displayList();
    init_charset();

    for (i = charStart; i < charStart + 5; i++) {
 ScreenMemory[i - charStart] = 0;
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

void init_charset() {
    // address of CharMap
    // unsigned int charmap_addr = (unsigned int )CharMap;
      unsigned int charmap_addr = (unsigned int )00;

    
    POKEW(CHARSET_PTR,charmap_addr);

    // test character

    // test character
    POKE(charmap_addr + 0,0b10000000);
    POKE(charmap_addr + 1,0b01000000);
    POKE(charmap_addr + 2,0b00100000);
    POKE(charmap_addr + 3,0b00010000);
    POKE(charmap_addr + 4,0b00001000);
    POKE(charmap_addr + 5,0b00000100);
    POKE(charmap_addr + 6,0b00000010);
    POKE(charmap_addr + 7,0b00000001);
}


// to compile with debug info
// cl65 --debug-info -Wl --dbgfile,test.dbg -t atari -O -g -Ln game.lbl -o test.xex test.c
