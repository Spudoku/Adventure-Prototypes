#include "entity.h"

STATUS entityConstructor(Entity* thisEntity, 
                      STATUS (*frameTask)(Entity* thisEntity), 
                      STATUS (*renderer)(Entity* thisEntity))
{
  //*(thisEntity)->childEntity = NULL;
  thisEntity->entityData = NULL;
  thisEntity->childEntity = NULL;
  thisEntity->frameTask = frameTask;
  thisEntity->renderer = renderer;
  return PASS;
};