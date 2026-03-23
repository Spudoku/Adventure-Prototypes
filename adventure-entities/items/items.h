#ifndef ITEMS_H_
#define ITEMS_H_

#include "core-support/entity.h"
#include "../../core/sound.h"
#include <stddef.h>

//item repo
//items are entities designed to be held by another entity to do some action
//to keep things concise, the declarations for all items will be here, but the
//defintion will be in their respective [obj].c file




/*
  Begin nullItem declarations
*/
extern Entity nullItem;

STATUS nullItem_Task(Entity* thisEntity);
STATUS nullItem_renderer(Entity* thisEntity);


//TODO: should the initialization and allocation be seperated? i.e. constructor
//being comprised of allocation and initing, rather than just initing
STATUS nullItem_constructor(Entity* thisEntity);

/*
  End nullItem declarations
*/

/*
  Begin dumbItem declarations
  dumbItem will play a sound when it touches things (I think?)
*/
extern Entity dumbItem;

STATUS dumbItem_Task(Entity* thisEntity);

STATUS dumbItem_renderer(Entity* thisEntity);

STATUS dumbItem_constructor(Entity* thisEntity);
/*
  End dumbItem declarations
*/

#endif 