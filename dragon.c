#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "dragon.h"
#include "items.c"

extern DragonEntity dragonEnt;

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
STATUS dragonConstructor(){
  dragonEnt.dragonSpeed = 1;
  dragonEnt.dragonVelocity.x = 0;
  dragonEnt.dragonVelocity.y = 0;

  //call the "base" constructor

  // entityConstructor(&(dragonEnt.dragonEntity), dragonRoutine, dragonRenderer);
  //assign to the dragon entity it's dummy obj item

  dragonEnt.dragonEntity.childEntity = &nullItem;
  // in the future, the constructor will be not ran right here, probably during
  // boot sequence
  nullItem_constructor(&nullItem);  

  return PASS;
}