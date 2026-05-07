#include "player.h"
#pragma optimize(on)
#pragma static-locals(on)

Vector2 worldCoordPlayerView;
uint8_t TEMP_player_anticIndex;

bool vertMovePlayer = true;
bool movePlayer = true;
bool updateSafePlace = true;



//initializer list to allow compile time assign/construct
PlayerEntity playerEnt = {
  {player_FrameTask, playerRenderer, player_OnCollide, (void *)&playerEnt, NULL, //entity
    
    {{0,0}, {0,0}, {0,0},{4,4}}}, //entity.transform


  {0,0}, 1, {NULL}, {0,0},{0,0}, // player specific vars
    

  4,            // size variable
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
  
  

  pmgSilo_setY_player(playerEnt.playerSilo, thisEntity->_eyeCoords.y, false);
  return UNDEFINED;
}


//made global/static due to frequency of use
unsigned char joystickState = 0; 
unsigned char lastFrameState;

//Assumes the proper driver is loaded!!
STATUS playerInputProcess(){
  unsigned char destination;
  int16_t tempX;
  int16_t tempY;

  if (!movePlayer) {
    movePlayer = true;
    return PASS;
  }
  movePlayer = true;
  
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
    // playerEnt.playerEntity.childEntity->frameTask(playerEnt.playerEntity.childEntity);
    player_drop_item(playerEnt.playerEntity.childEntity);
    debug_action();
  }

  tempX = playerEnt.playerEntity._worldCoords.x;
  tempY = playerEnt.playerEntity._worldCoords.y;

  // TODO: optimize this!
  // handling horizontal movement/collisions
  // Vec2_add_x(&(playerEnt.playerEntity._worldCoords), playerEnt.playerVelocity.x);
  if (playerEnt.playerVelocity.x > 0 || playerEnt.playerVelocity.x < 0) {
    tempX += playerEnt.playerVelocity.x;

  
    destination = getTileAt(tempX,tempY,playerEnt.size);
    if (destination) {

      // playerEnt.playerEntity._worldCoords = playerEnt.player_LastPos;
      tempX = playerEnt.playerEntity._worldCoords.x;
    } 
    else {
      
    }
  }
  

  if (playerEnt.playerVelocity.y > 0 || playerEnt.playerVelocity.y < 0) {
  // handling vertical movement/collisions
    tempY += playerEnt.playerVelocity.y;

    
    destination = getTileAt(tempX, tempY ,playerEnt.size);
    // destination2 = getTileAt(maxCoords);
    // if any non-blank tile is hit...
    if (destination) {
      // updateSafePlace = false;
      // playerEnt.playerEntity._worldCoords = playerEnt.player_LastPos;
      tempY = playerEnt.playerEntity._worldCoords.y;
    } 
    else {
      
    }
  }

  
  playerEnt.playerEntity._worldCoords.x = tempX;
  playerEnt.playerEntity._worldCoords.y = tempY;
  playerEnt.player_LastPos.y = tempY;
  playerEnt.player_LastPos.x = tempX;
  
  if (playerEnt.playerEntity.childEntity != NULL) {
    // TODO: move item
    playerEnt.playerEntity.childEntity->_worldCoords.x = tempX + playerEnt.item_offset.x;
    playerEnt.playerEntity.childEntity->_worldCoords.y = tempY + playerEnt.item_offset.y;
  }
  return PASS;
}

// Presently only sets worldcoord back when called
// May need more grandular checks later...
// currently, this is completely redundant
void player_OnCollide(Entity* thisEntity, Entity* otherEntity){

  // if (otherEntity == NULL) {
  //   // // assume that playfield was hit
  //   // playerEnt.playerEntity._worldCoords = playerEnt.player_LastPos;
  //   // // playerEnt.playerVelocity.x = playerEnt.playerSpeed;
  //   // playerEnt.playerEntity._worldCoords.x -= playerEnt.playerVelocity.x;
  //   // playerEnt.playerEntity._worldCoords.y -= playerEnt.playerVelocity.y;
  //   // // printf("players world coords and last safe place\n");
  //   // // PRINT_VEC2(playerEnt.playerEntity._worldCoords);
  //   // // PRINT_VEC2(playerEnt.player_LastPos);
  //   // updateSafePlace = false;
  //   // movePlayer = false;

  // } else if (otherEntity == &(dragonSingleton.myEntity)) {
    
  //   // prevent collisions through a chomping dragon
  //   // TODO: simplify this if possible
  //   if (dragonSingleton.state == D_STATE_CHOMP) {
  //     playerEnt.playerEntity._worldCoords = playerEnt.player_LastPos;
  //     // playerEnt.playerVelocity.x = playerEnt.playerSpeed;
  //     // playerEnt.playerEntity._worldCoords.x -= playerEnt.playerVelocity.x;
  //     // playerEnt.playerEntity._worldCoords.y -= playerEnt.playerVelocity.y;
  //     // updateSafePlace = false;
  //     // movePlayer = false;
  //   }
  // }
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

  playerEnt.playerEntity.childEntity = NULL;

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


// attempts to pickup item
void player_pickup_item(Entity* item) {
  if (playerEnt.playerEntity.childEntity) {
    return;
  }
  // set item as child
  playerEnt.playerEntity.childEntity = item;
  sound_generic_buzz();
  sound_item_drop();
  // compute offset

  // this is inefficient but thankfully it doesn't happen each frame
  // (its not ideal because multiplication is bad)
  // TODO: compute this better!
  playerEnt.item_offset.x = playerEnt.playerVelocity.x * 10;
  playerEnt.item_offset.y = playerEnt.playerVelocity.y * 10;

}

void player_drop_item(Entity* item) {
  item->_worldCoords.x = playerEnt.playerEntity._worldCoords.x + playerEnt.item_offset.x;
  item->_worldCoords.y = playerEnt.playerEntity._worldCoords.y + playerEnt.item_offset.y;
  playerEnt.playerEntity.childEntity = NULL;
  sound_item_drop();
}

// debug action for controller pressed
void debug_action() { 
  unsigned char test;
  // printf("debug action: \n");
  // test = getTileAt(playerEnt.playerEntity._worldCoords);
  // printf("\t");
  // PRINT_VEC2(playerEnt.playerEntity._worldCoords);
  // printf("\ttile at that location: %d\n",test);
 
}

// from wikipedia; https://en.wikipedia.org/wiki/Bounding_volume#Basic_intersection_checks:
// In the case of an AABB, this tests becomes a simple set of overlap tests in terms of the unit axes. 
// For an AABB defined by M,N against one defined by O,P 
// they do not intersect if (Mx > Px) or (Ox > Nx) or (My > Py) or (Oy > Ny) or (Mz > Pz) or (Oz > Nz).


