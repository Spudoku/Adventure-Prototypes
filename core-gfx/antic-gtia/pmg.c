#include "pmg.h"

PMGInstance* activePMGInstance; 

void pmg_Init(PMGInstance* pmgInstance){
  IntToTwoChar convert;
  activePMGInstance = pmgInstance;
  pmg_clear();

    
  convert.integer = (unsigned int)activePMGInstance;

  printf("PMbase highbyte: %d\n", convert.bytes[1]);
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



//assume clear
uint8_t pmg_addPlayerSprite(Sprite* toAdd){
  int i = 0;

  while(i < 4){
    // check if current sprite in the slot is NULL
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

  // height is
  // clamping oldy
  // assumes no overflow
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
// However, I've spent like 6 hours trying to do better than this with 
// no success
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
    //printf("%d\n", newY -neswY - sizeof(silo->visibleBytes))
    memcpy((silo->visibleBytes + newY), retrievedSprite->bitmap, sizeof(silo->visibleBytes) - newY);
  }

  silo->header.cachedY = newY;
}

void pmgSilo_setY(PMGPlayerSpriteSilo* silo, int8_t newY){
  
  //if(silo->header.cachedY == newY) return;
  // why does this clear all 96 bytes
  // memset(silo->visibleBytes, 0, sizeof(silo->visibleBytes));
  pmgSilo_clear(silo);

  pmgSilo_writeRefSprite(silo, newY);
  //memcpy the new y with bounds checking
  
}


// START COLLISION HELPERS
unsigned char player_to_player_collisions(unsigned char player) {
    switch (player) {
        case 0:
            return GTIA_READ.p0pl;
            
        case 1:
            return GTIA_READ.p1pl;
            
        case 2:
            return GTIA_READ.p2pl;
            
        case 3:
            return GTIA_READ.p3pl;
            
        default:
            return 0;
            
    }
}

unsigned char missile_to_player_collisions(unsigned char missile) {
    switch (missile) {
        case 0:
            return GTIA_READ.m0pl;

        case 1:
            return GTIA_READ.m1pl;
            
        case 2:
            return GTIA_READ.m2pl;
            
        case 3:
            return GTIA_READ.m3pl;
       default:
            return 0;
            
    }

}

unsigned char player_to_playfield_collisions(unsigned char player) {
    switch (player) {
        case 0:
            return GTIA_READ.p0pf;

        case 1:
            return GTIA_READ.p1pf;
            
        case 2:
            return GTIA_READ.p2pf;
            
        case 3:
            return GTIA_READ.p3pf;
       default:
            return 0;
            
    }
}

unsigned char missile_to_playfield_collisions(unsigned char missile) {
    switch (missile) {
        case 0:
            return GTIA_READ.m0pf;

        case 1:
            return GTIA_READ.m1pf;
            
        case 2:
            return GTIA_READ.m2pf;
            
        case 3:
            return GTIA_READ.m3pf;
       default:
            return 0;
            
    }
}

// How this works is you pass 'data' from one of the helper methods (e.g,
// player_to_playfield_collisions(0)), and an index from 0-3
// to check which object is being collided with.
// usage example:
// unsigned char p0_pf_collisions = player_to_playfield_collisions(unsigned char player);
// // say 0b00001000 is returned
// bool collide_with_3 = collision_with_index(p0_pf_collisions, 3);
// // result: collide_with_3 is true if player 0 is colliding with playfield 3
bool collision_with_index(unsigned char data, unsigned char index){
    if (index > 3) {
        return false;
    }
    return (data >> index) & 1;
}