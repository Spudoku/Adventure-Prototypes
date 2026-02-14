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
} DragonEntity;



// //may also want to include a delta time if we aren't synced with vblank 1:1

STATUS dragonRoutine(Entity* thisEntity);
STATUS dragonRenderer(Entity* thisEntity);


//other entities might not have this special privilege
// STATUS dragonInputProcess();
STATUS dragonBehaviorProcess();
STATUS dragonConstructor();


#endif 