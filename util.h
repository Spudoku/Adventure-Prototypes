#ifndef UTIL_H
#define UTIL_H

/**
    UTILITY FUNCTIONS 
**/

unsigned int clamp_int(unsigned int value,unsigned int min,unsigned int max);
void memZero(unsigned int start, unsigned int offset);

// clamps an int between min and max
unsigned int clamp_int(unsigned int value,unsigned int min,unsigned int max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}

// clamps a single bytes value (a character)
unsigned char clamp_char(unsigned char value,unsigned char min,unsigned char max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}

// zeroes out a section of memory
void memZero(unsigned int start, unsigned int offset) {
    int x = 0;
    for (x = 0; x < offset; x++) {
        POKE(start + x,0);
    }
}
#endif