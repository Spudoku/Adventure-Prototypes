#ifndef PLAYER_H_
#define PLAYER_H_

#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "camera.h"
#include <joystick.h>
#include "entity_sprites.h"
#include "player_missile.h"

#define PLAYER_GRAPHICS_PLAYER 0
#define PLAYER_SPRITE_COUNT 1

//player will probably end up being effectively a singleton here


extern Sprite playerSprites[];

typedef struct PlayerEntity {
  Vector2 playerVelocity;
  int playerSpeed;  //may consider making this an unsigned char
  Entity playerEntity;

  unsigned int moveFrameDelay;
  unsigned int moveDelayCounter;
} PlayerEntity;


// //may also want to include a delta time if we aren't synced with vblank 1:1

STATUS player_FrameTask(Entity* thisEntity);
STATUS playerRenderer(Entity* thisEntity);

//player specific, note how there is always a player so these manip the struct
//directly without passing a ptr
//other entities might not have this special privelge
STATUS playerInputProcess();
STATUS playerConstructor();

void TrackMoveDelayFramesPlayer();
extern PlayerEntity playerEnt;


#endif 