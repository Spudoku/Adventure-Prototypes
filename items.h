#ifndef ITEMS_H_
#define ITEMS_H_

#include "util_structs.h"
#include "entity.h"
#include <stddef.h>

//item repo
//items are entities designed to be held by another entity to do some action
//to keep things concise, the declarations for all items will be here, but the
//defintion will be in their respective [obj].c file

//null item funcs
Entity nullItem;

STATUS nullItem_Task(Entity* thisEntity) { return UNDEFINED; };
STATUS nullItem_renderer(Entity* thisEntity) { return UNDEFINED; };


//TODO: should the initialization and allocation be seperated? i.e. constructor
//being comprised of allocation and initing, rather than just initing
STATUS nullItem_constructor(Entity* thisEntity) {
  return entityConstructor(&nullItem, nullItem_Task, nullItem_renderer);
};


#endif //ITEMS_H_