#ifndef ITEMS_H_
#define ITEMS_H_

#include <stdint.h>
#include <joystick.h>

#include "util/util.h"
#include "util/util_structs.h"
#include "util/sprite.h"
#include "util/util_input.h"

#include "core-support/entity.h"
#include "core-support/camera.h"

#include "../core-gfx/antic-gtia/pmg.h"

#include "../core/sound.h"
#include "../dragon.h"

//item repo
//items are entities designed to be held by another entity to do some action
//to keep things concise, the declarations for all items will be here, but the
//defintion will be in their respective [obj].c file




/*
  Begin nullItem declarations
*/
extern Entity nullItem;


STATUS nullItem_Task(Entity* thisEntity);
STATUS nullItem_renderer(Entity* thisEntity);


//TODO: should the initialization and allocation be seperated? i.e. constructor
//being comprised of allocation and initing, rather than just initing
STATUS nullItem_constructor(Entity* thisEntity);

/*
  End nullItem declarations
*/


/*
  Start Chalice Declarations
*/

typedef struct ChaliceEntity {
  Entity chaliceEntity;
  PMGPlayerSpriteSilo *chaliceSilo;
} ChaliceEntity;

// extern CaliceEntity chaliceEnt;
extern Sprite chaliceSprite;

STATUS chalice_Task(Entity* thisEntity);
STATUS chalice_renderer(Entity* thisEntity);

void chalice_OnCollision(struct Entity* thisEntity, struct Entity* otherEntity);

STATUS chalice_constructor(Entity* thisEntity);

/*
  End Chalice Declarations
*/

#endif 