#ifndef GAME_MAP_H
#define GAME_MAP_H

#define MAP_LENGTH_BYTES 60
#define MAP_HEIGHT_BYTES 48


extern unsigned char gameMap[MAP_HEIGHT_BYTES][MAP_LENGTH_BYTES];

//precalced multiplication map for game map height interval vals
extern int multmap_gameMapheight_raw[13];

//set to point to the 0th element, 
//works for -6 >= x >= 6
//example, mult by -5: mult_gameMapHeight[-5]
extern int* mult_gameMapHeight;
#endif