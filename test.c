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
#include "player.h"



#include <time.h>
// #include <tgi.h>

// #define SCREEN_HEIGHT 24


// Memory locations
#define DISPLAY_LIST 0x230    // location that the display list must be pushed to
#define CHARSET_PTR 0x2F4     // character base


// memory allocations


#pragma bss-name (push, "SCREENAREA")
unsigned char ScreenMemory[960];

#pragma bss-name (pop)
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

// wait until VCOUNT == 0
void wait_vblank() {
    while(ANTIC.vcount);
}

bool check_if_any_collision(unsigned char player);
void init_DLI();

// variable declarations
char DisplayList[] = {
    // 24 blank lines
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
    // TODO: tell ANTIC to load Graphics 2 at Screen Address, total 24 times
    DL_LMS(DL_GRAPHICS12),
    // was DL_GRAPHICS12, now DL_GRAPHICS0
    // these 2 bytes will store the location of the screen memory
    0x00,0x00,
    DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,
    DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,
    DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,
    DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,DL_GRAPHICS12,
    DL_JVB,
    // These two bytes store the location of the dispalyList itself
    0x00,0x00
    };

    // variable declarations
int i;
int charStart = 480;

int main() {

    // variable declarations
    int frames = 0;
    int i = 0;
    unsigned int cur_player = 0;
    // unsigned int cur_horiz_position;
    // unsigned int cur_vert_position;
    // end variable declarations

    initializeEngine();



    ScreenMemory[20] = 3;
    ScreenMemory[39] = 1;
    ScreenMemory[519] = 1;

    

    set_player_horiz_position(0,SCREEN_HORIZ_CENTER,true);
    set_player_vert_position(0,SCREEN_VERT_CENTER,true);
    while (true) {

        //process gamestate
        // cur_horiz_position = player_horiz_positions[cur_player];
        // cur_vert_position = player_vert_positions[cur_player];


        
        processFrameTasks();

        waitvsync();
        
        
        
        //process graphics
        if (check_if_any_collision(cur_player)) {
            GTIA_WRITE.hitclr = 1;
            set_player_horiz_position(cur_player,player.playerEntity.eyeCoords.x,true);
            set_player_vert_position(cur_player,player.playerEntity.eyeCoords.y,true);
        }
        
    }
}

//todo: return and process STATUS?

void initializeEngine(){
    fix_displayList();
    init_charset();
    edit_colors();
    setup_pmg();

    initializeStaticEntities(); //temp function
    
    
}

//initalizes just the player for now
void initializeStaticEntities(){
    test_player1();
    playerConstructor();

    //temp init assign
    player.playerEntity.eyeCoords.x = SCREEN_HORIZ_CENTER;
    player.playerEntity.eyeCoords.y = SCREEN_VERT_CENTER;
}

//stub for now, this will be designed later
//should produce a final gamestate...
//the idea is to have an array of frametask ptrs to run in order
void processFrameTasks(){
    player.playerEntity.frameTask(&(player.playerEntity));
}

// void joystick_test() {
//     unsigned int joystick_input = (unsigned int)PEEK(JOYSTICK_REG_INPUT_0);
//     unsigned int cur_player = 0;
//     // move player 0 aroud

    
//     switch (joystick_input) {
//         case JOYSTICK_MOVE_NOT: 
            
//             break;
        
//         case JOYSTICK_MOVE_DOWN:
//             move_player_vert_position(cur_player,1,true);
//             break;
        
//         case JOYSTICK_MOVE_DOWN_LEFT:
//             move_player_vert_position(cur_player,1,true);
//             move_player_horiz_position(cur_player,-1,true);
//             break;
        
//         case JOYSTICK_MOVE_LEFT:

//             move_player_horiz_position(cur_player,-1,true);
//             break;

//         case JOYSTICK_MOVE_UP_LEFT:
//             move_player_vert_position(cur_player,-1,true);
//             move_player_horiz_position(cur_player,-1,true);
//             break;

//         case JOYSTICK_MOVE_UP:
//             move_player_vert_position(cur_player,-1,true);
            
//             break;
        
//         case JOYSTICK_MOVE_UP_RIGHT:
//             move_player_vert_position(cur_player,-1,true);
//             move_player_horiz_position(cur_player,1,true);
            
            
//             break;
        
//         case JOYSTICK_MOVE_RIGHT:
            
//             move_player_horiz_position(cur_player,1,true);
//             break;

//         case JOYSTICK_MOVE_DOWN_RIGHT:
//             move_player_vert_position(cur_player,1,true);
//             move_player_horiz_position(cur_player,1,true);
//             break;

//         default:
//             break;
//     }

//     // if there's a collision, revert to previous position?
    
// }

// writes crucial bytes to the display list
void fix_displayList() {    
    // location of screen memory
    unsigned int scr_addr = (unsigned int)ScreenMemory; // address of screen memory array
    unsigned int dl_addr = (unsigned int)DisplayList; // address of display list

    // inject screenmemory address into lms instruction
    DisplayList[4] = scr_addr & 0xFF;
    DisplayList[5] = (scr_addr >> 8) & 0xFF;

    // inject dlistAddr into the indices 28 and 29
    DisplayList[sizeof(DisplayList) - 2] = dl_addr & 0xFF;   // this injects the 8 most significant bits
    DisplayList[sizeof(DisplayList) - 1] = (dl_addr >> 8) & 0xFF; // this injects the 8 least signifcant bits
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
    charset[character * 8 + 1] = 0b11000011;
    charset[character * 8 + 2] = 0b11000011;
    charset[character * 8 + 3] = 0b11000011;
    charset[character * 8 + 4] = 0b11000011;
    charset[character * 8 + 5] = 0b11000011;
    charset[character * 8 + 6] = 0b11000011;
    charset[character * 8 + 7] = 0b11000011;
    

    
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
    player_sprites[0][0] = 0x81;
    player_sprites[0][1] = 0x81;
    player_sprites[0][2] = 0x81;
    player_sprites[0][3] = 0xFF;
    player_sprites[0][4] = 0xFF;
    player_sprites[0][5] = 0xFF;
    player_sprites[0][6] = 0xFF;
    player_sprites[0][7] = 0xA1;
    player_sprites[0][8] = 0xFF;
    player_sprites[0][9] = 0xFF;
    player_sprites[0][10] = 0xFF;
    player_sprites[0][11] = 0xFF;
    player_sprites[0][12] = 0xFF;
    player_sprites[0][13] = 0xFF;
    player_sprites[0][14] = 0xFF;
    player_sprites[0][15] = 0xFF;
  
    // set_player_vert_position(0,64,true);

    player_sprites[1][0] = 0xFA;
    player_sprites[1][1] = 0xF1;
    player_sprites[1][2] = 0xF1;
    player_sprites[1][3] = 0xFA;
    player_sprites[1][4] = 0xFA;
    player_sprites[1][5] = 0xFA;
    player_sprites[1][6] = 0xFA;
    player_sprites[1][7] = 0xFA;
}

// checks if a player collides with any bit other than 0 in playfield
bool check_if_any_collision(unsigned char player) {
    unsigned char collision;

    // read the correct collision register based on player number
    switch (player) {
        case 0:
            collision = GTIA_READ.p0pf;
            break;
        case 1:
            collision = GTIA_READ.p1pf;
            break;
        case 2:
            collision = GTIA_READ.p2pf;
            break;
        case 3:
            collision = GTIA_READ.p3pf;
            break;
        default:
            collision = GTIA_READ.p0pf;
            break;
    }

    if (collision & 0x01 || collision & 0x02 || collision & 0x04) {
        return true;
    }
    return false;

}


// to compile with debug info
// cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari -O -g -Ln game.lbl -o test.xex test.c
