#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "dragon.h"
#include "items.h"
#include "player.h"

int test;
int dummy;

Vector2 testLocation;

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
  
  DragonEntity* dEntity = (DragonEntity*)(thisEntity->superEntity);
  // MOVEMENT
    TrackMoveDelayFrames(dEntity);

  if (dEntity->moveDelayCounter >= dEntity->moveFrameDelay) {
    dEntity->targetLocation = chooseTargetLocation(dEntity);
    testLocation.x = dEntity->targetLocation.x;
    testLocation.y = dEntity->targetLocation.y;
    moveTowards(dEntity,&(dEntity->targetLocation));
  }
  

  return PASS;
}

// //init the dragon specific vars

STATUS dragonConstructor(Entity* subEntity,DragonEntity* theSuperEntity){
  // thisEntity.myEntity.superEntity = thisEntity;
  subEntity->superEntity = theSuperEntity;
  theSuperEntity->mySpeed = DEFAULT_DRAGON_SPEED;
  theSuperEntity->sightRange = DEFAULT_DRAGON_SIGHTRANGE;
  theSuperEntity->targetLocation = chooseTargetLocation(theSuperEntity);

  theSuperEntity->moveFrameDelay = DEFAULT_DRAGON_MOVE_DELAY;
  theSuperEntity->moveDelayCounter = 0;
  return PASS;
}

// returns a target location
Vector2 chooseTargetLocation(DragonEntity* thisEntity) {
  
  struct Vector2 newLocation;
  unsigned int dist;
  // currently relies on the 'loves' Entity
  newLocation.x = thisEntity->loves->eyeCoords.x;
  newLocation.y = thisEntity->loves->eyeCoords.y;


  return newLocation;
}

// moves a given dragon entity towards an x/y coordinate
void moveTowards(DragonEntity* thisEntity, Vector2* location) {
  Entity* transformEntity = &(thisEntity->myEntity);
  
  if (location->y > transformEntity->eyeCoords.y) {
    // move down
    transformEntity->eyeCoords.y += thisEntity->mySpeed;
  } else if (location->y < transformEntity->eyeCoords.y) {
    transformEntity->eyeCoords.y -= thisEntity->mySpeed;
  }

  if (location->x > transformEntity->eyeCoords.x) {
    // move left
    transformEntity->eyeCoords.x += thisEntity->mySpeed;
  } else if (location->x < transformEntity->eyeCoords.x) {

    transformEntity->eyeCoords.x -= thisEntity->mySpeed;
  }

}

// increment moveDelay frames
void TrackMoveDelayFrames(DragonEntity* thisEntity) {
  thisEntity->moveDelayCounter++;
  if (thisEntity->moveDelayCounter > thisEntity->moveFrameDelay) {
    thisEntity->moveDelayCounter = 0;
  }
}