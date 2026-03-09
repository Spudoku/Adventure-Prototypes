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

<<<<<<< HEAD
=======

>>>>>>> 40d6363eb74a5ac503cff75762d8fa049e0e1db1

extern Sprite playerSprites[];
typedef struct PlayerEntity {
  Vector2 playerVelocity;
  int playerSpeed;  //may consider making this an unsigned char
  Entity playerEntity;

  unsigned int moveFrameDelay;
  unsigned int moveDelayCounter;
} PlayerEntity;


<<<<<<< HEAD
// //may also want to include a delta time if we aren't synced with vblank 1:1
=======

>>>>>>> 40d6363eb74a5ac503cff75762d8fa049e0e1db1

STATUS player_FrameTask(Entity* thisEntity);
STATUS playerRenderer(Entity* thisEntity);

//player specific, note how there is always a player so these manip the struct
//directly without passing a ptr
//other entities might not have this special privelge
STATUS playerInputProcess();
STATUS playerConstructor();

<<<<<<< HEAD
void TrackMoveDelayFramesPlayer();
=======
extern PlayerEntity playerEnt;


>>>>>>> 40d6363eb74a5ac503cff75762d8fa049e0e1db1
#endif 