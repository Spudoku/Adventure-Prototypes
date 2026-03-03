
// // #include "player_missile.h"

// // memory segments



// void setup_pmg() {
//     // TODO: do any setup for player missile graphics here
//     // what Ed's code appears to do is:
//     // store pmg label into PMBASE ($D407)
//     // move 46 into SDMCTL ($22F), which sets to double-line resolution
//     // move 0x3 into GRACTL ( $D01D ), which enables PMG
//     // move 0x1 into GRPRIOR ($26F), which gives player priorty?

//     ANTIC.pmbase = (unsigned int)player_horiz_positions >> 8;

//     //zero out the section just in case,
//     //this looks dangerous but you see. i'm more dangerous than this
//     memset(player_horiz_positions, 0, 1024);    //1024 bytes are avail in here

//     OS.sdmctl = 46;
//     GTIA_WRITE.gractl = 3; // enable PMG
//     OS.gprior = PRIOR_P03_PF03;

    
//     // set color of players
//     OS.pcolr0 = COLOR_YELLOW;
//     OS.pcolr1 = COLOR_RED;

// }

// /**
//     HORIZONTAL MOVEMENT FUNCTIONS
// **/

// // sets the horizontal position (pos) of a player
// // player must be between 0 and 3 inclusive
// // if boundsCorrect is true, clamp the final position between
// // SCREEN_LEFT_BOUND and SCREEN_RIGHT_BOUND, inclusive
// // Sets the horizontal register of the corresponding 'player' to position 'pos'.
// // if boundsCorrect is true, keep the player in bounds
// void set_player_horiz_position(unsigned char playerID, unsigned char pos, bool boundsCorrect) {
//     unsigned char correctedPos = pos;

//     // if boundsCorrect is true, force the position to be within horizontal screen bounds
//     if (boundsCorrect) {
//         correctedPos = clamp_char(pos, SCREEN_LEFT_BOUND, SCREEN_RIGHT_BOUND);
//     }

//     switch (playerID) {
//         case 0:
//             GTIA_WRITE.hposp0 = correctedPos;
//             player_horiz_positions[0] = correctedPos;
//             break;
//         case 1:
//             GTIA_WRITE.hposp1 = correctedPos;
//             player_horiz_positions[1] = correctedPos;
//             break;
//         case 2:
//             GTIA_WRITE.hposp2 = correctedPos;
//             player_horiz_positions[2] = correctedPos;
//             break;
//         case 3:
//             GTIA_WRITE.hposp3 = correctedPos;
//             player_horiz_positions[3] = correctedPos;
//             break;
//         default:
//             // invalid bounds case: write to player 0
//             GTIA_WRITE.hposp0 = correctedPos;
//             player_horiz_positions[0] = correctedPos;
//             break;
//     }


// }

// // effectively sets the position of 'player' by its current position plus 'delta'
// void move_player_horiz_position(unsigned char playerID,char delta, bool boundsCorrect) {
//     unsigned char curPosition = get_player_horiz_position(playerID);
//     unsigned char finalPosition = (unsigned char)(curPosition + delta);


//     set_player_horiz_position(playerID,finalPosition,boundsCorrect);
// }

// // returns current horizontal position
// unsigned char get_player_horiz_position(unsigned char playerID) {
//     return player_horiz_positions[playerID % 4];
// }

// /**
//     VERTICAL MOVEMENT FUNCTIONS
// **/

// // TODO: write_sprite
// // player: range from 0 to 3 inclusive
// // position: range from 0 to 128
// // the main purpose of this function is to make sure that the sprite is
// // remembered if parts of it are outside of memory. However, this may not be
// // necessary because I just remembered that the top and bottom 16 pixels are off screen.
// // if boundsCorrect is true, then 'sprite remembering' becomes redundant.

// // moves current player sprite from its current position in player_vert_position[] to a new position
// // TODO: use VDELAY every other line
// void write_sprite(unsigned char playerID, unsigned char position) {
//     // first, determine if the sprite would be rendered out of bounds
//     // determine the center of the sprite, subtract by 4
//     // Also, this method was rewritten to use memset and memcpy with the help of Google Gemini
//     unsigned char old_y = player_vert_positions[playerID];

//     if (old_y > SCREEN_TOP_BOUND) {
//         memset(&player_graphics[playerID][old_y-16],0,32);
//     }

//     // zero out the old sprite
    

    
//    if (position >= SCREEN_TOP_BOUND || position <= SCREEN_BOTTOM_BOUND) {
//         memcpy(&player_graphics[playerID][position - 16],player_sprites[playerID],32);

//    } 
// }
// void set_player_vert_position(unsigned char playerID, unsigned char pos, bool boundsCorrect) {
//     unsigned char correctedPos = pos;
    
//     if (boundsCorrect) {
        
//         correctedPos = clamp_char(pos,SCREEN_TOP_BOUND,SCREEN_BOTTOM_BOUND);
//     }
//     // the idea: copy the bytes from range(position - 4, position + 4) and set to pos.   
//     write_sprite(playerID,correctedPos);
    
//     player_vert_positions[playerID] = correctedPos;

// }

// void move_player_vert_position(unsigned char playerID,char delta, bool boundsCorrect) {
//     unsigned char curPosition = get_player_vert_position(playerID);
//     unsigned char finalPosition = (unsigned char)((char)curPosition + delta);


//     set_player_vert_position(playerID,finalPosition,boundsCorrect);
// }

// unsigned char get_player_vert_position(unsigned char playerID) {
//     return player_vert_positions[playerID % 4];
// }

// // START COLLISION HELPERS
// unsigned char player_to_player_collisions(unsigned char player) {
//     switch (player) {
//         case 0:
//             return GTIA_READ.p0pl;
            
//         case 1:
//             return GTIA_READ.p1pl;
            
//         case 2:
//             return GTIA_READ.p2pl;
            
//         case 3:
//             return GTIA_READ.p3pl;
            
//         default:
//             return 0;
            
//     }
// }

// unsigned char missile_to_player_collisions(unsigned char missile) {
//     switch (missile) {
//         case 0:
//             return GTIA_READ.m0pl;

//         case 1:
//             return GTIA_READ.m1pl;
            
//         case 2:
//             return GTIA_READ.m2pl;
            
//         case 3:
//             return GTIA_READ.m3pl;
//        default:
//             return 0;
            
//     }

// }

// unsigned char player_to_playfield_collisions(unsigned char player) {
//     switch (player) {
//         case 0:
//             return GTIA_READ.p0pf;

//         case 1:
//             return GTIA_READ.p1pf;
            
//         case 2:
//             return GTIA_READ.p2pf;
            
//         case 3:
//             return GTIA_READ.p3pf;
//        default:
//             return 0;
            
//     }
// }

// unsigned char missile_to_playfield_collisions(unsigned char missile) {
//     switch (missile) {
//         case 0:
//             return GTIA_READ.m0pf;

//         case 1:
//             return GTIA_READ.m1pf;
            
//         case 2:
//             return GTIA_READ.m2pf;
            
//         case 3:
//             return GTIA_READ.m3pf;
//        default:
//             return 0;
            
//     }
// }



// // How this works is you pass 'data' from one of the helper methods (e.g,
// // player_to_playfield_collisions(0)), and an index from 0-3
// // to check which object is being collided with.
// // usage example:
// // unsigned char p0_pf_collisions = player_to_playfield_collisions(unsigned char player);
// // // say 0b00001000 is returned
// // bool collide_with_3 = collision_with_index(p0_pf_collisions, 3);
// // // result: collide_with_3 is true if player 0 is colliding with playfield 3
// bool collision_with_index(unsigned char data, unsigned char index){
//     if (index > 3) {
//         return false;
//     }
//     return (data >> index) & 1;
// }


// // END COLLISION HELPERS