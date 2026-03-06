
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









// #define CHARSET_ADDR 0x3C
// according to Ed, 0x3C00 is chosen since its 4 pages from the screen memory buffer he allocated (0x4000). S
// According to Mapping the Atari, cannot set 756 to any odd number or else we will have screen garbage.
// additionally, need to start on a page boundary (any value in the form $XX00), such as $C000
// DL_CHR40x8x1


// function declarations
void fix_displayList();
void init_charset();
void edit_colors();
void setScreenMemOffset(int x, int y);
void relativeMoveScrMem(int dx, int dy);


void joystick_test();

void test_player1();


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
int i = 0;
// PlayerEntity playerEnt = {{0,0}, 0, 
//     {player_FrameTask, playerRenderer, (void *)NULL, (Entity *)NULL, {{0,0}, {0,0}, {0,0},{0,0}}, {0,0}}};

extern PlayerEntity playerEnt;

unsigned int cur_player = 0; 

int goForward = 1;
int goDown = 1;
int fineScroll_y = 16; //test only
int fineScroll_x = 16; //test only
int roughScroll_x;

Vector2 QuickAndDirtyCamera = {0,0};
Vector2 dir = {1, 1};
Vector2 revDir = {0, -1};

//char theFiller[17000];    //test for worst case map fit

int main() {
 // variable declarations
    unsigned int cur_horiz_position;
    unsigned int cur_vert_position;


    InitializeJoystick();
    initializeEngine();


    set_player_horiz_position(0,SCREEN_HORIZ_CENTER,true);
    set_player_vert_position(0,SCREEN_VERT_CENTER,true);


    while (true) {

        
        //process gamestate
        cur_horiz_position = playerEnt.playerEntity.eyeCoords.x;
        cur_vert_position = playerEnt.playerEntity.eyeCoords.y;
        
        
        
        processFrameTasks();
        waitvsync();
        GTIA_WRITE.hposp0 = playerEnt.playerEntity._eyeCoords.x + 48;
        camera.cameraEntity.renderer(&camera.cameraEntity);
        // map_relativeMove(dir);
        // ADD_ASSIGN_VEC2(QuickAndDirtyCamera, dir)

        // //quick and dirty debug test
        // //if depth height is lessEq than 96 (test val)) add


        // //if in range, dont flip
        // dir.y = ((QuickAndDirtyCamera.y > 96) 
        //     || (QuickAndDirtyCamera.y <= 0)) ? -dir.y : dir.y;


        // dir.x = ((QuickAndDirtyCamera.x > 160) 
        //     || (QuickAndDirtyCamera.x <= 0)) ? -dir.x : dir.x;

   
        



        //set_player_horiz_position(cur_player,playerEnt.playerEntity._eyeCoords.x,true);
        set_player_vert_position(cur_player,playerEnt.playerEntity._eyeCoords.y,true);
    
        
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


    
    playerConstructor();
    cameraConstructor(&playerEnt.playerEntity);

    //debug manual assign for now
    playerEnt.playerEntity._worldCoords = camera.centerPoint;

    // //temp init assign
    playerEnt.playerEntity.eyeCoords.x = SCREEN_HORIZ_CENTER;
    playerEnt.playerEntity.eyeCoords.y = SCREEN_VERT_CENTER;
}

//stub for now, this will be designed later
//should produce a final gamestate...
//the idea is to have an array of frametask ptrs to run in order
void processFrameTasks(){
    playerEnt.playerEntity.frameTask(&(playerEnt.playerEntity));
    camera.cameraEntity.frameTask(&(camera.cameraEntity));
}

// writes crucial bytes to the display list
void fix_displayList() {
    //Vector2 zero = {0, 0};

    


    //this means that antic technically loads offscreen garbage data, but this
    //is how a true 0,0 pixel coord is achieved
    QuickAndDirtyCamera.x = 0;
    QuickAndDirtyCamera.y = 0;
    // setScreenMemOffset(-2,0);
    // //relativeMoveScrMem(0,0);


    // *(unsigned int *)&DisplayList[sizeof(DisplayList) - 1] = (unsigned int)DisplayList;
    map_absoluteMove(QuickAndDirtyCamera);
    InitDisplayList();

    //hscroll does mean that we cant get to the furthest left part of the screen
    // ANTIC.hscrol = 0; 
    // ANTIC.vscrol = 0;


    //OS.sdlst = &DisplayList;




    return;
    // location of screen memory
    // unsigned int scr_addr = (unsigned int)ScreenMemory; // address of screen memory array
    // unsigned int dl_addr = (unsigned int)DisplayList; // address of display list

    // // inject screenmemory address into lms instruction
    // DisplayList[4] = scr_addr & 0xFF;
    // DisplayList[5] = (scr_addr >> 8) & 0xFF;

    // // inject dlistAddr into the indices 28 and 29
    // DisplayList[sizeof(DisplayList) - 2] = dl_addr & 0xFF;   // this injects the 8 most significant bits
    // DisplayList[sizeof(DisplayList) - 1] = (dl_addr >> 8) & 0xFF; // this injects the 8 least signifcant bits
    //POKEW(DISPLAY_LIST,dl_addr);


}

//offset from top left (0,0)
void setScreenMemOffset(int x, int y){
    //unsigned int j = y;
    unsigned int i;

    
    //go to the location, scan down in inc of three

    //start at the first graphics instr
    //the comparison is to compare memory address
    roughScroll_x = x;

    //TODO: transition this later to iterate the ptr directly
    for(i = 3; DisplayList[i] != DL_JVB && i < sizeof(DisplayList); i+= 3){
        //work with a window of 3 bytes, convert i+1 to an int ptr
        //foregoing the syntatic sugar here


        
        *(unsigned int *)(DisplayList + (i+1)) = (unsigned int)(gameMap[y]) + x;
        y += 1;
    } 
}

//this may be slower than just setting!
void relativeMoveScrMem(int dx, int dy){
    int i = 0;
    switch(dy) {
        case -1:
            dy = -40;
        case 0:
            break;
        case 1:
            dy = 40;
            break;
        default:
            dy *= 40;
            break;
    }

    for(i = 3; DisplayList[i] != DL_JVB && i < sizeof(DisplayList); i+= 3){
        //work with a window of 3 bytes, convert i+1 to an int ptr
        //foregoing the syntatic sugar here


        
        *(unsigned int *)(DisplayList + (i+1)) += dy + dx;
        //y += ;
    } 
}

// initializes the character set
void init_charset() {
    IntToTwoChar charsetAddress;
    charsetAddress.integer = (unsigned int)charset; //get highbyte from addr

    OS.chbas = charsetAddress.bytes[1]; //set high byte to CHBASE register
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
