#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "player.h"
#include "util_input.h"
#include <joystick.h>
#include "items.h"

<<<<<<< HEAD
Vector2 worldCoordPlayerView;

//initializer list to allow compile time assign/construct
PlayerEntity playerEnt = {
  {0,0}, 1, // player specific vars
    
    {player_FrameTask, playerRenderer, (void *)&playerEnt, &nullItem, //entity
      
      {{0,0}, {0,0}, {1,6},{6,6}}} //entity.transform

  
  };
Sprite playerSprites[PLAYER_SPRITE_COUNT] = {
  {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111100,
        0b00101000, //
        0b00111100, //
        0b00111100,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
    },
};
=======
Vector2 worldCoordPlayerView;

//initializer list to allow compile time assign/construct
PlayerEntity playerEnt = {
  {0,0}, 1, // player specific vars
    
    {player_FrameTask, playerRenderer, (void *)&playerEnt, &nullItem, //entity
      
      {{0,0}, {0,0}, {1,6},{6,6}}} //entity.transform

  
  };
>>>>>>> 40d6363eb74a5ac503cff75762d8fa049e0e1db1

//per frame behavior
STATUS player_FrameTask(Entity* thisEntity) {
STATUS player_FrameTask(Entity* thisEntity) {
  //pseudo

  // read input and set velocity
  return playerInputProcess();
}

//remember, the renderer happens as the second batch, after all game logic is
//calced
//prepare the graphics driver
STATUS playerRenderer(Entity* thisEntity) {

  thisEntity->_eyeCoords = convertToEyeCoords(thisEntity->_worldCoords);
  //incomplete


  thisEntity->_eyeCoords = convertToEyeCoords(thisEntity->_worldCoords);
  //incomplete

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

  //TODO: make a bitflag of the states and use a switch statement
<<<<<<< HEAD
  // 
=======

  //TODO: make a bitflag of the states and use a switch statement
  //this is if statements to allow this fuckery (cant do it in a switch?)
>>>>>>> 40d6363eb74a5ac503cff75762d8fa049e0e1db1
  if(JOY_RIGHT(joystickState)){
    playerEnt.playerVelocity.x = playerEnt.playerSpeed;
    playerEnt.playerEntity._worldCoords.x += playerEnt.playerVelocity.x;
  } else if (JOY_LEFT(joystickState)){
<<<<<<< HEAD
    playerEnt.playerVelocity.x = -playerEnt.playerSpeed;
    // this looks redundant but apparently susbee said that subtracting unsigned ints is weird,
    // so don't touch this for now
    playerEnt.playerEntity._worldCoords.x += playerEnt.playerVelocity.x;
=======
    playerEnt.playerVelocity.x = playerEnt.playerSpeed;
    playerEnt.playerEntity._worldCoords.x -= playerEnt.playerVelocity.x;
    playerEnt.playerEntity._worldCoords.x -= playerEnt.playerVelocity.x;
>>>>>>> 40d6363eb74a5ac503cff75762d8fa049e0e1db1
  } else {
    playerEnt.playerVelocity.x = 0;
  }

  // vertical movement
  if(JOY_UP(joystickState)){
    playerEnt.playerVelocity.y = -playerEnt.playerSpeed;
    playerEnt.playerEntity.eyeCoords.y += playerEnt.playerVelocity.y;
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

  //TODO: interrupt the task or make a "lateupdate" to wait for gamestate
  // to process
  //TODO: clamping
  
  playerEnt.playerEntity._worldCoords.y += playerEnt.playerVelocity.y;
  
  //calc


  
  
<<<<<<< HEAD
=======
  playerEnt.playerEntity._worldCoords.y += playerEnt.playerVelocity.y;
  
  //calc

>>>>>>> 40d6363eb74a5ac503cff75762d8fa049e0e1db1
  return PASS;
}

//init the player specific vars
STATUS playerConstructor(){
<<<<<<< HEAD
  // playerEnt.playerSpeed = 1;
  playerEnt.moveFrameDelay = 0;
  // playerEnt.playerVelocity.x = 0;
  // playerEnt.playerVelocity.y = 0;

  //call the "base" constructor

  

  //entityConstructor((Entity*)&playerEnt.playerEntity, player_FrameTask, playerRenderer);

  // entityConstructor(&(playerEnt.playerEntity), playerRoutine, playerRenderer);
  // assign to the player entity it's dummy obj item
=======
  // playerEnt.playerSpeed = 1;
  // playerEnt.playerVelocity.x = 0;
  // playerEnt.playerVelocity.y = 0;

  //call the "base" constructor

  

  //entityConstructor((Entity*)&playerEnt.playerEntity, player_FrameTask, playerRenderer);
  //assign to the player entity it's dummy obj item
>>>>>>> 40d6363eb74a5ac503cff75762d8fa049e0e1db1

  // playerEnt.playerEntity.childEntity = &nullItem;
  // //in the future, the constructor will be not ran right here, probably during
  // //boot sequence
  // nullItem_constructor(&nullItem);  
  // playerEnt.playerEntity.childEntity = &nullItem;
  // //in the future, the constructor will be not ran right here, probably during
  // //boot sequence
  // nullItem_constructor(&nullItem);  

  return PASS;
}

void TrackMoveDelayFramesPlayer() {
  playerEnt.moveDelayCounter++;
  if (playerEnt.moveDelayCounter > playerEnt.moveFrameDelay) {
    playerEnt.moveDelayCounter = 0;
  }
}