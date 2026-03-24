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
    playerEnt.playerEntity._worldCoords.x = 624;
    playerEnt.playerEntity._worldCoords.y = 560;
    
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

  switch((&(GTIA_READ.p0pf))[TEMP_player_anticIndex]){
    case 0:
        break;
    case 8: //only the trigger color is activated
        orb_singleton.entity.OnCollision(NULL, NULL);
        break;
    default:
        player_OnCollide(&playerEnt.playerEntity, NULL);
        break;
  }

  GTIA_WRITE.hitclr = 1; 
        
};