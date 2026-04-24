#include "items.h"
#pragma optimize(on)
#pragma static-locals(on)

Entity nullItem = {nullItem_Task, nullItem_renderer, default_OnCollision, (void *)NULL, &nullItem, {{0,0}, {0,0}, {0,0},{0,0}}};

STATUS nullItem_Task(Entity* thisEntity) { return UNDEFINED; };
STATUS nullItem_renderer(Entity* thisEntity) { return UNDEFINED; };

//TODO: should the initialization and allocation be seperated? i.e. constructor
//being comprised of allocation and initing, rather than just initing
STATUS nullItem_constructor(Entity* thisEntity) {
  return entityConstructor(&nullItem, nullItem_Task, nullItem_renderer);
};

uint8_t TEMP_item_anticIndex;

/*
  End nullItem definitions
*/

/*
  Start Chalice Definitions
*/

int16_t chalice_eyecoords_x;
int16_t chalice_eyecoords_y;

int16_t chalice_worldcoords_x;
int16_t chalice_worldcoords_y;


ChaliceEntity chaliceEnt = {
  {
    chalice_Task,chalice_renderer,chalice_OnCollision,(void*)&chaliceEnt,NULL, // functions, reference, child
    {{0,0},{0,0},{0,0},{4,4}}                     // transform
  },

  {0,0}, // destination
  0,  // distance to desintation
  
  0   // chalice silo
};

STATUS chalice_constructor() {
  uint8_t pmg_index;

  //call the "base" constructor

  // set PMG index and validate it
  pmg_index = pmg_addPlayerSprite(&chaliceSprite);


  if(pmg_index < 4){
    chaliceEnt.chaliceSilo = activePMGInstance->playerGFX + pmg_index;
    TEMP_item_anticIndex = pmg_index;
  }
  chaliceEnt.chaliceSilo -> header.refsprite = &chaliceSprite;

  return PASS;
}

STATUS chalice_Task(Entity* thisEntity) {

  
  return UNDEFINED;
}

STATUS chalice_renderer(Entity* thisEntity) {
  // sound_generic_buzz();
   thisEntity->_eyeCoords = convertToEyeCoords(thisEntity->_worldCoords);

  chalice_eyecoords_x = thisEntity->_eyeCoords.x;
  chalice_eyecoords_y = thisEntity->_eyeCoords.y;

  chalice_worldcoords_x = thisEntity->_worldCoords.x;
  chalice_worldcoords_y = thisEntity->_worldCoords.y;

    if(!objectVisible(&(thisEntity->transform))){
    (&(GTIA_WRITE.hposp0))[TEMP_item_anticIndex] = 0;

    return PASS;
  }


    (&(GTIA_WRITE.hposp0))[TEMP_item_anticIndex] = chaliceEnt.chaliceEntity._eyeCoords.x 
            + HPOSP_MIN + chaliceEnt.chaliceEntity._objectAnchorPoint.x;

    pmgSilo_setY(chaliceEnt.chaliceSilo, thisEntity->_eyeCoords.y);

  return UNDEFINED;
}

void chalice_OnCollision(struct Entity* thisEntity, struct Entity* otherEntity) {
  
}

void chalice_check_desintation() {

      Vector2 tempVector2;
    unsigned int chalice_distance;
   // checking if chalice touches altar asd
    tempVector2 = chaliceEnt.chaliceEntity._worldCoords;
    SUB_ASSIGN_VEC2(tempVector2,chaliceEnt.chalice_destination);
    chalice_distance = abs(tempVector2.x) + abs(tempVector2.y);
    if (chalice_distance < chaliceEnt.chalice_minDistToDest) {
        end_game_good();
        // printf("good ending!");
    }
    GTIA_WRITE.hitclr = 1; 
}



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