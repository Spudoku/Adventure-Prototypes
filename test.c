
#include <atari.h>
#include <6502.h>
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
#include "screen_memory.h"
#include "gamemap.h"
#include "player.h"

#include "util.h"
#include "util_input.h"
#include <assert.h>
#include "dragon.h"


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

void correct_eyecoords_test();


//void waitvsync(void);
//extern PlayerEntity playerEnt;

// wait until VCOUNT == 0
void wait_vblank() {
    while(ANTIC.vcount);
}

bool check_if_any_collision(unsigned char playerID);
void init_DLI();
void initializeEngine();
void initializeStaticEntities();
void processFrameTasks();
// variable declarations
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
    DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
    DL_JVB,
    // These two bytes store the location of the dispalyList itself
    0x00,0x00
    };

    // variable declarations
int i = 0;
int charStart = 480;
PlayerEntity playerEnt;

   int frames = 0;


//char TheFiller[28800];
int main() {
 // variable declarations
    unsigned int cur_horiz_position;
    unsigned int cur_vert_position;
    unsigned int cur_player = 0;
    // end variable declarations
    InitializeJoystick();


    initializeEngine();

    // 
    manual_load(&gameMap[0][0]);

    ScreenMemory[30] = 1;
    // set_player_horiz_position(0,SCREEN_HORIZ_CENTER,true);
    // set_player_vert_position(0,SCREEN_VERT_CENTER,true);
    //processFrameTasks();

    //memset(TheFiller, 3, sizeof(TheFiller)); 
    while (true) {
        // correct_eyecoords_test();
        //process gamestate
        // cur_horiz_position = playerEnt.playerEntity.eyeCoords.x;
        // cur_vert_position = playerEnt.playerEntity.eyeCoords.y;

        // processFrameTasks();

        waitvsync();
        // make sure player and dragon remain on screen for now
        
        //printf("h");
        // set_player_horiz_position(cur_player,playerEnt.playerEntity.eyeCoords.x,false);
        // set_player_vert_position(cur_player,playerEnt.playerEntity.eyeCoords.y,false);

        // set_player_horiz_position(1,dragonEnt.dragonEntity.eyeCoords.x,false);
        // set_player_vert_position(1,dragonEnt.dragonEntity.eyeCoords.y,false);
        set_player_horiz_position(0,SCREEN_HORIZ_CENTER,true);
        set_player_vert_position(0,SCREEN_VERT_CENTER,false);
        //process graphics
        // if (check_if_any_collision(cur_player)) {
        //     GTIA_WRITE.hitclr = 1;
        //     playerEnt.playerEntity.eyeCoords.x = cur_horiz_position;
        //     playerEnt.playerEntity.eyeCoords.y = cur_vert_position;
        //     playerEnt.playerVelocity.x = 0;
        //     playerEnt.playerVelocity.y = 0;
        // }
        
    }
}


//todo: return and process STATUS?

void initializeEngine(){
    
    fix_displayList();
    init_charset();
    edit_colors();
    setup_pmg();

    // fill_column(3,5);
    // fill_row(0,2);
    // fill_row(1,1);

    initializeStaticEntities(); //temp function
    
    
}

//initalizes just the player for now
void initializeStaticEntities(){
    test_player1();
    //Entity *test = (Entity*)&(playerEnt.playerEntity);


    // playerEnt.playerEntity.eyeCoords.x = SCREEN_HORIZ_CENTER;
    // playerEnt.playerEntity.eyeCoords.y = SCREEN_VERT_CENTER;


    entityConstructor((Entity*)&playerEnt.playerEntity, playerRoutine, playerRenderer);
    playerConstructor();

    //temp init assign
    // playerEnt.playerEntity.eyeCoords.x = SCREEN_HORIZ_CENTER;
    // playerEnt.playerEntity.eyeCoords.y = SCREEN_VERT_CENTER;

    playerEnt.playerEntity.eyeCoords.x = 30;
    playerEnt.playerEntity.eyeCoords.y = 30;

    set_player_horiz_position(0,playerEnt.playerEntity.eyeCoords.x,true);
    set_player_vert_position(0,playerEnt.playerEntity.eyeCoords.y,true);

    // entityConstructor((Entity*)&dragonEnt.dragonEntity,dragonRoutine, dragonRenderer);
    // dragonConstructor();
    // dragonEnt.dragonEntity.eyeCoords.x = 30;
    // dragonEnt.dragonEntity.eyeCoords.y = 0;
}

//stub for now, this will be designed later
//should produce a final gamestate...
//the idea is to have an array of frametask ptrs to run in order
void processFrameTasks(){
    playerEnt.playerEntity.frameTask(&(playerEnt.playerEntity));
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
    DisplayList[sizeof(DisplayList) - 2] = dl_addr & 0xFF;   // this injects the 8 most significant bits
    DisplayList[sizeof(DisplayList) - 1] = (dl_addr >> 8) & 0xFF; // this injects the 8 least signifcant bits
    //POKEW(DISPLAY_LIST,dl_addr);
    OS.sdlst = DisplayList;


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

    POKE(756,((unsigned int)charset) >> 8);   // poke high byte to CHBASE register



    // set page number in CHARSET_PTR
   
    

    
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
    player_sprites[0][0] =  0b00000000;
    player_sprites[0][1] =  0b00000000;
    player_sprites[0][2] =  0b00000000;
    player_sprites[0][3] =  0b00000000;
    player_sprites[0][4] =  0b00000000;
    player_sprites[0][5] =  0b00111100;
    player_sprites[0][6] =  0b01111110;
    player_sprites[0][7] =  0b01111110;
    player_sprites[0][8] =  0b01111110;
    player_sprites[0][9] =  0b01111110;
    player_sprites[0][10] = 0b00111100;
    player_sprites[0][11] = 0b00000000;
    player_sprites[0][12] = 0b00000000;
    player_sprites[0][13] = 0b00000000;
    player_sprites[0][14] = 0b00000000;
    player_sprites[0][15] = 0b00000000;
  
    // set_player_vert_position(0,64,true);
    // dragon sprite
    player_sprites[1][0] =  0b00000110;
    player_sprites[1][1] =  0b00001111;
    player_sprites[1][2] =  0b11110011;
    player_sprites[1][3] =  0b11111110;
    player_sprites[1][4] =  0b00000100;
    player_sprites[1][5] =  0b00011110;
    player_sprites[1][6] =  0b00111111;
    player_sprites[1][7] =  0b01110011;
    player_sprites[1][8] =  0b01100011;
    player_sprites[1][9] =  0b01100011;
    player_sprites[1][10] = 0b01100111;
    player_sprites[1][11] = 0b00111110;
    player_sprites[1][12] = 0b00011100;
    player_sprites[1][13] = 0b00011100;
    player_sprites[1][14] = 0b00001111;
    player_sprites[1][15] = 0b00000111;
}

// checks if a player collides with any bit other than 0 in playfield
bool check_if_any_collision(unsigned char playerID) {
    unsigned char collision;

    // read the correct collision register based on player number
    switch (playerID) {
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

void correct_eyecoords_test() {
    // player 
    playerEnt.playerEntity.eyeCoords.x = clamp_int(playerEnt.playerEntity.eyeCoords.x, SCREEN_LEFT_BOUND,SCREEN_RIGHT_BOUND);
    playerEnt.playerEntity.eyeCoords.y = clamp_int(playerEnt.playerEntity.eyeCoords.y, SCREEN_TOP_BOUND,SCREEN_BOTTOM_BOUND);
    // dragon
}


// to compile with debug info
// cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari  -g -Ln game.lbl -o test.xex charmap.c entity.c gamemap.c player.c test.c util_input.c util.c dragon.c
