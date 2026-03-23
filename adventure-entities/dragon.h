#ifndef DRAGON_H_
#define DRAGON_H_

#include <stdio.h>

#include "core-support/entity.h"
#include "core-support/camera.h"
#include "../core-gfx/antic-gtia/pmg.h"

#include "util/sprite.h"
#include "util/util.h"

#define D_ENT  ((DragonEntity*)(thisEntity->entityData))
#define D_CHOMP_DELAY 16

#define D_MOVE_DELAY 1
#define D_SIGHT_RANGE 200 //in pixels


//states
#define D_STATE_MOVE  1
#define D_STATE_CHOMP 2
#define D_STATE_REST  0   //clr flag

//symbolic link, alias
#define _target childEntity

typedef struct DragonEntity {
  Entity myEntity;  //dragon entity

  uint8_t flags;

  // Vector2 dragonVelocity;
  int dragonSpeed; 
  unsigned int moveDelayCounter;

  // dragon-specific fields
  unsigned int dragonChompCounter;
  
  
    PMGPlayerSpriteSilo *dragonSilo;
} DragonEntity;


//entity base funcs
STATUS dragon_frameTask(Entity* thisEntity);
STATUS dragonRenderer(Entity* thisEntity);
void dragon_OnCollision(Entity* thisEntity, Entity* otherEntity);

STATUS dragon_Init(DragonEntity* instance);

void dragon_TrackEntity(DragonEntity* instance, Entity *toTrack);

extern uint8_t TEMP_dragon_anticIndex;  
extern DragonEntity debug_dragonSingleton;

extern Sprite dragon_idle;

#endif 