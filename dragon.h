#ifndef DRAGON_H_
#define DRAGON_H_

#include "util.h"
#include "util_structs.h"
#include "entity.h"


// this should NOT be a singleton

typedef struct DragonEntity {
  Vector2 dragonVelocity;
  int dragonSpeed;  //may consider making this an unsigned char
  Entity dragonEntity;

  // dragon-specific fields
  unsigned int id;
  unsigned int sightRange;
  Vector2 targetLocation;
  Entity loves;
  // Entity loves[3]; // things this dragon loves
  // Entity hates[3]; // things this dragon hates
} DragonEntity;

extern DragonEntity dragonEntities[];

// //may also want to include a delta time if we aren't synced with vblank 1:1

STATUS dragonRoutine(Entity* thisEntity,unsigned int id);
STATUS dragonRenderer(Entity* thisEntity);


//other entities might not have this special privilege
// STATUS dragonInputProcess();
STATUS dragonBehaviorProcess(unsigned int id);
STATUS dragonConstructor();

Vector2 chooseTargetLocation(unsigned int dragonNum);
void moveTowards(unsigned int dragonNum, Vector2 location);


#endif 