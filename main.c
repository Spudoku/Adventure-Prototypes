
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
#include "gfx.h"
#include <time.h>
#include "camera.h"
#include "items.h"

#include "sound.h"




//legacy functions
void edit_colors();
void test_player1();
bool check_if_any_collision(unsigned char playerID);
//legacy vars
unsigned int cur_player = 0; 

// testing reverse_byte function
// unsigned char testByte = 0b00110010;


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
    


    //redirect stdout to altirra printer
    //for some reason this makes the top line bug out when x is negative
    //A PRINT IS EXPENSIVE 
    //freopen("P1:", "w", stdout);
    //printf("hi!\n");

    InitializeJoystick();
    InitializeEngine();


    // set_player_horiz_position(0,SCREEN_HORIZ_CENTER,true);
    // set_player_vert_position(0,SCREEN_VERT_CENTER,true);
    // play_sound(0,255,10,3,120);

    // testByte = reverse_char(testByte);
    while (true) {
        update_voice_frames();
        //process gamestate
        ProcessFrameTasks();
        //PRINT_VEC2(playerEnt.playerEntity._worldCoords)
        //printf("aaa");
        waitvsync();

        //test render code assumes player never goes off screen
        GTIA_WRITE.hposp0 = playerEnt.playerEntity._eyeCoords.x 
            + HPOSP_MIN + playerEnt.playerEntity._objectAnchorPoint.x;
        camera.cameraEntity.renderer(&camera.cameraEntity);
        // map_relativeMove(dir);
        // ADD_ASSIGN_VEC2(QuickAndDirtyCamera, dir)

        set_player_vert_position(cur_player,
                playerEnt.playerEntity._eyeCoords.y + V_MIN - 
                    playerEnt.playerEntity._objectAnchorPoint.y ,true);
    
        
        //process graphics
        // if (check_if_any_collision(cur_player)) {
        //     GTIA_WRITE.hitclr = 1;
        //     set_player_horiz_position(cur_player,cur_horiz_position,true);
        //     set_player_vert_position(cur_player,cur_vert_position,true);
        // } else {
        //     set_player_horiz_position(cur_player,playerEnt.playerEntity.eyeCoords.x,true);
        //     set_player_vert_position(cur_player,playerEnt.playerEntity.eyeCoords.y,true);
        // }
        
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


void InitializeStaticEntities(){
    test_player1();

    
    playerConstructor();
    
    //debug manual assign for now
    playerEnt.playerEntity._worldCoords.x = SCR_RES_X/2;
    playerEnt.playerEntity._worldCoords.y = SCR_RES_Y/2;
    
    cameraConstructor(&playerEnt.playerEntity);

    dumbItem_constructor(&dumbItem);
}

//stub for now, this will be designed later
//should produce a final gamestate...
//the idea is to have an array of frametask ptrs to run in order
void ProcessFrameTasks(){
    playerEnt.playerEntity.frameTask(&(playerEnt.playerEntity));

    
    //debug_autoMove(&(playerEnt.playerEntity.transform));


    camera.cameraEntity.frameTask(&(camera.cameraEntity));

    dumbItem.frameTask(&dumbItem);
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
