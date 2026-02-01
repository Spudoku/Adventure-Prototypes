#ifndef PLAYER_MISSILE_H
#define PLAYER_MISSILE_H
#pragma bss-name (push, "PMGAREA")
// assuming double-line resolution
unsigned int player_horiz_positions[4];
unsigned int player_vert_positions[4];

unsigned int player_sprite_0[8];
unsigned int player_sprite_1[8];
unsigned int player_sprite_2[8];
unsigned int player_sprite_3[8];
unsigned char unused[304];
unsigned char missiles_graphics[128];
unsigned char player0graphics[128]; // note: for an 8x8 sprite, use bytes 60 to 68. actually, just assume that vertical position '0' is byte 64 (center of this memory)
unsigned char player1graphics[128];
unsigned char player2graphics[128];
unsigned char player3graphics[128];
#pragma bss-name (pop)

#include "util.h"

// memory locations since I haven't found pre-defined values
#define PLAYER_0_POS_HORIZ 0xD000
#define PLAYER_1_POS_HORIZ 0xD001
#define PLAYER_2_POS_HORIZ 0xD002
#define PLAYER_3_POS_HORIZ 0xD003

#define SCREEN_LEFT_BOUND 48
#define SCREEN_RIGHT_BOUND 208

#define SCREEN_TOP_BOUND 16// assuming double line resolution
#define SCREEN_BOTTOM_BOUND 112 


void set_player_horiz_position(unsigned int player, unsigned int pos, bool boundsCorrect);
void move_player_horiz_position(unsigned int player,int delta, bool boundsCorrect);
unsigned int get_player_horiz_position(unsigned int player);

void set_player_vert_position(unsigned int player, unsigned int pos, bool boundsCorrect);
void move_player_vert_position(unsigned int player,int delta, bool boundsCorrect);
unsigned int get_player_vert_position(unsigned int player);

// copies the data in the appropriate player_sprite array to its correct
// location based on position 
void write_sprite(unsigned int player, unsigned int position, bool boundsCorrect);


// sets the horizontal position (pos) of a player
// player must be between 0 and 3 inclusive
// if boundsCorrect is true, clamp the final position between
// SCREEN_LEFT_BOUND and SCREEN_RIGHT_BOUND, inclusive


// Sets the horizontal register of the corresponding 'player' to position 'pos'.
// if boundsCorrect is true, keep the player in bounds
void set_player_horiz_position(unsigned int player, unsigned int pos, bool boundsCorrect) {
    unsigned int correctedPos = pos;

    // if boundsCorrect is true, force the position to be within horizontal screen bounds
    if (boundsCorrect) {
        correctedPos = clamp_int(pos, SCREEN_LEFT_BOUND, SCREEN_RIGHT_BOUND);
    }

    switch (player) {
        case 0:
            GTIA_WRITE.hposp0 = correctedPos;
            player_horiz_positions[0] = correctedPos;
            break;
        case 1:
            GTIA_WRITE.hposp1 = correctedPos;
            player_horiz_positions[0] = correctedPos;
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
void move_player_horiz_position(unsigned int player,int delta, bool boundsCorrect) {
    unsigned int curPosition = get_player_horiz_position(player);
    unsigned int finalPosition = (unsigned int)((int)curPosition + delta);


    set_player_horiz_position(player,finalPosition,boundsCorrect);
}

unsigned int get_player_horiz_position(unsigned int player) {
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
void write_sprite(unsigned int player, unsigned int position, bool boundsCorrect) {
    // first, determine if the sprite would be rendered out of bounds
    //  determine the center of the sprite, subtract by 4
    // 
}
void set_player_vert_position(unsigned int player, unsigned int pos, bool boundsCorrect) {
    // the idea: copy the bytes from range(position - 4, position + 4) and set to pos.   
}
void move_player_vert_position(unsigned int player,int delta, bool boundsCorrect) {

}

unsigned int get_player_vert_position(unsigned int player) {
    return player_vert_positions[player % 4];
}





#endif