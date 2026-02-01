#ifndef PLAYER_MISSILE_H
#define PLAYER_MISSILE_H
#pragma bss-name (push, "PMGAREA")
// assuming double-line resolution
unsigned int player_horiz_positions[4];
unsigned int player_vert_positions[4];
unsigned char unused[368];
unsigned char missilessprites[128];
unsigned char player0sprite[128];
unsigned char player1sprite[128];
unsigned char player2sprite[128];
unsigned char player3sprite[128];
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


// sets the horizontal position (pos) of a player
// player must be between 0 and 3 inclusive
// if boundsCorrect is true, clamp the final position between
// SCREEN_LEFT_BOUND and SCREEN_RIGHT_BOUND, inclusive



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

unsigned int get_player_vert_position(unsigned int player) {
    return player_vert_positions[player % 4];
}





#endif