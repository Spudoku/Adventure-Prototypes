#ifndef GFX_H_
#define GFX_H_

#include <atari.h>
#include "gamemap.h"  //TODO: rename this to gamemap data (or similar?
#include "displaylist.h"
#include "util_structs.h"



void map_absoluteMove(Vector2 absolutePosition);
void map_relativeMove(Vector2 relativePosition);
void map_resetPosition();


#endif