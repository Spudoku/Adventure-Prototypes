#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "player.h"
#include "util_input.h"
#include <joystick.h>
#include "items.h"


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


  lastFrameState = joystickState;
  //read the joystick data
  joystickState = joy_read(JOY_1);

  // set the intended velocity
  // switch(joystickState){
  //   case (JOY_UP(joystickState))



  //   default:
  //     playerVelocity.x = 0;
  //     playerVelocity.y = 0;
  //     break;
  // }
  
  //todo: speed may need to be normalized for diagonal
  //this is if statements to allow this fuckery (cant do it in a switch?)
  if(JOY_RIGHT(joystickState)){
    player.playerVelocity.x = player.playerSpeed;
  } else {
    player.playerVelocity.x = -player.playerSpeed * JOY_LEFT(joystickState);
  }

  if(JOY_UP(joystickState)){
    player.playerVelocity.y = player.playerSpeed;
  } else {
    player.playerVelocity.y = -player.playerSpeed * JOY_DOWN(joystickState);
  }


  //makes this lock out when held
  if(JOY_FIRE(joystickState) && !JOY_FIRE(lastFrameState)){
    //will currently break if player has no child
    player.playerEntity.childEntity->frameTask(player.playerEntity.childEntity);
  }

  return PASS;
}

//init the player specific vars
STATUS playerConstructor(){
  player.playerSpeed = 1;
  player.playerVelocity.x = 0;
  player.playerVelocity.y = 0;

  //call the "base" constructor
  entityConstructor(&(player.playerEntity), playerRoutine, playerRenderer);
  //assign to the player entity it's dummy obj item

  player.playerEntity.childEntity = &nullItem;
  //in the future, the constructor will be not ran right here, probably during
  //boot sequence
  nullItem_constructor(&nullItem);  

  return PASS;
}