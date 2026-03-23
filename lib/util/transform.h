#ifndef TRANSFORM_H_
#define TRANSFORM_H_ 

#include "util_structs.h"

//Transform data struct to streamline positioning and enable selective screen
//mem culling in the future

typedef struct Transform {

  //coords are the upper left corner of the entity/sprite
  Vector2 eyeCoords;  //0 > x > 255 TODO: compatibility with tilebased entity?
  Vector2 worldCoords;


  //these may be important later for collision stuff?
  //local point of the origin (upper left). if everything ends up being (0,0) we can refactor later
  //to do stuff
  Vector2 objectAnchorPoint; //TODO: may convert it to a u16vect, since its local coords

  //local point of the size (lower right)
  Vector2 objectBounds; //think dimensions, may be u16 vect since its local coords
} Transform;

//name the member in the struct used as "transform" to use this macro
//allow code to be a bit cleaner, 
#define _worldCoords transform.worldCoords
#define _eyeCoords transform.eyeCoords
#define _objectAnchorPoint transform.objectAnchorPoint
#define _objectBounds transform.objectBounds
//todo: move child entity to TF like unity?


#endif