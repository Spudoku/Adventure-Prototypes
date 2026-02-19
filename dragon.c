#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "dragon.h"
#include "items.h"
#include "player.h"

int test;
int dummy;

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
  DragonEntity* dEntity = (DragonEntity*)(thisEntity->superEntity);
  moveTowards(dEntity,&(dEntity->targetLocation));

  

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

Vector2 chooseTargetLocation(DragonEntity* thisEntity) {
  
  struct Vector2 newLocation;
  unsigned int dist;
  newLocation.x = thisEntity->loves->eyeCoords.x;
  newLocation.y = thisEntity->loves->eyeCoords.y;

  // // TODO: go through each LOVE and compare location
  // newLocation = dragonEntities[dragonNum].loves.eyeCoords;
  // dist = Vector2Dist(thisEntity->eyeCoords,newLocation);


  return newLocation;
}

void moveTowards(DragonEntity* thisEntity, Vector2* location) {
  Entity* transformEntity = &(thisEntity->myEntity);
  
  if (location->y > transformEntity->eyeCoords.y) {
    // move down
    transformEntity->eyeCoords.y += thisEntity->dragonSpeed;
  } else if (location->y > transformEntity->eyeCoords.y) {
    transformEntity->eyeCoords.y -= thisEntity->dragonSpeed;
  }
test++;
  if (location->x > transformEntity->eyeCoords.x) {
    // move down
    transformEntity->eyeCoords.x += thisEntity->dragonSpeed;
  } else if (location->x > transformEntity->eyeCoords.x) {
    transformEntity->eyeCoords.x -= thisEntity->dragonSpeed;
  }
  dummy++;
  // thisEntity->myEntityeyeCoords.x += 1;
}