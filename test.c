#include <atari.h>
#include <_atarios.h>
#include <peekpoke.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <joystick.h>
#include <unistd.h>
#include "charmap.h"
#include "color_pallete.h"
#include "joystick_locations.h"
#include "player_missile.h"

#include <time.h>
// #include <tgi.h>

// #define SCREEN_HEIGHT 24


// Memory locations
#define DISPLAY_LIST 0x230    // location that the display list must be pushed to
#define CHARSET_PTR 0x2F4     // character base

// Player-missile graphics starts at 0xD000



// #define CHARSET_ADDR 0x3C
// according to Ed, 0x3C00 is chosen since its 4 pages from the screen memory buffer he allocated (0x4000). S
// According to Mapping the Atari, cannot set 756 to any odd number or else we will have screen garbage.
// additionally, need to start on a page boundary (any value in the form $XX00), such as $C000
// DL_CHR40x8x1

//  #define CHARSET_ADDR // location of character set

// function declarations
void fix_displayList();
void init_charset();
void edit_colors();

void frame_delay();

void joystick_test();

void test_player1();

void setup_pmg();

void memZero(unsigned int start, unsigned int offset);

// memory allocations
unsigned char ScreenMemory[760];





char DisplayList[] = {
    // 24 blank lines
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
    // TODO: tell ANTIC to load Graphics 2 at Screen Address, total 24 times
    DL_LMS(DL_GRAPHICS13),
    // was DL_GRAPHICS13, now DL_GRAPHICS0
    // these 2 bytes will store the location of the screen memory
    0x00,0x00,
    DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,
    DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,
    DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,
    DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,DL_GRAPHICS13,
    DL_JVB,
    // These two bytes store the location of the dispalyList itself
    0x00,0x00
    };

    // variable declarations
int i;
int charStart = 480;

int main() {
   fix_displayList();
    init_charset();
    edit_colors();
    setup_pmg();
    
    test_player1();
    // for (i = 0; i < charStart; i++) {
    //     ScreenMemory[i] = 1;
    // }
   
    while (true) {
        joystick_test();
        frame_delay();
        
        // usleep(16667); // about 16.667 milliseconds
    }
}

void joystick_test() {
    unsigned int joystick_input = (unsigned int)PEEK(JOYSTICK_REG_INPUT_0);
    
    if (joystick_input == JOYSTICK_MOVE_NOT) {
        ScreenMemory[0] = 0;
    } else if (joystick_input == JOYSTICK_MOVE_DOWN) {
        ScreenMemory[0] = 1;
    } else if (joystick_input == JOYSTICK_MOVE_UP) {
        ScreenMemory[0] = 2;
    }else if (joystick_input == JOYSTICK_MOVE_RIGHT) {
        ScreenMemory[0] = 3;
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
}

int character = 0;
void init_charset() {
    // address of CharMap
    // unsigned int charmap_addr = (unsigned int )CharMap;
    //   unsigned int charmap_addr = (unsigned int )CHARSET_ADDR;  // this value specifies with page the address is located. multiply by 256 to locate actual characters.
    
    //   test = charmap_addr * 0x100; // page number * page size
    unsigned char* charset_ptr = charset;
    unsigned int charset_page_num = (unsigned int)charset_ptr;

    POKE(CHARSET_PTR,(charset_page_num >> 8));   // poke high byte to CHBASE register

    // set page number in CHARSET_PTR
    character = 0;
    charset[character * 8 + 0] = 0b00000000;
    charset[character * 8 + 1] = 0b00000000;
    charset[character * 8 + 2] = 0b00000000;
    charset[character * 8 + 3] = 0b00000000;
    charset[character * 8 + 4] = 0b00000000;
    charset[character * 8 + 5] = 0b00000000;
    charset[character * 8 + 6] = 0b00000000;
    charset[character * 8 + 7] = 0b00000000;

    character = 1;
    // solid box
    charset[character * 8 + 0] = 0b01010101;
    charset[character * 8 + 1] = 0b01010101;
    charset[character * 8 + 2] = 0b01010101;
    charset[character * 8 + 3] = 0b01010101;
    charset[character * 8 + 4] = 0b01010101;
    charset[character * 8 + 5] = 0b01010101;
    charset[character * 8 + 6] = 0b01010101;
    charset[character * 8 + 7] = 0b01010101;

    character = 2;
    // solid box
    charset[character * 8 + 0] = 0b10101010;
    charset[character * 8 + 1] = 0b10101010;
    charset[character * 8 + 2] = 0b10101010;
    charset[character * 8 + 3] = 0b10101010;
    charset[character * 8 + 4] = 0b10101010;
    charset[character * 8 + 5] = 0b10101010;
    charset[character * 8 + 6] = 0b10101010;
    charset[character * 8 + 7] = 0b10101010;

    character = 3;
    // solid box
    charset[character * 8 + 0] = 0b11111111;
    charset[character * 8 + 1] = 0b11111111;
    charset[character * 8 + 2] = 0b11111111;
    charset[character * 8 + 3] = 0b11111111;
    charset[character * 8 + 4] = 0b11111111;
    charset[character * 8 + 5] = 0b11111111;
    charset[character * 8 + 6] = 0b11111111;
    charset[character * 8 + 7] = 0b11111111;
    

    
}

void edit_colors() {
    POKE(COLOR_REG_1,0xF3);
}


void frame_delay() {
    clock_t frame_delay_length = 17;
    clock_t end = clock() + frame_delay_length * (CLOCKS_PER_SEC / 1000);
    while(clock() < end);
}

void test_player1() {
    int x;
    // zero out everything in player/missile graphics
    memZero(0x3800,0x0400);

    // write bits to player 0 memory
    for (x = 0x200; x < 0x280; x++) {
        POKE(0x3800 + x, 0xFF);
    }
    // move player 0 to horizontal positon 120

    ScreenMemory[50] = 1;
}


// clear out offset number of bytes from start
void memZero(unsigned int start, unsigned int offset) {
    int x = 0;
    for (x = 0; x < offset; x++) {
        POKE(start + i,0);
    }
}

void setup_pmg() {
    unsigned int GRACTL = 0xD01D;
    unsigned int PMBASE = 0xD407;
    unsigned int GRPRIOR = 0x26F;
    unsigned int SDMCTL = 0x22F;
    unsigned int HPOSP0 = 0xD000  //; Horizontal position of player 0
unsigned int HPOSP1 = 0xD001  //; Horizontal position of player 1
unsigned int HPOSP2 = 0xD002  //; Horizontal position of player 2
unsigned int HPOSP3 = 0xD003  //; Horizontal position of player 3
    
    // TODO: do any setup for player missile graphics here
    // what Ed's code appears to do is:
    // store pmg label into PMBASE ($D407)
    // move 46 into SDMCTL ($22F), which sets to double-line resolution
    // move 0x3 into GRACTL ( $D01D ), which enables PMG
    // move 0x1 into GRPRIOR ($26F), which gives player priorty?
    // set all player location registers to 120
    unsigned int playerData = 0x38;
    POKE(PMBASE,playerData);
    POKE(SDMCTL,46); // I think the does: enable fetching DMA instructions, enable player/missile DMA, standard playfield
    POKE(GRACTL,3);
    POKE(GRPRIOR,1);
    ScreenMemory[3] = 1;
    POKE(HPOSP0,120);
}

// to compile with debug info
// cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari -O -g -Ln game.lbl -o test.xex test.c
