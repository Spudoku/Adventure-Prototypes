#ifndef UTIL_S_H
#define UTIL_S_H



#include <stdint.h>

typedef struct s32Vector2 {
  int16_t x;
  int16_t y;
} Vector2;

//Useful for certain values that shouldn't be huge (i.e. velocity)
//or even screen coords
typedef struct u16Vector2{
  uint8_t x;
  uint8_t y;
} u16Vector2;


//used for global map coords
typedef struct u32Vector2 {
  uint16_t x;
  uint16_t y;
} u32Vector2;



//eventually this should be a full bitmask flag and converted to being based on a uint8_t
typedef enum STATUS {
  FAIL = -1,
  PASS = 0,
  UNDEFINED = 1,
} STATUS;


//allows getting most/least sig byte a bit easier
typedef union int16_2int8{
  int16_t integer;
  int8_t bytes[2];
} IntToTwoChar; 


#endif 
