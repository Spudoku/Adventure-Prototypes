#ifndef TRANSFORM_H_
#define TRANSFORM_H_ 

#include "util_structs.h"

//Transform data struct to streamline positioning and enable selective screen
//mem culling in the future

typedef struct Transform {
  Vector2 eyeCoords;  //0 > x > 255 TODO: compatibility with tilebased entity?
  u32Vector2 worldCoords;
  Vector2 objectAnchorPoint; //TODO: may convert it to a char Vect 
  Vector2 objectBounds; //think dimensions
} Transform;

//name the member in the struct used as "transform" to use this macro
//allow code to be a bit cleaner, 
#define _worldCoords transform.worldCoords
#define _eyeCoords transform.eyeCoords
#define _objectAnchorPoint transform.objectAnchorPoint'
#define _objectBounds transform.objectBounds
//todo: move child entity to TF like unity?


#endif