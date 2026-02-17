#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "dragon.h"
#include "items.h"


DragonEntity dragonEntities[3];
// //per frame behavior
STATUS dragonRoutine(Entity* thisEntity,unsigned int id) {
  //pseudo

  // read input and set velocity
  return dragonBehaviorProcess(id);
}

//remember, the renderer happens as the second batch, after all game logic is
//calced
//prepare the graphics driver
STATUS dragonRenderer(Entity* thisEntity) {
  return UNDEFINED;
}

STATUS dragonBehaviorProcess(unsigned int id) {
  // TODO: guard/pursue behavior (which will require tracking of world coordinates)
  dragonEntities[id].targetLocation = chooseTargetLocation(id);
  return PASS;
}

// //init the dragon specific vars
STATUS dragonConstructor(unsigned int num){
  dragonEntities[num].dragonSpeed = 1;
  dragonEntities[num].dragonVelocity.x = 0;
  dragonEntities[num].dragonVelocity.y = 0;

  dragonEntities[num].sightRange = 20;

  dragonEntities[num].id = num;
  //call the "base" constructor

  entityConstructor(&(dragonEntities[num].dragonEntity), dragonRoutine, dragonRenderer);
  //assign to the dragon entity it's dummy obj item

  dragonEntities[num].dragonEntity.childEntity = &nullItem;
  // dragonEntities[num].loves[0] = playerEnt;
  // in the future, the constructor will be not ran right here, probably during
  // boot sequence
  nullItem_constructor(&nullItem);  

  return PASS;
}

Vector2 chooseTargetLocation(unsigned int dragonNum) {
  struct Vector2 newLocation;
  unsigned int dist;
  newLocation.x = 0;
  newLocation.y = 0;

  // TODO: go through each LOVE and compare location
  newLocation = dragonEntities[dragonNum].loves.eyeCoords;

  return newLocation;
}