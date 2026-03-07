#ifndef PLAYER_H_
#define PLAYER_H_

#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "camera.h"
#include <joystick.h>

//player will probably end up being effectively a singleton here



typedef struct PlayerEntity {
  Vector2 playerVelocity;
  int playerSpeed;  //may consider making this an unsigned char
  Entity playerEntity;
} PlayerEntity;




STATUS player_FrameTask(Entity* thisEntity);
STATUS playerRenderer(Entity* thisEntity);

//player specific, note how there is always a player so these manip the struct
//directly without passing a ptr
//other entities might not have this special privelge
STATUS playerInputProcess();
STATUS playerConstructor();

extern PlayerEntity playerEnt;


#endif 