#ifndef PMG_H_
#define PMG_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "util/sprite.h"
#include "util/util_structs.h"
#include "util/util.h"


// TODO: redo this structure?
typedef struct DoubleLine_PMGPlayerSpriteSilo{

  struct {
    Sprite* refsprite;
    int8_t cachedY;  //offset from top of visibleBytes, notice this is SIGNED
    uint8_t cachedX;  //future use: caches the value that would be in the hpos
    uint8_t cachedHeight; //future use: save cycles by reducing indirection
    uint8_t unused[11];
  } header;

  uint8_t visibleBytes[96];

  struct{
    uint8_t unused[16];
  } trailer;

} PMGPlayerSpriteSilo;



typedef struct DoubleLine_PMGInstance{

  // assuming double-line resolution

  uint8_t unused[512];
  PMGPlayerSpriteSilo playerGFX[4];
} PMGInstance;


//to reduce parameter passing, we can take the assumption that we arent going to
//operate on more than one PMG instance at a time.
//allows us to have singleton like programming
extern PMGInstance* activePMGInstance; 


void pmg_Init(PMGInstance* pmgInstance);
// void pmg_updateActiveInstance();
void pmg_clear();

//returns location to which was inserted. values over 3 are invalid
//failure flag: 255
uint8_t pmg_addPlayerSprite(Sprite* toAdd); 

void pmgSilo_setY(PMGPlayerSpriteSilo* silo, int8_t newY);
void pmgSilo_clear(PMGPlayerSpriteSilo* silo);
void pmgSilo_writeRefSprite(PMGPlayerSpriteSilo* silo, int8_t newY);

// collision helpers
extern unsigned char player_to_player_collisions(unsigned char player);
extern unsigned char player_to_playfield_collisions(unsigned char player);
extern unsigned char missile_to_player_collisions(unsigned char player);
extern unsigned char missile_to_playfield_collisions(unsigned char missile);

extern bool collision_with_index(unsigned char data, unsigned char index);

#endif