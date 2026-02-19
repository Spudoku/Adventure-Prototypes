#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "dragon.h"
#include "items.h"


int test;

// //per frame behavior
STATUS dragonRoutine(Entity* thisEntity) {
  test++;

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

Vector2 chooseTargetLocation(Entity* thisEntity) {
  
  struct Vector2 newLocation;
  unsigned int dist;
  newLocation.x = 100;
  newLocation.y = 55;

  // // TODO: go through each LOVE and compare location
  // newLocation = dragonEntities[dragonNum].loves.eyeCoords;
  // dist = Vector2Dist(thisEntity->eyeCoords,newLocation);


  return newLocation;
}

void moveTowards(DragonEntity* thisEntity, Vector2* location) {
  Entity* transformEntity = &(thisEntity->myEntity);
  int deltaY = location->y - transformEntity->eyeCoords.y;
  int deltaX = location->x - transformEntity->eyeCoords.x;

  // horizontal movement
  if (deltaX < 0) {
    // moving left
    if (deltaX <= -(thisEntity->dragonSpeed)) {
      transformEntity->eyeCoords.x = location->x;
    } else {
      transformEntity->eyeCoords.x -= thisEntity->dragonSpeed;
    }
  } else {
    if (deltaX <= (thisEntity->dragonSpeed)) {
      transformEntity->eyeCoords.x = location->x;
    } else {
      transformEntity->eyeCoords.x += thisEntity->dragonSpeed;
    }
  }

  if (deltaY < 0) {
    // moving up
    if (deltaY <= -(thisEntity->dragonSpeed)) {
      transformEntity->eyeCoords.y = location->y;
    } else {
      transformEntity->eyeCoords.y -= thisEntity->dragonSpeed;
    }
  } else {
    if (deltaY <= (thisEntity->dragonSpeed)) {
      transformEntity->eyeCoords.y = location->y;
    } else {
      transformEntity->eyeCoords.y += thisEntity->dragonSpeed;
    }
  }

  

  // thisEntity->myEntityeyeCoords.x += 1;
}