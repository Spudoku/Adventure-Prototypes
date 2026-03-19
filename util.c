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

// based on: https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte

unsigned char reverse_char(unsigned char c) {
    c = (c & 0xF0) >> 4 | (c & 0x0F) << 4;
   c = (c & 0xCC) >> 2 | (c & 0x33) << 2;
   c = (c & 0xAA) >> 1 | (c & 0x55) << 1;
   return c;
}