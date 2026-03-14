#include "scheduler.h"



SchedulerData schedulerData;

//todo: setup funcs


/*
* idea here is to eliminate checks to reduce the number of calls. a self-collide
* can be ignored, even if that *somehow* triggers. 
* new worst case: 16 calls/16 checks -> 12 calls/12 checks
* new best case: 0 calls/16 checks -> 0 calls/12 checks
*
* since a collide implies the other party also collides, this means that we do
* not need to check that player bit again.
* this means that the last player wont need to be checked
* new worst case: 12 calls/12 checks -> 12 calls/ 6 checks
* new best case: 0 calls/12 checks -> 0 calls/ 6 checks
*
* the bit mask and MAX_PLAYER checks allow skipping antic players that are not
* entities.
*/

void scheduler_DispatchPlayerCollisions(){
  uint8_t i = 0;
  uint8_t dynMask = 0b00001100;



  

  //0 case specific bits
  switch(GTIA_READ.p0pf & schedulerData.antic_P2PCollisionLookupMask){ //assumes that bit 0 is always 0

      case 0b00000000:  //most likely bit to be triggered
        break;
      case 0b00001010:
        DISPATCH_AP2PCOLLIDE(0, 3);
        DISPATCH_AP2PCOLLIDE(3, 0);
        //fall
      case 0b00000010:
        //exec 0 col hndlr with 1
        //exec 1 col hndlr with 0
        DISPATCH_AP2PCOLLIDE(0, 1);
        DISPATCH_AP2PCOLLIDE(1, 0);
        break;
      case 0b00000110:
        //exec 0 col handlr with 1
        //exec 1 col hndlr with 0
        DISPATCH_AP2PCOLLIDE(0, 1);
        DISPATCH_AP2PCOLLIDE(1, 0);
        //fall
      case 0b00000100:
        //exec 0 col handlr with 2
        //exec 2 col hndlr with 0
        DISPATCH_AP2PCOLLIDE(0, 2);
        DISPATCH_AP2PCOLLIDE(2, 0);
        break;

      case 0b00001110:
        //exec 0 col hndlr with 1
        //exec 1 col hndlr with 0
        DISPATCH_AP2PCOLLIDE(0, 1);
        DISPATCH_AP2PCOLLIDE(1, 0);
        //fall
      case 0b00001100:
        //exec 0 col hndlr with 2
        //exec 2 col hndlr with 0
        DISPATCH_AP2PCOLLIDE(0, 2);
        DISPATCH_AP2PCOLLIDE(2, 0);
        //fall
      case 0b00001000:
        //exec 0 col hndlr with 3
        //exec 3 col hndlr with 0
        DISPATCH_AP2PCOLLIDE(0, 3);
        DISPATCH_AP2PCOLLIDE(3, 0);
        //break
        break;
      
      default:
        break;//???
  }


  for(i = 1; i < MAX_PLAYERS; i++){

    //four possible situations
    switch((&(GTIA_READ.p0pl))[i] & dynMask & schedulerData.antic_P2PCollisionLookupMask){
      case 0b00000000:
        break;  //do nothing
      case 0b00000100:
        //exec i col hndlr with 2
        //exec 2 col hndlr with i
        DISPATCH_AP2PCOLLIDE(i, 2);
        DISPATCH_AP2PCOLLIDE(2, i);
        break;
      case 0b00001100:
        //exec i col hndlr with 2
        //exec 2 col hndlr with i
        DISPATCH_AP2PCOLLIDE(i, 2);
        DISPATCH_AP2PCOLLIDE(2, i);
        //fall
      case 0b00001000:
        //exec i col hndlr with 3
        //exec 3 col hndlr with i
        DISPATCH_AP2PCOLLIDE(i, 3);
        DISPATCH_AP2PCOLLIDE(3, i);
        break;
      
      default:  
        break;  //weird match (>15)
    }

      


  }
}