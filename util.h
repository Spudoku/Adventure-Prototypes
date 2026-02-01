#ifndef UTIL_H
#define UTIL_H
unsigned int clamp_int(unsigned int value,unsigned int min,unsigned int max);
void memZero(unsigned int start, unsigned int offset);

unsigned int clamp_int(unsigned int value,unsigned int min,unsigned int max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}

void memZero(unsigned int start, unsigned int offset) {
    int x = 0;
    for (x = 0; x < offset; x++) {
        POKE(start + x,0);
    }
}
#endif