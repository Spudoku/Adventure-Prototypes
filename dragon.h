#ifndef DRAGON_H_
#define DRAGON_H_

#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "entity_sprites.h"

#include "player_missile.h"
#include "player.h"

#define DEFAULT_DRAGON_SPEED 1
#define DEFAULT_DRAGON_MOVE_DELAY 1
#define DEFAULT_DRAGON_SIGHTRANGE 200


#define DRAGON_GRAPHICS_PLAYER 1     // this means that all dragons share player 1,
                              // if we only allow one dragon on screen at once

#define DRAGON_SPRITE_COUNT 3

#define DRAGON_CHOMP_DELAY 16

extern Sprite dragonSprites[];



// this should NOT be a singleton

typedef struct DragonEntity {
  // Vector2 dragonVelocity;
  int mySpeed;  //may consider making this an unsigned char
  unsigned int moveFrameDelay;
  unsigned int moveDelayCounter;
  Entity myEntity;

  // dragon-specific fields
  unsigned int id;            // which graphics player to use
  unsigned int sightRange;
  Vector2 targetLocation;
  Entity* loves;
  // Entity loves[3]; // things this dragon loves
  // Entity hates[3]; // things this dragon hates
  unsigned char activeSprite;

  unsigned int dragonChompCounter;
  
  

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

void SetSpriteByIndex(DragonEntity* thisEntity, unsigned char id);

// silly chomp animation
void DragonChomp(DragonEntity* thisEntity);

bool CollidingWithPlayer();


#endif 