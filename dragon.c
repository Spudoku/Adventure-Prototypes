#include "dragon.h"

//per frame behavior
STATUS dragonRoutine(Entity* thisEntity) {
    Vector2 distanceToDragon;
  //calculate state
 
  //XOR (my beloved) to flipflop toggle
  D_ENT->flags ^= 
      ((D_ENT->dragonChompCounter)-- < 1) && D_STATE_CHOMP; 

  COND_SET_BIT(((D_ENT->moveDelayCounter)-- < 1), D_STATE_MOVE, D_ENT->flags)


  //read move state, if we're not moving... do nothing
  if(!CHECK_FLAG(D_ENT->flags, D_STATE_MOVE)){
    return PASS;
  }

  //reset delay
  D_ENT->moveDelayCounter = D_MOVE_DELAY;

  //calculate taxicab but store intermediary delta vector
  distanceToDragon = thisEntity->childEntity->_worldCoords;
  SUB_ASSIGN_VEC2(distanceToDragon, thisEntity->_worldCoords)

  //bounce out taxicab distance
  if((abs(distanceToDragon.x) + abs(distanceToDragon.y)) > D_SIGHT_RANGE) {
    return PASS;
  }

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

STATUS dragonRenderer(Entity* thisEntity) {


  return UNDEFINED;
}


void dragon_OnCollision(Entity* thisEntity, Entity* otherEntity){
  D_ENT->moveDelayCounter += 100;
  return;
}

// the behavior of the dragon
// STATUS dragonBehaviorProcess(Entity* thisEntity) {
//   Vector2 distanceToDragon;
//   //calculate state
 
//   //XOR (my beloved) to flipflop toggle
//   D_ENT->flags ^= 
//       ((D_ENT->dragonChompCounter)-- < 1) && D_STATE_CHOMP; 

//   COND_SET_BIT(((D_ENT->moveDelayCounter)-- < 1), D_STATE_MOVE, D_ENT->flags)


//   //read move state, if we're not moving... do nothing
//   if(!CHECK_FLAG(D_ENT->flags, D_STATE_MOVE)){
//     return PASS;
//   }

//   //reset delay
//   D_ENT->moveDelayCounter = D_MOVE_DELAY;

//   //calculate taxicab but store intermediary delta vector
//   distanceToDragon = thisEntity->childEntity->_worldCoords;
//   SUB_ASSIGN_VEC2(distanceToDragon, thisEntity->_worldCoords)

//   //bounce out taxicab distance
//   if((abs(distanceToDragon.x) + abs(distanceToDragon.y)) > D_SIGHT_RANGE) {
//     return PASS;
//   }

//   switch((distanceToDragon.x > 0) - (distanceToDragon.x < 0)){
//     case -1:
//       thisEntity->_worldCoords.x -= D_ENT->dragonSpeed;
//       break;
//     case 1:
//       thisEntity->_worldCoords.x += D_ENT->dragonSpeed;
//       break;
//     default:
//       break;
//   }

//   switch((distanceToDragon.y > 0) - (distanceToDragon.y < 0)){
//     case -1:
//       thisEntity->_worldCoords.y -= D_ENT->dragonSpeed;
//       break;
//     case 1:
//       thisEntity->_worldCoords.y += D_ENT->dragonSpeed;
//       break;
//     default:
//       break;
//   }

//   return PASS;
// }

