#ifndef PMG_H_
#define PMG_H_

#include <stdint.h>
#include <string.h>

typedef struct DoubleLine_PMGInstance{

  // assuming double-line resolution
  uint8_t player_horiz_positions[4];
  uint8_t player_vert_positions[4];

  // sprite arrays
  uint8_t player_sprites[4][16];
  uint8_t unused[312];          // it appears we can use this area safely
  uint8_t missiles_graphics[4][32];
  uint8_t player_graphics[4][128];
} PMGInstance;


void pmg_clear(PMGInstance* pmgInstance);

#endif