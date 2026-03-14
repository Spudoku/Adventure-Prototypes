#ifndef SPRITE_H_
#define SPRITE_H_

#include <stdint.h>

typedef struct PMGSprite{
  uint8_t height;
  uint8_t cachedY;
  uint8_t color;
  uint8_t* spriteSilo;  //the corresponding entry in a pmg instance
  uint8_t* bitmap;
} PMGSprite;

#endif