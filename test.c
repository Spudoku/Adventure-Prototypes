#include <atari.h>
#include <6502.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <joystick.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>


#include "charmap.h"
#include "color_pallete.h"
#include "joystick_locations.h"
#include "player_missile.h"
#include "screen_memory.h"
#include "gamemap.h"
#include "player.h"
#include "util.h"
#include "util_input.h"
#include "gfx.h"
#include "camera.h"




//legacy functions
void edit_colors();
void test_player1();
bool check_if_any_collision(unsigned char playerID);
//legacy vars
unsigned int cur_player = 0; 


//engine routines
void InitializeEngine();
void InitializeStaticEntities();
void ProcessFrameTasks();

void initialize_sprite_registers();


//temp debug utils
void debug_autoMove(Transform *toMove);  //force an oscillating move


//diagnostic vars
Vector2 dir = {1, 1};
Vector2 zeroVec = {0, 0};
char theFiller[17000];    //test for worst case map fit

int main() {

    struct Vector2 prev_unstuck_pos = {
        0,
        0
    };




    //redirect stdout to altirra printer
    //for some reason this makes the top line bug out when x is negative
    //A PRINT IS EXPENSIVE 
    //freopen("P1:", "w", stdout);
    //printf("hi!\n");

    InitializeJoystick();
    InitializeEngine();


    // set_player_horiz_position(0,SCREEN_HORIZ_CENTER,true);
    // set_player_vert_position(0,SCREEN_VERT_CENTER,true);

    while (true) {
        
        // test code
        prev_unstuck_pos.x = playerEnt.playerEntity._worldCoords.x -  (playerEnt.playerVelocity.x);
        prev_unstuck_pos.y = playerEnt.playerEntity._worldCoords.y - (playerEnt.playerVelocity.y);

        
        //process gamestate
        ProcessFrameTasks();

        waitvsync();

        //test render code assumes player never goes off screen
        GTIA_WRITE.hposp0 = playerEnt.playerEntity._eyeCoords.x 
            + HPOSP_MIN + playerEnt.playerEntity._objectAnchorPoint.x;
        camera.cameraEntity.renderer(&camera.cameraEntity);


        set_player_vert_position(cur_player,
                playerEnt.playerEntity._eyeCoords.y + V_MIN - 
                    playerEnt.playerEntity._objectAnchorPoint.y ,true);
    
        
  
       if (check_if_any_collision(cur_player)) {
            
            // playerEnt.playerEntity._eyeCoords.x = cur_horiz_position - playerEnt.playerVelocity.x;
            // playerEnt.playerEntity._eyeCoords.y = cur_vert_position - playerEnt.playerVelocity.y;
            playerEnt.playerEntity._worldCoords.x = prev_unstuck_pos.x ;
            playerEnt.playerEntity._worldCoords.y = prev_unstuck_pos.y ;

        }
        GTIA_WRITE.hitclr = 1;
        
    }
}


//todo: return and process STATUS?

void InitializeEngine(){
    

    //init graphics
    InitDisplayList();
    init_charset();
    edit_colors();
    setup_pmg();

    InitializeStaticEntities();     
}


//initalizes just the player for now
void InitializeStaticEntities(){
    Vector2 starting_coords = {
        120,
        60
    };
    initialize_sprite_registers();

    
    playerConstructor();



    playerEnt.playerEntity._worldCoords.y = 94;
    playerEnt.playerEntity._worldCoords.x = 156;

        playerEnt.playerEntity._eyeCoords.x = SCREEN_HORIZ_CENTER + 20;
    playerEnt.playerEntity._eyeCoords.y = SCREEN_VERT_CENTER;


    // initialize dragon 0
    
    // dragonConstructor(&dragonEntities[0].myEntity, &dragonEntities[0]);
    // entityConstructor((Entity*)&dragonEntities[0].myEntity,dragonRoutine,dragonRenderer);

    // dragonEntities[0].loves = &playerEnt.playerEntity;
    // dragonEntities[0].myEntity._eyeCoords = starting_coords;

    cameraConstructor(&playerEnt.playerEntity);
    
        
}
//stub for now, this will be designed later
//should produce a final gamestate...
//the idea is to have an array of frametask ptrs to run in order
void ProcessFrameTasks(){
    playerEnt.playerEntity.frameTask(&(playerEnt.playerEntity));

    
    //debug_autoMove(&(playerEnt.playerEntity.transform));


    camera.cameraEntity.frameTask(&(camera.cameraEntity));
}


//WARNING: Use only on one object at a time!
void debug_autoMove(Transform *toMove){
    dir.x = ((toMove->worldCoords.x > 312) 
         || (toMove->worldCoords.x < 8)) ? -dir.x : dir.x;
    toMove->worldCoords.x += dir.x;
}


void edit_colors() {
    POKE(COLOR_REG_1,0xF3);
}


void initialize_sprite_registers() {
    // I will write a helper function in player_missile.h
    player_sprites[PLAYER_GRAPHICS_PLAYER] = &playerSprites[0];
    // player_sprites[DRAGON_GRAPHICS_PLAYER] = &dragonSprites[1];
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


// to compile with debug info
// cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari -O -g -Ln game.lbl -m mapfile.txt -o test.xex test.c charmap.c
