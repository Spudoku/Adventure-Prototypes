#ifndef ENGINE_H_
#define ENGINE_H_

#include <atari.h>
#include <joystick.h>

#include "../core-gfx/gfx.h"
#include "sound.h"
#include "../adventure-entities/player.h"
#include "../adventure-entities/dragon.h"  //TEMP

void engine_Boot();
void engine_InitSingletons();
void engine_StateUpdate();
void engine_Render();
void engine_EventDispatcher();

//Engine related state tracking globals as needed

#endif