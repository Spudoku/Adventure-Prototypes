#ifndef GFX_H_
#define GFX_H_

#include <atari.h>
#include <stdlib.h>

#include "antic-gtia/charmap.h"
#include "antic-gtia/displaylist.h"
#include "antic-gtia/pmg.h"

#include "util/sprite.h"
#include "util/util_structs.h"

#include "gamemap.h"  //TODO: rename this to gamemap data (or similar?



#define SCR_RES_X 160
#define SCR_RES_Y 96

#define ANTIC_SCROLL_VEC2 (*(u16Vector2 *)(&(ANTIC.hscrol)))


//careful, macros have no type checking!
//antic will auto truncate a value above 15.. but also go right to go left on x
#define SET_ANTIC_SCROLL(pixelX, pixelY)              \
  ANTIC_SCROLL_VEC2.x = -pixelX;                       \
  ANTIC_SCROLL_VEC2.y = pixelY << 1;                       \


#define SET_VEC2_ANTIC_SCROLL(input_u16vec)       \
SET_ANTIC_SCROLL(input_u16vec.x, input_u16vec.y)  \



void map_absoluteMove(Vector2 absolutePosition);  //for resetting HARD
void map_relativeMove(Vector2 relativePosition);
void map_fastAbsoluteMove(Vector2 absolutePosition);
void map_resetPosition();


typedef struct MapData{
  Vector2 offset; //this MIGHT get consolidated to the camera
  Vector2 coarseOffset;
} MapData;

extern PMGInstance pmgMainInstance; //in the future this will be a changable ptr

void gfx_Init();

#endif