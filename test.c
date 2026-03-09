
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
#include "util_structs.h"
#include <assert.h>
#include "dragon.h"
#include "test_sprites.h"


#include <time.h>
#include "camera.h"

DragonEntity dragonEntities[3];




// debugging variables



// Player-missile graphics starts at 0xD000

// function declarations
void fix_displayList();
// void init_charset();
void edit_colors();

void frame_delay();

void joystick_test();

void initialize_sprite_registers();

void correct_eyecoords_test();



bool check_if_any_collision(unsigned char playerID);
void init_DLI();
// void initializeEngine();
// void initializeStaticEntities();
// void processFrameTasks();
// variable declarations
// char DisplayList[] = {
//     // 24 blank lines
//     DL_BLK8,
//     DL_BLK8,
//     DL_BLK8,
//     // TODO: tell ANTIC to load Graphics 2 at Screen Address, total 24 times
//     DL_LMS(DL_GRAPHICS2),
//     // was DL_GRAPHICS2, now DL_GRAPHICS0
//     // these 2 bytes will store the location of the screen memory
//     0x00,0x00,
//     DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
//     DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,DL_GRAPHICS2,
//     DL_JVB,
//     // These two bytes store the location of the dispalyList itself
//     0x00,0x00
//     };

    // variable declarations
int i = 0;
int charStart = 480;
// PlayerEntity playerEnt;

   int frames = 0;


//engine routines
void InitializeEngine();
void InitializeStaticEntities();
void ProcessFrameTasks();


//temp debug utils
void debug_autoMove(Transform *toMove);  //force an oscillating move


//diagnostic vars
Vector2 dir = {1, 1};
Vector2 zeroVec = {0, 0};
char theFiller[17000];    //test for worst case map fit

int main() {
 // variable declarations


    unsigned int cur_player = 0;

    struct Vector2 prev_unstuck_pos = {
        0,
        0
    };

    // end variable declarations
    InitializeJoystick();

    // 
    manual_load(&gameMap[0][0]);
    InitializeEngine();

    // ScreenMemory[30] = 1;
    set_player_horiz_position(1,SCREEN_HORIZ_CENTER,true);
    set_player_vert_position(1,SCREEN_VERT_CENTER,true);

    prev_unstuck_pos.x = playerEnt.playerEntity._eyeCoords.x;
    prev_unstuck_pos.y = playerEnt.playerEntity._eyeCoords.y;
    while (true) {
        correct_eyecoords_test();
        //process gamestate
        prev_unstuck_pos.x = playerEnt.playerEntity._eyeCoords.x -  (playerEnt.playerVelocity.x);
        prev_unstuck_pos.y = playerEnt.playerEntity._eyeCoords.y - (playerEnt.playerVelocity.y);

        ProcessFrameTasks();

        waitvsync();
        // make sure player and dragon remain on screen for now
        
        //printf("h");
        set_player_horiz_position(cur_player,playerEnt.playerEntity._eyeCoords.x, true);
        set_player_vert_position(cur_player,playerEnt.playerEntity._eyeCoords.y, true);

        set_player_vert_position(1,dragonEntities[0].myEntity._eyeCoords.y,true);
        set_player_horiz_position(1,dragonEntities[0].myEntity._eyeCoords.x,true);
       

        //process graphics
        if (check_if_any_collision(cur_player)) {
            
            // playerEnt.playerEntity._eyeCoords.x = cur_horiz_position - playerEnt.playerVelocity.x;
            // playerEnt.playerEntity._eyeCoords.y = cur_vert_position - playerEnt.playerVelocity.y;
            playerEnt.playerEntity._eyeCoords.x = prev_unstuck_pos.x ;
            playerEnt.playerEntity._eyeCoords.y = prev_unstuck_pos.y ;

        }
        GTIA_WRITE.hitclr = 1;
    }
}


//todo: return and process STATUS?


void InitializeEngine(){
    

    //init graphics
    InitDisplayList();

    //init graphics
    InitDisplayList();
    init_charset();
    edit_colors();
    setup_pmg();


    InitializeStaticEntities(); //temp function
    
    
}

//initalizes just the player for now
void InitializeStaticEntities(){
    Vector2 starting_coords = {
        120,
        60
    };
    initialize_sprite_registers();

    
    playerConstructor();

    playerEnt.playerEntity._eyeCoords.x = SCREEN_HORIZ_CENTER + 20;
    playerEnt.playerEntity._eyeCoords.y = SCREEN_VERT_CENTER;

    // initialize dragons
    
    dragonConstructor(&dragonEntities[0].myEntity, &dragonEntities[0]);
    // // pass functions to dragon 0
    entityConstructor((Entity*)&dragonEntities[0].myEntity,dragonRoutine,dragonRenderer);

    dragonEntities[0].loves = &playerEnt.playerEntity;
    dragonEntities[0].myEntity._eyeCoords = starting_coords;

    cameraConstructor(&playerEnt.playerEntity);
    
    InitializeStaticEntities();     
}


//stub for now, this will be designed later
//should produce a final gamestate...
//the idea is to have an array of frametask ptrs to run in order
void ProcessFrameTasks(){
    playerEnt.playerEntity.frameTask(&(playerEnt.playerEntity));
    dragonEntities[0].myEntity.frameTask(&(dragonEntities[0].myEntity));
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


    
    //debug_autoMove(&(playerEnt.playerEntity.transform));


    camera.cameraEntity.frameTask(&(camera.cameraEntity));
}


//WARNING: Use only on one object at a time!
void debug_autoMove(Transform *toMove){
    dir.x = ((toMove->worldCoords.x > 312) 
         || (toMove->worldCoords.x < 8)) ? -dir.x : dir.x;
    toMove->worldCoords.x += dir.x;
}

// initializes a character set
// in the future we will write code that does this somewhere else
// or does writes to it at compile time
int character = 0;
// void init_charset() {
//     // address of CharMap
//     POKE(756,((unsigned int)charset) >> 8);   // poke high byte to CHBASE register
// }

void edit_colors() {
    POKE(COLOR_REG_1,0xF3);
}


void frame_delay() {
    clock_t frame_delay_length = 17;
    clock_t end = clock() + frame_delay_length * (CLOCKS_PER_SEC / 1000);
    while(clock() < end);
}

void initialize_sprite_registers() {
    // I will write a helper function in player_missile.h
    player_sprites[PLAYER_GRAPHICS_PLAYER] = &playerSprites[0];
    player_sprites[DRAGON_GRAPHICS_PLAYER] = &dragonSprites[1];
}

// checks if a player collides with any bit other than 0 in playfield
bool check_if_any_collision(unsigned char playerID) {
    unsigned char collision = player_to_playfield_collisions(playerID);
    return collision > 0;

}

void correct_eyecoords_test() {
    // player 
    playerEnt.playerEntity._eyeCoords.x = clamp_int(playerEnt.playerEntity._eyeCoords.x, SCREEN_LEFT_BOUND,SCREEN_RIGHT_BOUND);
    playerEnt.playerEntity._eyeCoords.y = clamp_int(playerEnt.playerEntity._eyeCoords.y, SCREEN_TOP_BOUND,SCREEN_BOTTOM_BOUND);
    // dragon
}


// to compile with debug info
// cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari  -g -Ln game.lbl -o test.xex charmap.c entity.c gamemap.c player.c test.c util_input.c util.c dragon.c
