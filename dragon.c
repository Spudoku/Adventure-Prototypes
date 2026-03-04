#include "util.h"
#include "util_structs.h"
#include "entity.h"
#include "dragon.h"
#include "items.h"
#include "player.h"


int test;
int dummy;

Vector2 testLocation;

// think of this as a 'static' library of sprites that any given dragon 
// can read from
// TODO: move this somewhere else?
Sprite dragonSprites[DRAGON_SPRITE_COUNT] = {
  // normal dragon
  {
    0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000110,
        0b00001111,
        0b11110011,
        0b11111110,
        0b00001110,
        0b00000100,
        0b00000100,
        0b00011110,
        0b00111111,
        0b01111111,
        0b11100011,
        0b11000011,
        0b11000011,
        0b11000111,
        0b11111111,
        0b00111100,
        0b00001000,
        0b10001111,
        0b11100001,
        0b00111111,
        


        
    }, 
    // eating dragon
    {
      0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10000000,
        0b01000110,
        0b00101111,
        0b00011011,
        0b00011110,
        0b00101110,
        0b01000100,
        0b10000100,
        0b00011110,
        0b00111111,
        0b01111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b00111100,
        0b00001000,
        0b10001111,
        0b11100001,
        0b00111111,
        


    }, 
    // dead dragon
    {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00001100,
        0b00001100,
        0b00001100,
        0b00001110,
        0b00011011,
        0b01111111,
        0b11001110,
        0b10000000,
        0b11111100,
        0b11111110,
        0b11111110,
        0b01111110,
        0b01111000,
        0b00100000,
        0b01101110,
        0b01000010,
        0b01111110,
        0b00000000, // bottom of the sprite
        0b00000000,
      

        
    }, 
};

// //per frame behavior
STATUS dragonRoutine(Entity* thisEntity) {
  

  // read input and set velocity
  return dragonBehaviorProcess(thisEntity);
}

//remember, the renderer happens as the second batch, after all game logic is
//calced
//prepare the graphics driver
STATUS dragonRenderer(Entity* thisEntity) {
  return UNDEFINED;
}

// the behavior of the dragon
STATUS dragonBehaviorProcess(Entity* thisEntity) {
  
  DragonEntity* dEntity = (DragonEntity*)(thisEntity->superEntity);
  // MOVEMENT
  TrackMoveDelayFrames(dEntity);
  test = dEntity->sightRange;
  dEntity->dragonChompCounter++;

  if (dEntity->dragonChompCounter >= DRAGON_CHOMP_DELAY) {
    DragonChomp(dEntity);
  }

  if (dEntity->stunFrames > 0) {
    dEntity->stunFrames--;
  }

  if (CollidingWithPlayer()) {
    dEntity->stunFrames = 100;
  }

  if (dEntity->moveDelayCounter >= dEntity->moveFrameDelay && dEntity->stunFrames == 0) {
    dEntity->targetLocation = chooseTargetLocation(dEntity);
    testLocation.x = dEntity->targetLocation.x;
    testLocation.y = dEntity->targetLocation.y;
    
    dummy = thisEntity->eyeCoords.x;
    if (Vector2Dist(&(dEntity->targetLocation),&(thisEntity->eyeCoords))
    <= dEntity->sightRange) {
        moveTowards(dEntity,&(dEntity->targetLocation));
    }
    
  }
  

  return PASS;
}

// //init the dragon specific vars

STATUS dragonConstructor(Entity* subEntity,DragonEntity* theSuperEntity){
  // thisEntity.myEntity.superEntity = thisEntity;
  subEntity->superEntity = theSuperEntity;
  theSuperEntity->mySpeed = DEFAULT_DRAGON_SPEED;
  theSuperEntity->sightRange = DEFAULT_DRAGON_SIGHTRANGE;
  theSuperEntity->targetLocation = chooseTargetLocation(theSuperEntity);

  theSuperEntity->moveFrameDelay = DEFAULT_DRAGON_MOVE_DELAY;
  theSuperEntity->moveDelayCounter = 0;

  subEntity->pmg_id = DRAGON_GRAPHICS_PLAYER;

  theSuperEntity->stunFrames = 0;
  
  SetSpriteByIndex(theSuperEntity,0);

  return PASS;
}

// returns a target location
Vector2 chooseTargetLocation(DragonEntity* thisEntity) {
  
  struct Vector2 newLocation;
  // unsigned int dist;
  // currently relies on the 'loves' Entity
  newLocation.x = thisEntity->loves->eyeCoords.x;
  newLocation.y = thisEntity->loves->eyeCoords.y;



  return newLocation;
}

// moves a given dragon entity towards an x/y coordinate
void moveTowards(DragonEntity* thisEntity, Vector2* location) {
  Entity* transformEntity = &(thisEntity->myEntity);
  
  if (location->y > transformEntity->eyeCoords.y) {
    // move down
    transformEntity->eyeCoords.y += thisEntity->mySpeed;
  } else if (location->y < transformEntity->eyeCoords.y) {
    transformEntity->eyeCoords.y -= thisEntity->mySpeed;
  }

  if (location->x > transformEntity->eyeCoords.x) {
    // move left
    transformEntity->eyeCoords.x += thisEntity->mySpeed;
  } else if (location->x < transformEntity->eyeCoords.x) {

    transformEntity->eyeCoords.x -= thisEntity->mySpeed;
  }

}

// increment moveDelay frames
void TrackMoveDelayFrames(DragonEntity* thisEntity) {
  thisEntity->moveDelayCounter++;
  if (thisEntity->moveDelayCounter > thisEntity->moveFrameDelay) {
    thisEntity->moveDelayCounter = 0;
  }
}

void SetSpriteByIndex(DragonEntity* thisEntity, unsigned char id) {
  thisEntity->activeSprite = id;
  // TODO: render the sprite;
  player_sprites[DRAGON_GRAPHICS_PLAYER] = &dragonSprites[(unsigned int)(thisEntity->activeSprite)];
}

void DragonChomp(DragonEntity* thisEntity) {
  thisEntity->dragonChompCounter = 0;
  if (thisEntity->activeSprite == 0) {
    SetSpriteByIndex(thisEntity,1);
  } else if (thisEntity->activeSprite == 1) {
SetSpriteByIndex(thisEntity,0);
  } else {
    // do nothing, because it's probably dead
  }
}

bool CollidingWithPlayer() {
  return collision_with_index(player_to_player_collisions(DRAGON_GRAPHICS_PLAYER),PLAYER_GRAPHICS_PLAYER);
}