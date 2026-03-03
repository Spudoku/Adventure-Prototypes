#ifndef PLAYER_MISSILE_H
#define PLAYER_MISSILE_H
/**
    POPULATING MEMORY SEGMENT
    allocated in atari_modifed.cfg
**/

#include "entity_sprites.h"

// TODO: 
// 0. make entities store sprite ids; write any necessary helpers
// 1. change player_sprites and missile_sprites to be arrays of Sprite and
//  Missile_Sprite pointers
// 2. change write_sprite to use Sprite* and access the bytes from it. basically
//  the idea is to make player_sprite and missile_sprites to be 'registers' that
//  write_sprite accesses
// 3. create data structures for sprites in test.c, then write helper methods
//  to set 'default' sprites for each entity
// other: 
//      make memory segment purely for an array of sprites and missile_sprites?
//      

// assuming double-line resolution
extern unsigned char player_horiz_positions[4];
extern unsigned char player_vert_positions[4];
extern unsigned char missile_horiz_positions[4];
extern unsigned char missile_vert_positions[4];
// sprite arrays
// unsigned char player_sprites[4][32];
extern Missile_Sprite* missile_sprites[4];
extern Sprite* player_sprites[4];
extern unsigned char unused[352];             // it appears we can use this area safely
                                        // I might use it to store all sprites
extern unsigned char missiles_graphics[4][32];
extern unsigned char player_graphics[4][128];



#include "util.h"
#include <string.h>
#include <assert.h>
#include <stdbool.h>

/**
    MEMORY LOCATIONS
**/



#define PLAYER_0_POS_HORIZ 0xD000
#define PLAYER_1_POS_HORIZ 0xD001
#define PLAYER_2_POS_HORIZ 0xD002
#define PLAYER_3_POS_HORIZ 0xD003

#define SCREEN_LEFT_BOUND 48
#define SCREEN_RIGHT_BOUND 200
#define SCREEN_HORIZ_CENTER (SCREEN_RIGHT_BOUND + SCREEN_LEFT_BOUND) / 2

#define SCREEN_TOP_BOUND 8// assuming double line resolution

#define SCREEN_BOTTOM_BOUND 116

#define SCREEN_VERT_CENTER (SCREEN_BOTTOM_BOUND + SCREEN_TOP_BOUND) / 2


/**
    FUNCTION DECLARATIONS
**/
extern void setup_pmg();

extern void set_player_horiz_position(unsigned char playerID, unsigned char pos, bool boundsCorrect);
extern void move_player_horiz_position(unsigned char playerID,char delta, bool boundsCorrect);
extern unsigned char get_player_horiz_position(unsigned char playerID);

extern void set_player_vert_position(unsigned char playerID, unsigned char pos, bool boundsCorrect);
extern void move_player_vert_position(unsigned char playerID,char delta, bool boundsCorrect);
extern unsigned char get_player_vert_position(unsigned char playerID);

// copies the data in the appropriate player_sprite array to its correct
// location based on position 
extern void write_sprite(unsigned char playerID, unsigned char position);


// collision helpers
extern unsigned char player_to_player_collisions(unsigned char player);
extern unsigned char player_to_playfield_collisions(unsigned char player);
extern unsigned char missile_to_player_collisions(unsigned char player);
extern unsigned char missile_to_playfield_collisions(unsigned char missile);

extern bool collision_with_index(unsigned char data, unsigned char index);




#endif