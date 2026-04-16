#include "items.h"


Entity nullItem = {nullItem_Task, nullItem_renderer, default_OnCollision, (void *)NULL, &nullItem, {{0,0}, {0,0}, {0,0},{0,0}}};

STATUS nullItem_Task(Entity* thisEntity) { return UNDEFINED; };
STATUS nullItem_renderer(Entity* thisEntity) { return UNDEFINED; };

//TODO: should the initialization and allocation be seperated? i.e. constructor
//being comprised of allocation and initing, rather than just initing
STATUS nullItem_constructor(Entity* thisEntity) {
  return entityConstructor(&nullItem, nullItem_Task, nullItem_renderer);
};

uint8_t TEMP_chalice_anticIndex;

/*
  End nullItem definitions
*/

/*
  Start Chalice Definitions
*/

STATUS chalice_constructor(Entity* thisEntity) {

  // uint8_t pmg_index;

  // //call the "base" constructor

  // // set PMG index and validate it
  // pmg_index = pmg_addPlayerSprite(&chaliceSprite);


  // if(pmg_index < 4){
  //   chaliceSilo = activePMGInstance->playerGFX + pmg_index;
  //   TEMP_chalice_anticIndex = pmg_index;
  // }

  // //printf("Address: %d\n", %d)
  return PASS;
};

// task that chalice does each frame
STATUS chalice_Task(Entity* thisEntity) { 
  
  return UNDEFINED;
  
}

// STATUS chalice_renderer(Entity* thisEntity) {
  
//     thisEntity->_eyeCoords = convertToEyeCoords(thisEntity->_worldCoords);
  

//   //quick and dirty hide
//   if(!objectVisible(&(thisEntity->transform))){
//     (&(GTIA_WRITE.hposp0))[TEMP_chalice_anticIndex] = 0;
//     return PASS;
//   }

//   // set horizontal position
//   (&(GTIA_WRITE.hposp0))[TEMP_chalice_anticIndex] = thisEntity->_eyeCoords.x 
//             + HPOSP_MIN + thisEntity->_objectAnchorPoint.x;
  
  


//   //printf("%d\n",(&(GTIA_WRITE.hposp0))[TEMP_player_anticIndex] );

//   // only update vertical position if vertical movement occurred
//   // pmgSilo_setY(&chaliceSilo, thisEntity->_eyeCoords.y);
  
//   return UNDEFINED;
// }

STATUS chalice_renderer(Entity* thisEntity) {
  return UNDEFINED;
}

void chalice_OnCollision(struct Entity* thisEntity, struct Entity* otherEntity) {

}

Entity chaliceItem = {chalice_Task, chalice_renderer, chalice_OnCollision, (void *)NULL, &chaliceItem, {{0,0}, {0,0}, {0,0},{0,0}}};


// sprite :D
uint8_t chaliceSpriteBitmap[] ={
  0b10000001,
  0b10000001,
  0b11000011,
  0b01111110,
  0b01111110,
  0b00111100,
  0b00011000,
  0b00011000,
  0b01111110,
};

Sprite chaliceSprite = {sizeof(chaliceSpriteBitmap),GTIA_COLOR_RED,chaliceSpriteBitmap};

/*
  End Chalice Definitions
*/