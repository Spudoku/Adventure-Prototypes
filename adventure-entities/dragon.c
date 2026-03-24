#include "dragon.h"

DragonEntity  debug_dragonSingleton = {
  {dragon_frameTask, dragonRenderer, dragon_OnCollision, (void *)&debug_dragonSingleton, (Entity*)NULL,//entity
    {{624, 560}, {0,0}, {0,0},{8,20}}},//entity.transform
    0,1,D_MOVE_DELAY,D_CHOMP_DELAY, NULL
};

//per frame behavior
STATUS dragon_frameTask(Entity* thisEntity) {
  Vector2 distanceToDragon;


  //calculate state
 
  //XOR (my beloved) to flipflop toggle
  // D_ENT->flags ^= 
  //     ((D_ENT->dragonChompCounter) < 1) && D_STATE_CHOMP; 

  //sets to true when move delay is 0. NOT A TOGGLE
  COND_SET_BIT(((D_ENT->moveDelayCounter)-- < 1), D_STATE_MOVE, D_ENT->flags)
  

  if(D_ENT->dragonChompCounter-- < 1){  
    D_ENT->dragonChompCounter = D_CHOMP_DELAY;
    D_ENT->flags ^= D_STATE_CHOMP;
  }

  //read move state, if we're not moving... do nothing
  if(!CHECK_FLAG(D_ENT->flags, D_STATE_MOVE)){
    return PASS;
  }

  //if here, should bee moving, now time to do math

  //reset delay
  D_ENT->moveDelayCounter = D_MOVE_DELAY;

  //calculate taxicab but store intermediary delta vector
  distanceToDragon = thisEntity->_target->_worldCoords;
  SUB_ASSIGN_VEC2(distanceToDragon, thisEntity->_worldCoords)



  //bounce out taxicab distance
  if((abs(distanceToDragon.x) + abs(distanceToDragon.y)) > D_SIGHT_RANGE) {
    return PASS;
  }

  
  //now we're in range

  //calculate the new x
  switch((distanceToDragon.x > 0) - (distanceToDragon.x < 0)){
    case -1:
      thisEntity->_worldCoords.x -= D_ENT->dragonSpeed;
      break;
    case 1:
      thisEntity->_worldCoords.x += D_ENT->dragonSpeed;
      break;
    default:
      break;
  }

  //and the new y
  switch((distanceToDragon.y > 0) - (distanceToDragon.y < 0)){
    case -1:
      thisEntity->_worldCoords.y -= D_ENT->dragonSpeed;
      break;
    case 1:
      thisEntity->_worldCoords.y += D_ENT->dragonSpeed;
      break;
    default:
      break;
  }

  return PASS;
}

uint8_t TEMP_dragon_anticIndex;


void dragon_OnCollision(Entity* thisEntity, Entity* otherEntity){
  D_ENT->moveDelayCounter += 100;
  return;
}



uint8_t dragon_chompingBitmap[] = {
  0b10000000,
  0b01000110,
  0b00101111,
  0b00011011,
  0b00011110,
  0b00101110,
  0b01000100,
  0b10000100,
  0b00011110,
  0b00111111,
  0b01111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b00111100,
  0b00001000,
  0b10001111,
  0b11100001,
  0b00111111,
};
Sprite dragon_chomping = {
  sizeof(dragon_chompingBitmap), 
  GTIA_COLOR_LIGHTRED, 
  dragon_chompingBitmap
};


uint8_t dragon_idleBitmap[] = {
  0b00000110,
  0b00001111,
  0b11110011,
  0b11111110,
  0b00001110,
  0b00000100,
  0b00000100,
  0b00011110,
  0b00111111,
  0b01111111,
  0b11100011,
  0b11000011,
  0b11000011,
  0b11000111,
  0b11111111,
  0b00111100,
  0b00001000,
  0b10001111,
  0b11100001,
  0b00111111,
};
Sprite dragon_idle = {
  sizeof(dragon_idleBitmap),
  GTIA_COLOR_LIGHTRED,
  dragon_idleBitmap
};

//some temp code here to bruteforce pmg
STATUS dragon_Init(DragonEntity* instance){
  uint8_t pmg_index;
  pmg_index = pmg_addPlayerSprite(&dragon_idle);

  if(pmg_index < 4){
    //schedulerData.antic_P2PCollisionLookupTable[pmg_index] = (Entity *)&playerEnt;
    //schedulerData.antic_P2PCollisionLookupMask |= (1 << pmg_index);
    instance->dragonSilo = activePMGInstance->playerGFX + pmg_index;
    TEMP_dragon_anticIndex = pmg_index;
  }
  printf("Dragon debug antic index: %d\n", TEMP_dragon_anticIndex);

  instance->myEntity._worldCoords.x = 400;
  instance->myEntity._worldCoords.y = 560;
  //printf("Address: %d\n", %d)
  return PASS;
}

//quick and dirty track
void dragon_TrackEntity(DragonEntity* instance, Entity *toTrack){
  instance->myEntity.childEntity = toTrack;
}

STATUS dragonRenderer(Entity* thisEntity) {
  
  thisEntity->_eyeCoords = convertToEyeCoords(thisEntity->_worldCoords);
  //incomplete

  //quick and dirty hide
  if(!objectVisible(&(thisEntity->transform))){
    (&(GTIA_WRITE.hposp0))[TEMP_dragon_anticIndex] = 0;
    return PASS;
  }

  (&(GTIA_WRITE.hposp0))[TEMP_dragon_anticIndex] = thisEntity->_eyeCoords.x 
            + HPOSP_MIN + thisEntity->_objectAnchorPoint.x;
  
  
  if(CHECK_FLAG(D_ENT->flags, D_STATE_CHOMP)){
    D_ENT->dragonSilo->header.refsprite = &dragon_chomping;
  } else{
    D_ENT->dragonSilo->header.refsprite = &dragon_idle;
  }
  //printf("%d\n",(&(GTIA_WRITE.hposp0))[TEMP_player_anticIndex] );
  pmgSilo_setY(D_ENT->dragonSilo, thisEntity->_eyeCoords.y);


  return PASS;
}
