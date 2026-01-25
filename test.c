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

#define CHARSET_ADDR 0x3C
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
int charStart = 100;

int main() {
   fix_displayList();
    init_charset();

    for (i = 0; i < charStart; i++) {
        ScreenMemory[i] = 4;
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

int index;
unsigned int test ;

void init_charset() {
    // address of CharMap
    // unsigned int charmap_addr = (unsigned int )CharMap;
      unsigned int charmap_addr = (unsigned int )CHARSET_ADDR;  // this value specifies with page the address is located. multiply by 256 to locate actual characters.
      test = charmap_addr * 0x100; // page number * page size

    // set page number in CHARSET_PTR
    POKE(CHARSET_PTR,charmap_addr);

    // character 0
    index = 0;
    POKE(test  + (index * 8) + 0,0b00000000);
    POKE(test + (index * 8) +  1,0b00000000);
    POKE(test + (index * 8) +  2,0b00000000);
    POKE(test + (index * 8) +  3,0b00000000);
    POKE(test + (index * 8) +  4,0b00000000);
    POKE(test + (index * 8) +  5,0b00000000);
    POKE(test + (index * 8) +  6,0b00000000);
    POKE(test + (index * 8) +  7,0b00000000);

    index = 1;
    test = charmap_addr * 0x100;
    // character 1
    POKE(test  + (index * 8) + 0,0b10000001);
    POKE(test + (index * 8) +1,0b01000010);
    POKE(test + (index * 8) +2,0b00100100);
    POKE(test + (index * 8) +3,0b00010000);
    POKE(test + (index * 8) +4,0b00101000);
    POKE(test + (index * 8) +5,0b01000100);
    POKE(test + (index * 8) +6,0b10000010);
    POKE(test + (index * 8) +7,0b00000001);

    // character 2
    index = 2;
    POKE(test  + (index * 8) + 0,0b00011000);
    POKE(test + (index * 8) +  1,0b00100100);
    POKE(test + (index * 8) +  2,0b01000010);
    POKE(test + (index * 8) +  3,0b10000001);
    POKE(test + (index * 8) +  4,0b10000001);
    POKE(test + (index * 8) +  5,0b01000010);
    POKE(test + (index * 8) +  6,0b00100100);
    POKE(test + (index * 8) +  7,0b00011000);


    index = 3;
    POKE(test  + (index * 8) + 0,0b00011000);
    POKE(test + (index * 8) +  1,0b00100100);
    POKE(test + (index * 8) +  2,0b01000010);
    POKE(test + (index * 8) +  3,0b10000001);
    POKE(test + (index * 8) +  4,0b10000001);
    POKE(test + (index * 8) +  5,0b01000010);
    POKE(test + (index * 8) +  6,0b00100100);
    POKE(test + (index * 8) +  7,0b00011000);

    index = 4;
    POKE(test  + (index * 8) + 0,0b11111111);
    POKE(test + (index * 8) +  1,0b11111111);
    POKE(test + (index * 8) +  2,0b11100111);
    POKE(test + (index * 8) +  3,0b11000011);
    POKE(test + (index * 8) +  4,0b11000011);
    POKE(test + (index * 8) +  5,0b11100111);
    POKE(test + (index * 8) +  6,0b11111111);
    POKE(test + (index * 8) +  7,0b11111111);
}


// to compile with debug info
// cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari -O -g -Ln game.lbl -o test.xex test.c
