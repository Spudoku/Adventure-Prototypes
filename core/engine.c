#include "engine.h"

void engine_Boot(){
    joy_install(&atrstd_joy);  

    //init graphics
    gfx_Init();
    engine_InitSingletons();     
}


void engine_InitSingletons(){
 
    playerConstructor();
    dragon_Init(&debug_dragonSingleton);  //TEMP
    dragon_TrackEntity(&debug_dragonSingleton, &playerEnt.playerEntity);

    
    //debug manual assign for now
    playerEnt.playerEntity._worldCoords.x = SCR_RES_X/2;
    playerEnt.playerEntity._worldCoords.y = SCR_RES_Y/2;
    
    cameraConstructor(&playerEnt.playerEntity);


}

//stub for now, this will be designed later
//should produce a final gamestate...
//the idea is to have an array of frametask ptrs to run in order
void engine_StateUpdate(){

    sound_StateUpdate();

    playerEnt.playerEntity.frameTask(&(playerEnt.playerEntity));

    debug_dragonSingleton.myEntity.frameTask(&(debug_dragonSingleton.myEntity));
    
    camera.cameraEntity.frameTask(&(camera.cameraEntity));
}

void engine_Render(){
    camera.cameraEntity.renderer(&camera.cameraEntity); //camera goes first

    playerEnt.playerEntity.renderer(&(playerEnt.playerEntity));
    debug_dragonSingleton.myEntity.renderer(&(debug_dragonSingleton.myEntity));
};

//stub to demonstrate where events would be dispatched during each tick
void engine_EventDispatcher(){

  //Temporary bruteforce collision behavior pending true dispatching
  if((&(GTIA_READ.p0pl))[TEMP_dragon_anticIndex]){
      debug_dragonSingleton.moveDelayCounter += 100;
  }

  if((&(GTIA_READ.p0pf))[TEMP_player_anticIndex]){
      player_OnCollide(&playerEnt.playerEntity, NULL);
  }
  GTIA_WRITE.hitclr = 1; 
        
};