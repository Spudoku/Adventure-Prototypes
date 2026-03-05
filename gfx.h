#ifndef GFX_H_
#define GFX_H_

#include <atari.h>
#include <stdlib.h>
#include "gamemap.h"  //TODO: rename this to gamemap data (or similar?
#include "displaylist.h"
#include "util_structs.h"



//#define UPDATE_SCROL_REGS(pixel_x,pixel_y)      \




#define ANTIC_SCROLL_VEC2 (*(u16Vector2 *)(&(ANTIC.hscrol)))


//careful, macros have no type checking!

#define SET_ANTIC_SCROLL(pixelX, pixelY)              \
  ANTIC_SCROLL_VEC2.x = -pixelX;                       \
  ANTIC_SCROLL_VEC2.y = pixelY << 1;                       \


#define SET_VEC2_ANTIC_SCROLL(input_u16vec)       \
SET_ANTIC_SCROLL(input_u16vec.x, input_u16vec.y)  \



void map_absoluteMove(Vector2 absolutePosition);
void map_relativeMove(Vector2 relativePosition);
void map_resetPosition();


typedef struct MapData{
  Vector2 offset; //this MIGHT get consolidated to the camera
  Vector2 coarseOffset;
} MapData;


#endif