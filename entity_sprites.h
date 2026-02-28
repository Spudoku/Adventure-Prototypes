#ifndef ENTITY_SPRITES_H
#define ENTITY_SPRITES_H
#define SPRITE_BYTE_COUNT 32
#define MISSILE_SPRITE_BYTE_COUNT 8
typedef struct Sprite {
    unsigned char bytes[SPRITE_BYTE_COUNT];
} Sprite;

typedef struct Missile_Sprite {
    unsigned char bytes[MISSILE_SPRITE_BYTE_COUNT];
} Missile_Sprite;
#endif