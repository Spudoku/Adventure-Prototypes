#include "items.h"
#include "player.h"
#include "sound.h"

/*
  Begin nullItem definitions
*/
unsigned int buzz_test_interval = 60;
unsigned int buzz_test_counter = 0;
Entity nullItem = {nullItem_Task, nullItem_renderer, (void *)NULL, &nullItem, {{0,0}, {0,0}, {0,0},{0,0}}};

STATUS nullItem_Task(Entity* thisEntity) { return UNDEFINED; };
STATUS nullItem_renderer(Entity* thisEntity) { return UNDEFINED; };

//TODO: should the initialization and allocation be seperated? i.e. constructor
//being comprised of allocation and initing, rather than just initing
STATUS nullItem_constructor(Entity* thisEntity) {
  return entityConstructor(&nullItem, nullItem_Task, nullItem_renderer);
};

/*
  End nullItem definitions
*/

/* Begin dumbItem definitions
*/
Entity dumbItem = {dumbItem_Task, dumbItem_renderer, (void *)NULL, (void *)NULL, {{0,0}, {0,0}, {0,0},{0,0}}};

// play a silly sound every 2 seconds
STATUS dumbItem_Task(Entity* thisEntity) {
  if (buzz_test_counter-- == 0) {
    sound_generic_buzz();
    buzz_test_counter = buzz_test_interval;
  }
  
  return PASS;
  }

STATUS dumbItem_renderer(Entity* thisEntity) {
  return UNDEFINED;
}

STATUS dumbItem_constructor(Entity* thisEntity) {
  return entityConstructor(&dumbItem, dumbItem_Task, dumbItem_renderer);
}