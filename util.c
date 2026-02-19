#include "util.h"
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

unsigned int max(unsigned int a, unsigned int b) {
    if (a >= b) {
        return a;
    }
    return b;
}

unsigned int min(unsigned int a,unsigned int b) {
    if (a <= b) {
        return a;
    }
    return b;
}

int abs(int a) {
    if (a > 0) {
        return a;
    }

    if (a < 0) {
        return a * -1;
    }
}