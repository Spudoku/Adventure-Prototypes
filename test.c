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
#define DISPLAY_LIST  0x6000

#define BLANK_8 0x70  // 8 blank lines

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

unsigned char ScreenMemory[760];

void DisplayList = {
        // first start with 24 blank lines
        BLANK_8,
        BLANK_8,
        BLANK_8,
        // tell ANTIC what mode we're using  (in this case antic 3)
        DL_LMS(DL_GRAPHICS2),
        // 
        
        ScreenMemory,
        DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2,
        DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2,
        DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2,
        DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2, DL_GRAPHICS2,
        DL_JVB + &DisplayList
    };

void install_display_list();
int main() {
    POKE(560,(unsigned int)&DisplayList);
    // install_display_list();
    printf("hello world");
    while (true) {
        sleep(1);
    }
}

// based on superbug code
void install_display_list() {
    unsigned char* dl = (unsigned char*)DISPLAY_LIST;
    unsigned int screen_address = (unsigned int)SCREEN_ADDR;

    int i, j = 0;

    // 3 blank lines
    dl[j++] = DL_BLK8;
    dl[j++] = DL_BLK8;
    dl[j++] = DL_BLK8;

    for (i = 1; i < SCREEN_HEIGHT; i++) {
        dl[j++] = 0x04; // antic mode 4
    }
    // Jump to self (JVB)
    dl[j++] = DL_JVB;
    dl[j++] = ((unsigned int)dl) & 0xFF;
    dl[j++] = ((unsigned int)dl) >> 8;

    // Set DL pointer
    POKE(560, (unsigned int)dl & 0xFF);
    POKE(561, (unsigned int)dl >> 8);


}

void edventure_display_list() {
    
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
