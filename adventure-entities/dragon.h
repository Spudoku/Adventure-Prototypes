#ifndef DRAGON_H_
#define DRAGON_H_

#include <stdio.h>

#include "../core/engine.h"
#include "core-support/entity.h"
#include "core-support/camera.h"
#include "../core-gfx/antic-gtia/pmg.h"

#include "util/sprite.h"
#include "util/util.h"
#include "../core/sound.h"

#include "player.h"     // for accessing TEMP_player_anticIndex

#define D_ENT  ((DragonEntity*)(thisEntity->entityData))
#define D_CHOMP_DELAY 16

#define D_MOVE_DELAY 2    // this is to make it slower than player
#define D_SIGHT_RANGE 500 //in pixels / scan lines


//states
#define D_STATE_MOVE  1
#define D_STATE_CHOMP 2
#define D_STATE_REST  0   

//symbolic link, alias
#define _target childEntity

typedef struct DragonEntity {
  Entity myEntity;  //dragon entity

  unsigned char state;

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
extern DragonEntity dragonSingleton;

extern Sprite dragon_idle;

void check_if_eating();

#endif 