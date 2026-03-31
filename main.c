#include <stdio.h>
#include <unistd.h>

#include "core/engine.h"


//temp debug utils
void debug_autoMove(Transform *toMove);  //force an oscillating move


//diagnostic vars
Vector2 dir = {1, 1};
Vector2 zeroVec = {0, 0};
//char theFiller[17000];    //test for worst case map fit

int main() {
    //redirect stdout to altirra printer
    //for some reason this makes the top line bug out when x is negative
    //A PRINT IS EXPENSIVE 
    freopen("P1:", "w", stdout);
    printf("hi!\n");

    
    engine_Boot();


    while (true) {

        
        //process gamestate
    
        engine_StateUpdate();

   
        
        
        waitvsync();
        engine_Render();


        engine_EventDispatcher();

        
    }
}





//WARNING: Use only on one object at a time!
void debug_autoMove(Transform *toMove){
    dir.x = ((toMove->worldCoords.x > 312) 
         || (toMove->worldCoords.x < 8)) ? -dir.x : dir.x;
    toMove->worldCoords.x += dir.x;
}

