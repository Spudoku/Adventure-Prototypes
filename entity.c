#include "entity.h"

STATUS entityConstructor(Entity* thisEntity, 
                      STATUS (*frameTask)(Entity* thisEntity), 
                      STATUS (*renderer)(Entity* thisEntity))
{
  thisEntity->_worldCoords.x = 0;
  thisEntity->_worldCoords.y = 0;
  thisEntity->entityData = NULL;
  thisEntity->childEntity = NULL;
  thisEntity->frameTask = frameTask;
  thisEntity->renderer = renderer;
  
  //TODO: set collider but figure out how to make it optional


  return PASS;
};

  void default_OnCollision(struct Entity* thisEntity, 
    struct Entity* otherEntity){
      
    return;
  };