#ifndef TRANSFORM_H_
#define TRANSFORM_H_ 

#include "util_structs.h"

//Transform data struct to streamline positioning and enable selective screen
//mem culling in the future

typedef struct Transform {
  Vector2 eyeCoords;  //0 > x > 255 TODO: compatibility with tilebased entity?
  Vector2 worldSpace;
  Vector2 objectAnchorPoint; //TODO: may convert it to a char Vect
  //Vector2 objectBounds;
} Transform;


#endif