#include "util.h"
#include "util_structs.h"
// #include "entity.h"
#include "player.h"
#include "util_input.h"
#include <joystick.h>
#include "items.h"

#include <stdio.h>

Vector2 worldCoordPlayerView;
uint8_t TEMP_player_anticIndex;

//initializer list to allow compile time assign/construct
PlayerEntity playerEnt = {
  {player_FrameTask, playerRenderer, player_OnCollide, (void *)&playerEnt, &dumbItem, //entity
    
    {{0,0}, {0,0}, {0,0},{4,4}}}, //entity.transform


  {0,0}, 1, {NULL} // player specific vars
    

  
  };

//per frame behavior
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

  //generally dont need a bounds check, player is always in frame

  (&(GTIA_WRITE.hposp0))[TEMP_player_anticIndex] = playerEnt.playerEntity._eyeCoords.x 
            + HPOSP_MIN + playerEnt.playerEntity._objectAnchorPoint.x;
  
  //printf("%d\n",(&(GTIA_WRITE.hposp0))[TEMP_player_anticIndex] );
  pmgSilo_setY(playerEnt.playerSilo, thisEntity->_eyeCoords.y);


  return UNDEFINED;
}
//made global/static due to frequency of use
unsigned char joystickState = 0; 
unsigned char lastFrameState;

//Assumes the proper driver is loaded!!
STATUS playerInputProcess(){


  lastFrameState = joystickState;
  //read the joystick data
  joystickState = joy_read(JOY_1);


  switch(JOY_LEFTRIGHT(joystickState)){
    case JOY_LEFT_MASK:
      playerEnt.playerVelocity.x = -playerEnt.playerSpeed;
      break;
    case JOY_RIGHT_MASK:
      playerEnt.playerVelocity.x = playerEnt.playerSpeed;
      break;
    default:
      playerEnt.playerVelocity.x = 0;
      break;
  }

  switch(JOY_UPDOWN(joystickState)){
    case JOY_UP_MASK: 
      playerEnt.playerVelocity.y = -playerEnt.playerSpeed;
      break;
    case JOY_DOWN_MASK: //down
      playerEnt.playerVelocity.y = playerEnt.playerSpeed;
      break;
    default:  //nothing or null cancelled
      playerEnt.playerVelocity.y = 0;
      break;
  }

  //makes this lock out when held
  if(JOY_FIRE(joystickState) && !JOY_FIRE(lastFrameState)){
    //will currently break if player has no child
    playerEnt.playerEntity.childEntity->frameTask(playerEnt.playerEntity.childEntity);
  }


  ADD_ASSIGN_VEC2(playerEnt.playerEntity._worldCoords, playerEnt.playerVelocity)
  


  return PASS;
}

void player_OnCollide(Entity* thisEntity, Entity* otherEntity){
  //currently, do nothing

  return;
}

//init the player specific vars
STATUS playerConstructor(){
  uint8_t pmg_index;
  // playerEnt.playerSpeed = 1;
  // playerEnt.playerVelocity.x = 0;
  // playerEnt.playerVelocity.y = 0;

  //call the "base" constructor

  

  //entityConstructor((Entity*)&playerEnt.playerEntity, player_FrameTask, playerRenderer);
  //assign to the player entity it's dummy obj item

  // playerEnt.playerEntity.childEntity = &nullItem;
  // //in the future, the constructor will be not ran right here, probably during
  // //boot sequence
  // nullItem_constructor(&nullItem);  



  pmg_index = pmg_addPlayerSprite(&playerSprite);

  if(pmg_index < 4){
    //schedulerData.antic_P2PCollisionLookupTable[pmg_index] = (Entity *)&playerEnt;
    //schedulerData.antic_P2PCollisionLookupMask |= (1 << pmg_index);
    playerEnt.playerSilo = activePMGInstance->playerGFX + pmg_index;
    TEMP_player_anticIndex = pmg_index;
  }
  printf("Player antic index: %d\n", TEMP_player_anticIndex);

  //printf("Address: %d\n", %d)
  return PASS;
}
uint8_t playerSpriteBitmap[] ={
  0b11110000,
  0b11110000,
  0b11110000,
  0b11110000,
};

Sprite playerSprite = {sizeof(playerSpriteBitmap),GTIA_COLOR_YELLOW,playerSpriteBitmap};



