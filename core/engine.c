#include "engine.h"

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

    sound_StateUpdate();

    playerEnt.playerEntity.frameTask(&(playerEnt.playerEntity));

    dragonSingleton.myEntity.frameTask(&(dragonSingleton.myEntity));

    // chaliceItem.frameTask(&chaliceItem);
    
    camera.cameraEntity.frameTask(&(camera.cameraEntity));
}

void engine_Render(){
    camera.cameraEntity.renderer(&camera.cameraEntity); //camera goes first

    playerEnt.playerEntity.renderer(&(playerEnt.playerEntity));
    dragonSingleton.myEntity.renderer(&(dragonSingleton.myEntity));

    // chaliceItem.renderer(&chaliceItem);
    chaliceEnt.chaliceEntity.renderer(&(chaliceEnt.chaliceEntity));
};

//stub to demonstrate where events would be dispatched during each tick
void engine_EventDispatcher(){

  //Temporary bruteforce collision behavior pending true dispatching
  if((&(GTIA_READ.p0pl))[TEMP_dragon_anticIndex]){
      
      dragon_OnCollision(&(dragonSingleton.myEntity), &playerEnt.playerEntity);
        player_OnCollide(&playerEnt.playerEntity, &(dragonSingleton.myEntity));
  }

  // player collisions
  switch((&(GTIA_READ.p0pf))[TEMP_player_anticIndex]){
    case 0:
        break;
    case 8: //only the trigger color is activated
        orb_singleton.entity.OnCollision(NULL, NULL);
        orb_sound();
        break;
    default:
        player_OnCollide(&playerEnt.playerEntity, NULL);
        break;
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