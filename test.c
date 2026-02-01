#include <atari.h>
#include <_atarios.h>
#include <peekpoke.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <joystick.h>
#include <unistd.h>
#include <_gtia.h>
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
    int frames = 0;
    fix_displayList();
    init_charset();
    edit_colors();
    setup_pmg();
    
    test_player1();
    // for (i = 0; i < charStart; i++) {
    //     ScreenMemory[i] = 1;
    // }
    set_player_horiz_position(0,48,true);
    
    while (true) {
        joystick_test();

        frame_delay();

    }
}

void joystick_test() {
    unsigned int joystick_input = (unsigned int)PEEK(JOYSTICK_REG_INPUT_0);
    // move player 0 aroud
    if (joystick_input == JOYSTICK_MOVE_NOT) {
        
    } else if (joystick_input == JOYSTICK_MOVE_DOWN) {
        
        move_player_vert_position(0,1,true);
    } else if (joystick_input == JOYSTICK_MOVE_UP) {
        
        move_player_vert_position(0,-1,true);
    }else if (joystick_input == JOYSTICK_MOVE_RIGHT) {
        
        move_player_horiz_position(0,1,true);
    } else if (JOYSTICK_MOVE_LEFT) {
        move_player_horiz_position(0,-1,true);
    }
}

// writes crucial bytes to the display list
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

// initializes a character set
// in the future we will write code that does this somewhere else
// or does writes to it at compile time
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
    // I will write a helper function in player_missile.h
    player0graphics[60] = 0xFF;
    player0graphics[61] = 0xFF;
    player0graphics[62] = 0xFF;
    player0graphics[63] = 0xFF;
    player0graphics[64] = 0xFF;
    player0graphics[65] = 0xFF;
    player0graphics[66] = 0xFF;
    player0graphics[67] = 0xFF;
}


// clear out offset number of bytes from start




// to compile with debug info
// cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari -O -g -Ln game.lbl -o test.xex test.c
