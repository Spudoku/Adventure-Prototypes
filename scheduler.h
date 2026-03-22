#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "entity.h"
#include <stdint.h>
#include <atari.h>

#define MAX_ENTITIES 8
#define MAX_PLAYERS 2
//scheduler backend to support EDC

//macro to make the antic p2p collide dispatch readable
#define DISPATCH_AP2PCOLLIDE(i, n)                                \
  schedulerData.antic_P2PCollisionLookupTable[i]->OnCollision(    \
      schedulerData.antic_P2PCollisionLookupTable[i],             \
      schedulerData.antic_P2PCollisionLookupTable[n])             \



//FCFS scheduler with some process segregation
typedef struct QD_SchedulerData{
  
  Entity* activeEntityList[MAX_ENTITIES];



  //collision dispatching support:
  //these should run BEFORE gamestate calculation, so that its calculating the
  //current frame actions and not the previous frame actions
  //in practice the difference is just semantics, but this assumption allows
  //programming without needing to think "oh but what if i get a collision in my
  //entity after i process my gamestate!?"


  //part 1: antic/gtia specific support
    //section a: player-player

  //ordered p0-p3
  //implicates that it exists in the active entity list
  Entity* antic_P2PCollisionLookupTable[4];
  //set dynamically when entity lookup changes
  //i.e. if you have p0 and p2 occupied it would be set to 0b00000101
  uint8_t antic_P2PCollisionLookupMask; 

} SchedulerData;


void scheduler_DispatchPlayerCollisions();

extern SchedulerData schedulerData;

#endif