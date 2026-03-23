#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdint.h>
#include <joystick.h>

#include "util/util.h"
#include "util/util_structs.h"
#include "util/sprite.h"
#include "util/util_input.h"

#include "core-support/entity.h"
#include "core-support/camera.h"
#include "items/items.h"

#include "../core-gfx/antic-gtia/pmg.h"


//player will probably end up being effectively a singleton here

#define DEFAULT_PLAYER_SPEED 1


typedef struct PlayerEntity {
  Entity playerEntity;
  Vector2 playerVelocity;
  int playerSpeed;  //may consider making this an unsigned char
  PMGPlayerSpriteSilo *playerSilo;
  Vector2 player_LastPos;
} PlayerEntity;




STATUS player_FrameTask(Entity* thisEntity);
STATUS playerRenderer(Entity* thisEntity);
void player_OnCollide(Entity* thisEntity, Entity* otherEntity);

//player specific, note how there is always a player so these manip the struct
//directly without passing a ptr
//other entities might not have this special privelge
STATUS playerInputProcess();
STATUS playerConstructor();

extern PlayerEntity playerEnt;
extern Sprite playerSprite;
//temporary, pmg will eventually assign X based on its cached value that
//player will assign. i just want it working
extern uint8_t TEMP_player_anticIndex;  


#endif 