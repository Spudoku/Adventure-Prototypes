#include "items.h"

Entity nullItem;

STATUS nullItem_Task(Entity* thisEntity) {
  return UNDEFINED;
}

STATUS nullItem_renderer(Entity* thisEntity) {
  return UNDEFINED;
}
//TODO: should the initialization and allocation be seperated? i.e. constructor
//being comprised of allocation and initing, rather than just initing
STATUS nullItem_constructor(Entity* thisEntity) {
  return entityConstructor(&nullItem, nullItem_Task, nullItem_renderer);
};
