#ifndef UTIL_S_H
#define UTIL_S_H

typedef struct Vector2 {
  int x;
  int y;
} Vector2;

//Useful for certain values that shouldn't be huge (i.e. velocity)
//or even screen coords
typedef struct u8Vector2{
  unsigned char x;
  unsigned char y;
} u8Vector2;


typedef enum STATUS {
  FAIL = -1,
  PASS = 0,
  UNDEFINED = 1,
} STATUS;
#endif 
