#ifndef PLAYER_H_
#define PLAYER_H_

#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "camera.h"
#include <stdint.h>
#include "sprite.h"
#include "pmg.h"
//#include "scheduler.h"
#include <joystick.h>

//player will probably end up being effectively a singleton here

#define DEFAULT_PLAYER_SPEED 1


typedef struct PlayerEntity {
  Entity playerEntity;
  Vector2 playerVelocity;
  int playerSpeed;  //may consider making this an unsigned char
  PMGPlayerSpriteSilo *playerSilo;
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