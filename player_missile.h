#ifndef PLAYER_MISSILE_H
#define PLAYER_MISSILE_H
/**
    POPULATING MEMORY SEGMENT
    allocated in atari_modifed.cfg
**/
#pragma bss-name (push, "PMGAREA")
// assuming double-line resolution
unsigned char player_horiz_positions[4];
unsigned char player_vert_positions[4];

// sprite arrays
unsigned char player_sprites[4][16];
unsigned char unused[304];          // it appears we can use this area safely
unsigned char missiles_graphics[4][32];
unsigned char player_graphics[4][128];

#pragma bss-name (pop)

#include "util.h"

/**
    MEMORY LOCATIONS
**/

#define PLAYER_0_POS_HORIZ 0xD000
#define PLAYER_1_POS_HORIZ 0xD001
#define PLAYER_2_POS_HORIZ 0xD002
#define PLAYER_3_POS_HORIZ 0xD003

#define SCREEN_LEFT_BOUND 48
#define SCREEN_RIGHT_BOUND 208

#define SCREEN_TOP_BOUND 24// assuming double line resolution
#define SCREEN_BOTTOM_BOUND 120

/**
    FUNCTION DECLARATIONS
**/
void setup_pmg();

void set_player_horiz_position(unsigned char player, unsigned char pos, bool boundsCorrect);
void move_player_horiz_position(unsigned char player,char delta, bool boundsCorrect);
unsigned char get_player_horiz_position(unsigned char player);

void set_player_vert_position(unsigned char player, unsigned char pos, bool boundsCorrect);
void move_player_vert_position(unsigned char player,char delta, bool boundsCorrect);
unsigned char get_player_vert_position(unsigned char player);

// copies the data in the appropriate player_sprite array to its correct
// location based on position 
void write_sprite(unsigned char player, unsigned char position);


void setup_pmg() {
    unsigned int PMBASE = 0xD407;
    unsigned int SDMCTL = 0x22F;
    unsigned int PCOLR0 = 0x2C0;
    // TODO: do any setup for player missile graphics here
    // what Ed's code appears to do is:
    // store pmg label into PMBASE ($D407)
    // move 46 into SDMCTL ($22F), which sets to double-line resolution
    // move 0x3 into GRACTL ( $D01D ), which enables PMG
    // move 0x1 into GRPRIOR ($26F), which gives player priorty?
    
    unsigned int playerDataPage = 0x38;
    unsigned int playerData = playerDataPage << 8;
    unsigned int missileLocation = (unsigned int)missiles_graphics[0];

    unsigned int zeroIndex;
    unsigned int playerIndex;
    

    // POKE(PCOLR0,0x1E);
    POKE(PMBASE,playerDataPage);
    POKE(SDMCTL,46); // I think the does: enable fetching DMA instructions, enable player/missile DMA, standard playfield

    // TODO: clear out memory more efficiently

    // THIS IS TEST CODE
    

    for (zeroIndex = 0; zeroIndex < 0x80;zeroIndex++) {
        // clear bits from missiles and players 0-1 at the same time
        POKE(missileLocation + zeroIndex,0);
        for (playerIndex = 0; playerIndex < 4; playerIndex++) {
            player_graphics[playerIndex][zeroIndex] = 0;
        }
    }
    
    GTIA_WRITE.prior = 1; // set player priorty
    GTIA_WRITE.gractl = 3; // enable PMG
    
    // // set horizontal position of p0 to 120
    // GTIA_WRITE.hposp0 = 150;
    
    // set color of player 0
    POKE(PCOLR0,0x1E);

    // GTIA_WRITE.colpm0 = (unsigned char)0x1E;
}

/**
    HORIZONTAL MOVEMENT FUNCTIONS
**/

// sets the horizontal position (pos) of a player
// player must be between 0 and 3 inclusive
// if boundsCorrect is true, clamp the final position between
// SCREEN_LEFT_BOUND and SCREEN_RIGHT_BOUND, inclusive
// Sets the horizontal register of the corresponding 'player' to position 'pos'.
// if boundsCorrect is true, keep the player in bounds
void set_player_horiz_position(unsigned char player, unsigned char pos, bool boundsCorrect) {
    unsigned char correctedPos = pos;

    // if boundsCorrect is true, force the position to be within horizontal screen bounds
    if (boundsCorrect) {
        correctedPos = clamp_char(pos, SCREEN_LEFT_BOUND, SCREEN_RIGHT_BOUND);
    }

    switch (player) {
        case 0:
            GTIA_WRITE.hposp0 = correctedPos;
            player_horiz_positions[0] = correctedPos;
            break;
        case 1:
            GTIA_WRITE.hposp1 = correctedPos;
            player_horiz_positions[1] = correctedPos;
            break;
        case 2:
            GTIA_WRITE.hposp2 = correctedPos;
            player_horiz_positions[2] = correctedPos;
            break;
        case 3:
            GTIA_WRITE.hposp3 = correctedPos;
            player_horiz_positions[3] = correctedPos;
            break;
        default:
            // invalid bounds case: write to player 0
            GTIA_WRITE.hposp0 = correctedPos;
            player_horiz_positions[0] = correctedPos;
            break;
    }


}

// effectively sets the position of 'player' by its current position plus 'delta'
void move_player_horiz_position(unsigned char player,char delta, bool boundsCorrect) {
    unsigned char curPosition = get_player_horiz_position(player);
    unsigned char finalPosition = (unsigned char)(curPosition + delta);


    set_player_horiz_position(player,finalPosition,boundsCorrect);
}

// returns current horizontal position
unsigned char get_player_horiz_position(unsigned char player) {
    return player_horiz_positions[player % 4];
}

/**
    VERTICAL MOVEMENT FUNCTIONS
**/

// TODO: write_sprite
// player: range from 0 to 3 inclusive
// position: range from 0 to 128
// the main purpose of this function is to make sure that the sprite is
// remembered if parts of it are outside of memory. However, this may not be
// necessary because I just remembered that the top and bottom 16 pixels are off screen.
// if boundsCorrect is true, then 'sprite remembering' becomes redundant.

// moves current player sprite from its current position in player_vert_position[] to a new position
// TODO: use VDELAY every other line
void write_sprite(unsigned char player, unsigned char position) {
    // first, determine if the sprite would be rendered out of bounds
    // determine the center of the sprite, subtract by 4


    // zero out current sprite
    unsigned int lowerBound = max(0,(unsigned int)player_vert_positions[player] - 8);
    unsigned int upperBound = min(128, (unsigned int)player_vert_positions[player] + 8);

    
    int i;
    int intendedPos;

    // TODO: change the way that graphics are cleaned out
    // for (i = 0; i < 128; i++) {
    //     player_graphics[player][i] = 0;
    // }
    for (i = lowerBound; i < upperBound; i++) {
        player_graphics[player][i] = 0;
    }
    // write sprite to current position;

    lowerBound = max(0,(unsigned int)position - 8);
    upperBound = min(128,(unsigned int)position + 8);

    for (i = 0; i < 16; i++) {
        intendedPos = position - 8 + i;
        if (intendedPos < 0 || intendedPos > 128) {
            continue;
        }
        player_graphics[player][intendedPos] = player_sprites[player][i];
    }


}
void set_player_vert_position(unsigned char player, unsigned char pos, bool boundsCorrect) {
    unsigned char correctedPos = pos;
    
    if (boundsCorrect) {
        
        correctedPos = clamp_char(pos,SCREEN_TOP_BOUND,SCREEN_BOTTOM_BOUND);
    }
    // the idea: copy the bytes from range(position - 4, position + 4) and set to pos.   
    write_sprite(player,correctedPos);
    
    player_vert_positions[player] = correctedPos;

}
void move_player_vert_position(unsigned char player,char delta, bool boundsCorrect) {
    unsigned char curPosition = get_player_vert_position(player);
    unsigned char finalPosition = (unsigned char)((char)curPosition + delta);


    set_player_vert_position(player,finalPosition,boundsCorrect);
}

unsigned char get_player_vert_position(unsigned char player) {
    return player_vert_positions[player % 4];
}





#endif