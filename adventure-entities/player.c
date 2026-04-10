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
  // TODO: check for collisions

  

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

  playerEnt.player_LastPos = playerEnt.playerEntity._worldCoords;
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
    // TODO: make this drop the childEntity!
    playerEnt.playerEntity.childEntity->frameTask(playerEnt.playerEntity.childEntity);
  }


  ADD_ASSIGN_VEC2(playerEnt.playerEntity._worldCoords, playerEnt.playerVelocity)
  
  return PASS;
}

//Presently only sets worldcoord back when called
//May need more grandular checks later...
// TODO: check if otherEntity is dragon and if so, check its state
// if its chomping, do 
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
 
  return;
}

void player_horiz_collisions() {
  playerEnt.playerEntity._worldCoords = playerEnt.player_LastPos;
  
}

//init the player specific vars
STATUS playerConstructor(){
  uint8_t pmg_index;

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
  0b00111100,
  0b00111100,
  0b00111100,
  0b00111100,
};

Sprite playerSprite = {sizeof(playerSpriteBitmap),GTIA_COLOR_YELLOW,playerSpriteBitmap};



