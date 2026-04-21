#ifndef ORB_H_
#define ORB_H_

#include <stdio.h>

#include "util/util.h"
#include "util/util_structs.h"
#include "util/sprite.h"
#include "util/util_input.h"

#include "core-support/entity.h"
#include "core-support/camera.h"
#include "../core/sound.h"

#define ORB_DELAY 300

typedef struct InsightOrb{
  Entity entity;
  int counter;
} Orb;

extern Orb orb_singleton;
extern char *intrusiveThoughts[];

//no need for frametask or renderer, not running thoseS

void orb_OnCollision(Entity* thisEntity, Entity* otherEntity);

#endif