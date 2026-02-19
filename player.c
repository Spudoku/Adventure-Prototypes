#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "player.h"
#include "util_input.h"
#include <joystick.h>
#include "items.h"

extern PlayerEntity playerEnt;

//per frame behavior
STATUS playerRoutine(Entity* thisEntity) {
  //pseudo

  // read input and set velocity
  return playerInputProcess();
}

//remember, the renderer happens as the second batch, after all game logic is
//calced
//prepare the graphics driver
STATUS playerRenderer(Entity* thisEntity) {
  return UNDEFINED;
}

//Assumes the proper driver is loaded!!
STATUS playerInputProcess(){
  static unsigned char joystickState = 0; //useful for how often it will be
  static unsigned char lastFrameState;

  TrackMoveDelayFramesPlayer();
  if (playerEnt.moveDelayCounter < playerEnt.moveFrameDelay) {
    return PASS;
  }

  lastFrameState = joystickState;
  //read the joystick data
  joystickState = joy_read(JOY_1);
  
  //todo: speed may need to be normalized for diagonal
  // 
  if(JOY_RIGHT(joystickState)){
    playerEnt.playerVelocity.x = playerEnt.playerSpeed;
    playerEnt.playerEntity.eyeCoords.x += playerEnt.playerVelocity.x;
  } else if (JOY_LEFT(joystickState)){
    playerEnt.playerVelocity.x = playerEnt.playerSpeed;
    // this looks redundant but apparently susbee said that subtracting unsigned ints is weird,
    // so don't touch this for now
    playerEnt.playerEntity.eyeCoords.x -= playerEnt.playerVelocity.x;
  } else {
    playerEnt.playerVelocity.x = 0;
  }

  // vertical movement
  if(JOY_UP(joystickState)){
    playerEnt.playerVelocity.y = playerEnt.playerSpeed;
    playerEnt.playerEntity.eyeCoords.y -= playerEnt.playerVelocity.y;
  } else if (JOY_DOWN(joystickState)) {
    playerEnt.playerVelocity.y = playerEnt.playerSpeed;
    playerEnt.playerEntity.eyeCoords.y += playerEnt.playerVelocity.y;
  }else {
    // playerEnt.playerVelocity.y = playerEnt.playerSpeed * JOY_DOWN(joystickState);
    playerEnt.playerVelocity.y = 0;
  }


  //makes this lock out when held
  if(JOY_FIRE(joystickState) && !JOY_FIRE(lastFrameState)){
    //will currently break if player has no child
    playerEnt.playerEntity.childEntity->frameTask(playerEnt.playerEntity.childEntity);
  }


  
  
  return PASS;
}

//init the player specific vars
STATUS playerConstructor(){
  playerEnt.playerSpeed = 1;
  playerEnt.moveFrameDelay = 0;
  playerEnt.playerVelocity.x = 0;
  playerEnt.playerVelocity.y = 0;

  //call the "base" constructor

  // entityConstructor(&(playerEnt.playerEntity), playerRoutine, playerRenderer);
  // assign to the player entity it's dummy obj item

  playerEnt.playerEntity.childEntity = &nullItem;
  //in the future, the constructor will be not ran right here, probably during
  //boot sequence
  nullItem_constructor(&nullItem);  

  return PASS;
}

void TrackMoveDelayFramesPlayer() {
  playerEnt.moveDelayCounter++;
  if (playerEnt.moveDelayCounter > playerEnt.moveFrameDelay) {
    playerEnt.moveDelayCounter = 0;
  }
}