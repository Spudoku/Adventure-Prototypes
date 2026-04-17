#include "engine.h"

unsigned char p1_collisions;
void engine_Boot(){
    // install drivers
    joy_install(&atrstd_joy);  

    //init graphics
    gfx_Init();
    engine_InitSingletons();     
}


void engine_InitSingletons(){
    
    playerConstructor();
    dragon_Init(&dragonSingleton);  //TEMP
    dragon_TrackEntity(&dragonSingleton, &playerEnt.playerEntity);



    
    //debug manual assign for now
    playerEnt.playerEntity._worldCoords.x = 624;
    playerEnt.playerEntity._worldCoords.y = 560;

    chaliceEnt.chaliceEntity._worldCoords.x = 600;
    chaliceEnt.chaliceEntity._worldCoords.y = 560;


    
    cameraConstructor(&playerEnt.playerEntity);

    chalice_constructor();


}

//should produce a final gamestate...
//the idea is to have an array of frametask ptrs to run in order
void engine_StateUpdate(){

    // this is generally just decrementing stuff
    sound_StateUpdate();

    playerEnt.playerEntity.frameTask(&(playerEnt.playerEntity));


    chaliceEnt.chaliceEntity.frameTask(&chaliceEnt.chaliceEntity);
    
    camera.cameraEntity.frameTask(&(camera.cameraEntity));
}

void engine_Render(){
    camera.cameraEntity.renderer(&camera.cameraEntity); //camera goes first

    playerEnt.playerEntity.renderer(&(playerEnt.playerEntity));
    dragonSingleton.myEntity.renderer(&(dragonSingleton.myEntity));

    chaliceEnt.chaliceEntity.renderer(&(chaliceEnt.chaliceEntity));
};

//stub to demonstrate where events would be dispatched during each tick
void engine_EventDispatcher(){



    // if dragon collides with player...
    p1_collisions = player_to_player_collisions(TEMP_dragon_anticIndex);
    
    // if dragon collides with player avatar...
    if (collision_with_index(p1_collisions,TEMP_player_anticIndex)) {
        // sound_generic_buzz();
        dragon_OnCollision(&(dragonSingleton.myEntity), &playerEnt.playerEntity);
        player_OnCollide(&playerEnt.playerEntity, &(dragonSingleton.myEntity));
    }



    // player character collisions
    // currently this checks if any playfield is collided with
    if (player_to_playfield_collisions(TEMP_player_anticIndex)) {
        player_OnCollide(&playerEnt.playerEntity, NULL);
    }

    GTIA_WRITE.hitclr = 1; 
        
};


// play the sad sound when you get got
// for now, just reset the game
void end_game() {

    trigger_warm_reset();
}

void trigger_warm_reset(void) {
    // 1. Set the Warmstart flag (WARMST) at 0x0008 to non-zero
    *(unsigned char*)0x0008 = 0x01;

    // 2. Jump to the OS reset vector. 
    // On the Atari, the reset vector is at 0xE474.
    // In cc65, we can use an assembly wrapper or a function pointer.
    ((void (*)(void))0xE474)();
}