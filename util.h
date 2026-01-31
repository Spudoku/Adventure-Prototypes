#ifndef UTIL_H
#define UTIL_H
unsigned int clamp_int(unsigned int value,unsigned int min,unsigned int max);


unsigned int clamp_int(unsigned int value,unsigned int min,unsigned int max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}
#endif