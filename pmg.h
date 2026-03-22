#ifndef PMG_H_
#define PMG_H_

#include <stdint.h>
#include <string.h>
#include "sprite.h"
#include <stdbool.h>
#include "util_structs.h"




typedef struct DoubleLine_PMGPlayerSpriteSilo{

  struct {
    Sprite* refsprite;
    int8_t cachedY;  //offset from top of visibleBytes, notice this is SIGNED
    uint8_t cachedX;
    uint8_t cachedHeight;
    uint8_t unused[7];
  } header;

  uint8_t visibleBytes[96];

  struct{
    uint8_t unused[8];
  } trailer;

} PMGPlayerSpriteSilo;



typedef struct DoubleLine_PMGInstance{

  // assuming double-line resolution
  uint8_t player_horiz_positions[4];
  uint8_t player_vert_positions[4];

  // sprite arrays
  uint8_t player_sprites[4][16];
  uint8_t unused[312];          // it appears we can use this area safely
  uint8_t missiles_graphics[4][32];
  // uint8_t player_graphics[4][128];
  PMGPlayerSpriteSilo playerGFX[4];
} PMGInstance;


//to reduce parameter passing, we can take the assumption that we arent going to
//operate on more than one PMG instance at a time.
//allows us to have singleton like programming
extern PMGInstance* activePMGInstance; 


void pmg_init(PMGInstance* pmgInstance);
// void pmg_updateActiveInstance();
void pmg_clear();

//returns location to which was inserted. values over 3 are invalid
//failure flag: 255
uint8_t pmg_addPlayerSprite(Sprite* toAdd); 

void pmgSilo_setY(PMGPlayerSpriteSilo* silo, int8_t newY);
void pmgSilo_clear(PMGPlayerSpriteSilo* silo);
void pmgSilo_writeRefSprite(PMGPlayerSpriteSilo* silo, int8_t newY);

//TODO: these things
// void pmgSilo_absoluteVerticalMove(PMGPlayerSpriteSilo* silo, int8_t newY);

// //only works when sprite is completely visible
// void pmgSilo_fastRelativeVerticalMove(PMGPlayerSpriteSilo* silo, int8_t newY);

#endif