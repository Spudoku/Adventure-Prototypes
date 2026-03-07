// extern; references charset
#ifndef CHARMAP_H
#define CHARMAP_H

#include "util_structs.h"
#include <atari.h>

extern unsigned char charset[1024];

void init_charset();

#endif