#ifndef ENGINE_H_
#define ENGINE_H_

#include <atari.h>
#include <joystick.h>
#include <setjmp.h>
#include "../core-gfx/gfx.h"
#include "sound.h"
#include "../adventure-entities/player.h"
#include "../adventure-entities/dragon.h"  //TEMP
#include "../adventure-entities/map-interactables/orb.h"
#include "../adventure-entities/items/items.h"
#include <peekpoke.h>

#define RANDOM_LOCATIONS_COUNT 4 // this should be a power of 2

void engine_Boot();
void engine_InitSingletons();
void engine_StateUpdate();
void engine_Render();
void engine_EventDispatcher();

void trigger_warm_reset(void);

void end_game();

void end_game_good();

void game_loop();

void setup_reset_handler();

extern Vector2 startingLocations[];
extern Vector2 swordLocations[];

//Engine related state tracking globals as needed

#endif