#ifndef SPRITE_H_
#define SPRITE_H_

#include <stdint.h>

typedef struct PMSprite{
  uint8_t height;
  uint8_t cachedY;
  uint8_t color;
  uint8_t* bitmap;
} PMSprite;

#endif