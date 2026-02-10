////
//  Entity
////

#ifndef ENTITY_H
#define ENTITY_H


//keep in mind, these may require NULL checks

#include "util_structs.h"
#include <stddef.h>

//Entity interface
// typedef struct Entitye {
//   STATUS (*frameTask)(Entity* ); 
// };


//entity base
typedef struct Entity {
  STATUS (*frameTask)(struct Entity* thisEntity);
  STATUS (*renderer)(struct Entity* thisEntity);
  struct Entity* childEntity;
  Vector2 eyeCoords;
} Entity;

//allows accessing to be funny

//constructs an entity that is allocated either automatically or by malloc
//TODO: deconstructor?
//WARNING: YOU WANT TO ASSIGN FUNCS HERE NULL OR VALID. good luck
// STATUS entityConstructor(Entity* thisEntity, 
//                       STATUS (*frameTask)(Entity* thisEntity), 
//                       STATUS (*renderer)(Entity* thisEntity));


STATUS entityConstructor(Entity* thisEntity, 
                      STATUS (*frameTask)(Entity* thisEntity), 
                      STATUS (*renderer)(Entity* thisEntity))
{
  //*(thisEntity)->childEntity = NULL;
  thisEntity->childEntity = NULL;
  thisEntity->frameTask = frameTask;
  thisEntity->renderer = renderer;
  return PASS;
};




#endif



