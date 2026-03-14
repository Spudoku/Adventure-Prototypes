////
//  Entity
////

#ifndef ENTITY_H_
#define ENTITY_H_ 


//keep in mind, these may require NULL checks

#include "util_structs.h"
#include "transform.h"
#include <stddef.h>



//entity base
typedef struct Entity {
  STATUS (*frameTask)(struct Entity* thisEntity);
  STATUS (*renderer)(struct Entity* thisEntity);
  //TODO: should this pass a self ref?
  void (*OnCollision)(struct Entity* thisEntity, struct Entity* otherEntity);  //optional

  //TODO: make this a one way ref instead?
  void *entityData; //cast into sub class as appropriate
  struct Entity* childEntity;
  Transform transform;
} Entity;



//constructs an entity that is allocated either automatically or by malloc
//TODO: deconstructor?
//WARNING: YOU WANT TO ASSIGN FUNCS HERE NULL OR VALID. good luck
STATUS entityConstructor(Entity* thisEntity, 
                      STATUS (*frameTask)(Entity* thisEntity), 
                      STATUS (*renderer)(Entity* thisEntity));


void default_OnCollision(struct Entity* thisEntity, struct Entity* otherEntity);

#endif



