#ifndef PLAYER_H_
#define PLAYER_H_

#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include <joystick.h>

//player will probably end up being effectively a singleton here

// typedef struct Player{


// };

// typedef struct Entity{
//   STATUS (*frameTask)(Entity* thisEntity);
//   STATUS (*renderer)(Entity* thisEntity);
//   Vector2 eyeCoords;
// };



typedef struct PlayerEntity {
  Vector2 playerVelocity;
  int playerSpeed;  //may consider making this an unsigned char
  Entity playerEntity;
} PlayerEntity;


// extern STATUS entityConstructor(Entity* thisEntity, 
//                       STATUS (*frameTask)(Entity* thisEntity), 
//                       STATUS (*renderer)(Entity* thisEntity));

// Vector2 playerVelocity;
// int playerSpeed;  //may consider making this an unsigned char
// //may also want to include a delta time if we aren't synced with vblank 1:1

STATUS playerRoutine(Entity* thisEntity);
STATUS playerRenderer(Entity* thisEntity);

//player specific, note how there is always a player so these manip the struct
//directly without passing a ptr
//other entities might not have this special privelge
STATUS playerInputProcess();
STATUS playerConstructor();


#endif 