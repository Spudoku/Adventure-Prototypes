#include "pmg.h"

PMGInstance* activePMGInstance; 

void pmg_init(PMGInstance* pmgInstance){
  IntToTwoChar convert;
  activePMGInstance = pmgInstance;
  pmg_clear();

    
  convert.integer = (unsigned int)activePMGInstance;
  ANTIC.pmbase = convert.bytes[1];


  ANTIC.dmactl = 46;
    OS.sdmctl = 46;
    GTIA_WRITE.gractl = 3; // enable PMG
    OS.gprior = 1; // set player priorty

  //OS.pcolr0 = COLOR_YELLOW; //hardcode this for now
}

void pmg_clear(){
  memset(activePMGInstance, 0, sizeof(PMGInstance));
}

// void pmg_updateActiveInstance(){
//   int i = 0;
//   if(activePMGInstance == NULL) return;

//   for(; i < 4; i++){
//     if(activePMGInstance->playerGFX[i].header.refsprite == NULL) continue;


//   }
// }

//assume clear
uint8_t pmg_addPlayerSprite(Sprite* toAdd){
  int i = 0;

  while(i < 4){
    if(activePMGInstance->playerGFX[i].header.refsprite == NULL){
      activePMGInstance->playerGFX[i].header.refsprite = toAdd;
      (&(OS.pcolr0))[i] = toAdd->color;
      return i;
    }
    i++;
  }
  return 255;
}

void pmgSilo_clear(PMGPlayerSpriteSilo* silo){
  int8_t oldY;
  int8_t height;


  oldY = silo->header.cachedY;
  height = silo->header.refsprite->height;

  //clamping oldy
  //assumes no overflow
  if((oldY < 0) && (height + oldY > 0)) {
    //partial visibility on top
    memset(silo->visibleBytes, 0, (height + oldY));
  } else if ((oldY < sizeof(silo->visibleBytes)) && (height + oldY) < sizeof(silo->visibleBytes)) {
    //complely visible, 0 < y < 96, and the height is the same
    memset((silo->visibleBytes + oldY), 0, height);
  } else {
    //partial bottom occlusion
    memset((silo->visibleBytes + oldY), 0, sizeof(silo->visibleBytes));
  }

}

//TODO: optimize
void pmgSilo_writeRefSprite(PMGPlayerSpriteSilo* silo, int8_t newY){
  int8_t height;
  Sprite* retrievedSprite;
  retrievedSprite = silo->header.refsprite;
  if(retrievedSprite == NULL) return; //could be a clear flag
  height = retrievedSprite->height;

  if((newY < 0) && (height + newY > 0)) {
    //partial visibility on top
    //printf("ok");
    memcpy(silo->visibleBytes, retrievedSprite->bitmap - newY, (height + newY));
  } else if ((newY < sizeof(silo->visibleBytes)) && (height + newY) < sizeof(silo->visibleBytes)) {
    //complely visible, 0 < y < 96, and the height is the same
    //printf("lol");
    memcpy((silo->visibleBytes + newY), retrievedSprite->bitmap, height);
  } else {
    //partial bottom occlusion
    //printf("%d\n", newY -newY - sizeof(silo->visibleBytes))
    memcpy((silo->visibleBytes + newY), retrievedSprite->bitmap, sizeof(silo->visibleBytes) - newY);
  }

  silo->header.cachedY = newY;
}

void pmgSilo_setY(PMGPlayerSpriteSilo* silo, int8_t newY){
  
  //if(silo->header.cachedY == newY) return;
  memset(silo->visibleBytes, 0, sizeof(silo->visibleBytes));

  pmgSilo_writeRefSprite(silo, newY);
  //memcpy the new y with bounds checking
  
}


//TODO: optimize. i dont like it but it gets the job done for now
//algorithm could be simplified

// void pmgSilo_absoluteVerticalMove(PMGPlayerSpriteSilo* silo, int8_t newY){
//   int8_t beginErasePos, oldY;
//   int8_t delta;
//   int8_t height;
//   oldY = silo->header.cachedY;
//   if(oldY == newY) return;

//   height = silo->header.refsprite->height;
//   delta = newY - oldY;

//   beginErasePos = oldY;

//   if(newY < oldY) {
//     //make the vector from the new top to old top  into old top to new bottom
//     beginErasePos += height - delta;
//   }
  
//   //bounds processing
//   if(beginErasePos < 0) {
//     delta -= beginErasePos;
//     beginErasePos = 0;
//   }

//   if(beginErasePos + height >= sizeof(silo->visibleBytes)){
//     delta = beginErasePos - sizeof(silo->visibleBytes);
//   }


//   //erase bytes

//   for(;delta >= 0; delta--){
//     silo->visibleBytes[beginErasePos + delta] = 0;
//   }


//   //copy new bytes

// }