#ifndef GAME_MAP_H
#define GAME_MAP_H

#define MAP_LENGTH_BYTES 120
#define MAP_HEIGHT_BYTES 144

#define TEXT_OFFSET_1 7298
#define TEXT_OFFSET_2 7418

#define GATE_OFFSET 14508
#define GATE_SIZE 4

// extern unsigned char gameMap[MAP_HEIGHT_BYTES][MAP_LENGTH_BYTES];
extern unsigned char gameMap[MAP_HEIGHT_BYTES * MAP_LENGTH_BYTES];

//precalced multiplication map for game map height interval vals
extern int multmap_gameMapheight_raw[13];

//set to point to the 0th element, 
//works for -6 >= x >= 6
//example, mult by -5: mult_gameMapHeight[-5]
extern int* mult_gameMapHeight;
#endif