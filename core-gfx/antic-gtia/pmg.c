#include "pmg.h"
#pragma optimize(on)
#pragma static-locals(on)
PMGInstance* activePMGInstance; 
static uint8_t* temp_visible_bytes;
static uint8_t* temp_bitmap_ptr;
static int8_t temp_height;
Sprite* temp_sprite;

void pmg_Init(PMGInstance* pmgInstance){
  IntToTwoChar convert;
  activePMGInstance = pmgInstance;
  pmg_clear();

    
  convert.integer = (unsigned int)activePMGInstance;

  ANTIC.pmbase = convert.bytes[1];


  ANTIC.dmactl = 46;
    OS.sdmctl = 46;
    GTIA_WRITE.gractl = 3; // enable PMG
    OS.gprior = 1; // set player priorty


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

void pmgSilo_clear(PMGPlayerSpriteSilo* silo, int8_t newY){
    int8_t oldY;
    // int8_t height;
    oldY = silo->header.cachedY;
    // object isnt moving, so return
    if (newY == oldY) {
        return;
    }
    // access visible bytes array
    temp_visible_bytes = silo->visibleBytes;  
    // accessed cached y and height
    

    temp_height = silo->header.refsprite->height;

    if (oldY < 0) {
        temp_height += oldY;
    } else {
        temp_visible_bytes += oldY;
        // in theory, storing sizeof(silo->visibleBytes) is redundant,
        // because its constant (96 bytes); however changing this breaks things
        // for some reason

        if ((temp_height + oldY) > sizeof(silo->visibleBytes)) {
            // height = 9, oldY = 90, size = 96
            temp_height = (temp_height + oldY) - 96 ;
            // height = 96;
        }
    }
    //  clear relevant bytes
    memset(temp_visible_bytes,0,temp_height);
    
}

//  TODO: optimize further
// copies the sprite from silo into PMG memory
void pmgSilo_writeRefSprite(PMGPlayerSpriteSilo* silo, int8_t newY){
    int8_t oldY;
    oldY = silo->header.cachedY;

    // object isnt moving, so return
    if (newY == oldY) {
        return;
    }

    temp_sprite = silo->header.refsprite;
    
    if(!temp_sprite) return; //could be a clear flag

    // global versions of the pointers
    temp_visible_bytes = silo->visibleBytes;  
    temp_bitmap_ptr = temp_sprite->bitmap; 

    temp_height = temp_sprite->height;

    if (newY < 0) {
        // occlusion at the top
        temp_bitmap_ptr -= newY; // this is addition
        temp_height += newY;
        
    } else {
        // top is visible
        temp_visible_bytes += newY;

        // clipping at bottom
        if ((temp_height + newY) > 96) {
            temp_height = 96 - (temp_height + newY);
        }
    }
    // printf("height: %d",height);

    if (temp_height <= 0) {
        return;
    }
    memcpy(temp_visible_bytes,temp_bitmap_ptr,temp_height);
    
    silo->header.cachedY = newY;
}

void pmgSilo_setY(PMGPlayerSpriteSilo* silo, int8_t newY){
  
  // why does this clear all 96 bytes
    pmgSilo_clear(silo, newY);

    //memcpy the new y with bounds checking
    pmgSilo_writeRefSprite(silo, newY);

  
}

// player rendering; player is ALWAYS onscreen, so no need for
// height checking
void pmgSilo_setY_player(PMGPlayerSpriteSilo* silo, int8_t newY) {
    pmgSilo_clear_player(silo, newY);

    pmgSilo_writeRefSprite_player(silo, newY);
}

// a more efficient version of pmgSilo_clear, which takes advantage of the fact
// that the player will always be onscreen (no occlusion is possible)
void pmgSilo_clear_player(PMGPlayerSpriteSilo* silo,int8_t newY) {
    int8_t oldY;
    // int8_t height;
    oldY = silo->header.cachedY;
    // object isnt moving, so return
    if (newY == oldY) {
        return;
    }
    // access visible bytes array
    temp_visible_bytes = silo->visibleBytes + oldY;  
    temp_height = silo->header.refsprite->height;

    //  clear relevant bytes
    memset(temp_visible_bytes,0,temp_height);
}

// a more efficient version of pmgSilo_writeRefSprite, which takes advantage of
// the fact that the player will always be onscreen (no occlusion is possible)
void pmgSilo_writeRefSprite_player(PMGPlayerSpriteSilo* silo, int8_t newY) {
    int8_t oldY;
    oldY = silo->header.cachedY;

    // object isnt moving, so return
    if (newY == oldY) {
        return;
    }

    temp_sprite = silo->header.refsprite;
    
    if(!temp_sprite) return; //could be a clear flag

    // global versions of the pointers
    temp_visible_bytes = silo->visibleBytes + newY;  
    temp_bitmap_ptr = temp_sprite->bitmap; 

    temp_height = temp_sprite->height;


    memcpy(temp_visible_bytes,temp_bitmap_ptr,temp_height);

    silo->header.cachedY = newY;
}


/*
    Start collision helpers
*/

// return all player-to-player collision data for player
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

// return all missile-to-player collision data for player
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
// to check which object is being collided with. usage example:
//      unsigned char p0_pf_collisions = player_to_playfield_collisions(unsigned char player);
//      say 0b00001000 is returned
//      bool collide_with_3 = collision_with_index(p0_pf_collisions, 3);
//      result: collide_with_3 is true if player 0 is colliding with playfield 3
bool collision_with_index(unsigned char data, unsigned char index){
    
    if (index > 3) {
        return false;
    }
    return (data >> index) & 1;
}

/*
    end collision helpers
*/