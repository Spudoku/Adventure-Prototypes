#ifndef DRAGON_H_
#define DRAGON_H_

#include "util.h"
#include "util_structs.h"
#include "entity.h"

#define DEFAULT_DRAGON_SPEED 1
#define DEFAULT_DRAGON_MOVE_DELAY 1
#define DEFAULT_DRAGON_SIGHTRANGE 20
// this should NOT be a singleton

typedef struct DragonEntity {
  // Vector2 dragonVelocity;
  int mySpeed;  //may consider making this an unsigned char
  unsigned int moveFrameDelay;
  unsigned int moveDelayCounter;
  Entity myEntity;

  // dragon-specific fields
  unsigned int id;
  unsigned int sightRange;
  Vector2 targetLocation;
  Entity* loves;
  // Entity loves[3]; // things this dragon loves
  // Entity hates[3]; // things this dragon hates
} DragonEntity;

extern DragonEntity dragonEntities[];

// //may also want to include a delta time if we aren't synced with vblank 1:1

STATUS dragonRoutine(Entity* thisEntity);
STATUS dragonRenderer(Entity* thisEntity);


//other entities might not have this special privilege
// STATUS dragonInputProcess();
STATUS dragonBehaviorProcess(Entity* thisEntity);
STATUS dragonConstructor(Entity* subEntity, DragonEntity* theSuperEntity);

Vector2 chooseTargetLocation(DragonEntity* thisEntity);

void moveTowards(DragonEntity* thisEntity, Vector2* location);

void TrackMoveDelayFrames(DragonEntity* thisEntity);


#endif 