#include "entity.h"

STATUS entityConstructor(Entity* thisEntity, 
                      STATUS (*frameTask)(Entity* thisEntity), 
                      STATUS (*renderer)(Entity* thisEntity))
{
  thisEntity->_worldCoords.x = 0;
  thisEntity->_worldCoords.y = 0;
  thisEntity->superEntity = NULL;
  thisEntity->childEntity = NULL;
  thisEntity->frameTask = frameTask;
  thisEntity->renderer = renderer;
  return PASS;
};