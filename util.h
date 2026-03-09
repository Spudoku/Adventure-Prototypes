#ifndef UTIL_H_
#define UTIL_H_

#include <stddef.h>
#include <atari.h>
#include <peekpoke.h>


//fixed screen offsets 
#define HPOSP_MIN 47
#define V_MIN 24

/**
    UTILITY FUNCTIONS 
**/

unsigned int clamp_int(unsigned int value,unsigned int min,unsigned int max);

void memZero(unsigned int start, unsigned int offset);

unsigned char clamp_char(unsigned char value,unsigned char min,unsigned char max);

unsigned int max(unsigned int a, unsigned int b);
unsigned int min(unsigned int a, unsigned int b);
<<<<<<< HEAD





=======
>>>>>>> 40d6363eb74a5ac503cff75762d8fa049e0e1db1


#endif