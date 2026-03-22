#ifndef SPRITE_H_
#define SPRITE_H_

#include <stdint.h>
#include <atari.h>

typedef struct SingleColor_Sprite{
  uint8_t height; //height in bytes
  uint8_t color;  //base color of the sprite in GTIA color
  uint8_t* bitmap;
} Sprite;


//sprite instance example

// Sprite foo = {sizeof(fooSpriteRawBytes), GTIA_COLOR_CYAN, fooSpriteRawBytes};
// uint8_t fooSpriteRawBytes[] = {
//   0b11111111,
//   0b11111111,
//   0b11111111,
//   0b11111111
// };


#endif