#include "player.h"

Vector2 worldCoordPlayerView;
uint8_t TEMP_player_anticIndex;

bool vertMovePlayer = true;


//initializer list to allow compile time assign/construct
PlayerEntity playerEnt = {
  {player_FrameTask, playerRenderer, player_OnCollide, (void *)&playerEnt, &dumbItem, //entity
    
    {{0,0}, {0,0}, {0,0},{4,4}}}, //entity.transform


  {0,0}, 1, {NULL}, {0,0} // player specific vars
    

  
  };

//per frame behavior
STATUS player_FrameTask(Entity* thisEntity) {
  //pseudo

  // read input and set velocity
  // TODO: check for collisions before input?

  return playerInputProcess();
}

// remember, the renderer happens as the second batch, after all game logic is
// calced
// prepare the graphics driver
STATUS playerRenderer(Entity* thisEntity) {

  thisEntity->_eyeCoords = convertToEyeCoords(thisEntity->_worldCoords);
  //incomplete

  //generally dont need a bounds check, player is always in frame
  (&(GTIA_WRITE.hposp0))[TEMP_player_anticIndex] = playerEnt.playerEntity._eyeCoords.x 
            + HPOSP_MIN + playerEnt.playerEntity._objectAnchorPoint.x;
  
  // sadly, this is a necessary evil
  // (it takes up ~20-40% of frame cycles)
  pmgSilo_setY(playerEnt.playerSilo, thisEntity->_eyeCoords.y);

  return UNDEFINED;
}


//made global/static due to frequency of use
unsigned char joystickState = 0; 
unsigned char lastFrameState;

//Assumes the proper driver is loaded!!
STATUS playerInputProcess(){

  playerEnt.player_LastPos = playerEnt.playerEntity._worldCoords;
  lastFrameState = joystickState;
  //read the joystick data
  joystickState = joy_read(JOY_1);

  // left/right movement
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

  // up/down movement
  switch(JOY_UPDOWN(joystickState)){
    case JOY_UP_MASK: 
      playerEnt.playerVelocity.y = -playerEnt.playerSpeed;
      vertMovePlayer = true;
      break;
    case JOY_DOWN_MASK: //down
      playerEnt.playerVelocity.y = playerEnt.playerSpeed;
      vertMovePlayer = true;
      break;
    default:  //nothing or null cancelled
      playerEnt.playerVelocity.y = 0;
      break;
  }

  //makes this lock out when held
  if(JOY_FIRE(joystickState) && !JOY_FIRE(lastFrameState)){
    //will currently break if player has no child
    // TODO: make this drop the childEntity! (which should be an item)
    playerEnt.playerEntity.childEntity->frameTask(playerEnt.playerEntity.childEntity);
  }


  ADD_ASSIGN_VEC2(playerEnt.playerEntity._worldCoords, playerEnt.playerVelocity)
  
  return PASS;
}

// Presently only sets worldcoord back when called
// May need more grandular checks later...
// TODO: FIX THESE STUPID COLLISIONS!!!!!
void player_OnCollide(Entity* thisEntity, Entity* otherEntity){

  if (otherEntity == NULL) {
    // assume that playfield was hit
    playerEnt.playerEntity._worldCoords = playerEnt.player_LastPos;
    // playerEnt.playerVelocity.x = playerEnt.playerSpeed;
    playerEnt.playerEntity._worldCoords.x -= playerEnt.playerVelocity.x;
    playerEnt.playerEntity._worldCoords.y -= playerEnt.playerVelocity.y;
  } else if (otherEntity == &(dragonSingleton.myEntity)) {
    
    // prevent collisions through a chomping dragon
    // TODO: simplify this if possible
    if (dragonSingleton.state == D_STATE_CHOMP) {
      playerEnt.playerEntity._worldCoords = playerEnt.player_LastPos;
      // playerEnt.playerVelocity.x = playerEnt.playerSpeed;
      playerEnt.playerEntity._worldCoords.x -= playerEnt.playerVelocity.x;
      playerEnt.playerEntity._worldCoords.y -= playerEnt.playerVelocity.y;
    }
  }
  // TODO: check if its an item
 
  return;
}

// bad :D
void player_horiz_collisions() {
  playerEnt.playerEntity._worldCoords = playerEnt.player_LastPos;
  
}

//init the player specific vars
STATUS playerConstructor(){
  uint8_t pmg_index;

  //call the "base" constructor

  // set PMG index and validate it
  pmg_index = pmg_addPlayerSprite(&playerSprite);


  if(pmg_index < 4){
    playerEnt.playerSilo = activePMGInstance->playerGFX + pmg_index;
    TEMP_player_anticIndex = pmg_index;
  }

  //printf("Address: %d\n", %d)
  return PASS;
}

// sprite :D
uint8_t playerSpriteBitmap[] ={
  0b00111100,
  0b00111100,
  0b00111100,
  0b00111100,
};

Sprite playerSprite = {sizeof(playerSpriteBitmap),GTIA_COLOR_YELLOW,playerSpriteBitmap};



