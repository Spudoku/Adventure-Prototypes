#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "dragon.h"
#include "items.h"


DragonEntity dragonEntities[3];
// //per frame behavior
STATUS dragonRoutine(Entity* thisEntity) {
  //pseudo

  // read input and set velocity
  return dragonBehaviorProcess();
}

//remember, the renderer happens as the second batch, after all game logic is
//calced
//prepare the graphics driver
STATUS dragonRenderer(Entity* thisEntity) {
  return UNDEFINED;
}

STATUS dragonBehaviorProcess() {
  // TODO: guard/pursue behavior (which will require tracking of world coordinates)
  return PASS;
}

// //init the dragon specific vars
STATUS dragonConstructor(unsigned int num){
  dragonEntities[num].dragonSpeed = 1;
  dragonEntities[num].dragonVelocity.x = 0;
  dragonEntities[num].dragonVelocity.y = 0;

  //call the "base" constructor

  entityConstructor(&(dragonEntities[num].dragonEntity), dragonRoutine, dragonRenderer);
  //assign to the dragon entity it's dummy obj item

  dragonEntities[num].dragonEntity.childEntity = &nullItem;
  // in the future, the constructor will be not ran right here, probably during
  // boot sequence
  nullItem_constructor(&nullItem);  

  return PASS;
}