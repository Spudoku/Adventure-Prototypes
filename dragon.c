#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "dragon.h"
#include "items.h"



Vector2 test;
// //per frame behavior
STATUS dragonRoutine(Entity* thisEntity) {
  

  // read input and set velocity
  return dragonBehaviorProcess(thisEntity);
}

//remember, the renderer happens as the second batch, after all game logic is
//calced
//prepare the graphics driver
STATUS dragonRenderer(Entity* thisEntity) {
  return UNDEFINED;
}

// the behavior of the dragon
STATUS dragonBehaviorProcess(Entity* thisEntity) {
  // TODO: guard/pursue behavior (which will require tracking of world coordinates)
  
  // test = chooseTargetLocation(id);
  // dragonEntities[id].targetLocation = test;
  

  return PASS;
}

// //init the dragon specific vars

STATUS dragonConstructor(Entity* subEntity,DragonEntity* theSuperEntity){
  // thisEntity.myEntity.superEntity = thisEntity;
  subEntity->superEntity = theSuperEntity;
  theSuperEntity->dragonSpeed = DEFAULT_DRAGON_SPEED;
  theSuperEntity->sightRange = DEFAULT_DRAGON_SIGHTRANGE;
  theSuperEntity->targetLocation = chooseTargetLocation(subEntity);
  return PASS;
}

Vector2 chooseTargetLocation(Entity* thisEntity) {
  
  struct Vector2 newLocation;
  unsigned int dist;
  newLocation.x = 100;
  newLocation.y = 0;

  // // TODO: go through each LOVE and compare location
  // newLocation = dragonEntities[dragonNum].loves.eyeCoords;
  dist = Vector2Dist(thisEntity->eyeCoords,newLocation);


  return newLocation;
}

void moveTowards(Entity* thisEntity, Vector2* location) {
  int curX = thisEntity->eyeCoords.x;
  int curY = thisEntity->eyeCoords.y;

  int deltaY = location->y - curY;
  int deltaX = location->x - curX;
  DragonEntity* dEntity = (DragonEntity*)(thisEntity->superEntity);
  // vertical movement
  if (abs(deltaY) > dEntity->dragonSpeed) {

  }
}