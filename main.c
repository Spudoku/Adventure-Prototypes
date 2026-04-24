#include <stdio.h>
#include <unistd.h>

#include "core/engine.h"
#pragma optimize(on)
#pragma static-locals(on)

void game_loop();

//temp debug utils
void debug_autoMove(Transform *toMove);  //force an oscillating move


//diagnostic vars
Vector2 dir = {1, 1};
Vector2 zeroVec = {0, 0};

int main() {
    //redirect stdout to altirra printer
    //for some reason this makes the top line bug out when x is negative
    //A PRINT IS EXPENSIVE; only use for debugging
    freopen("P1:", "w", stdout);

    // pass game_loop to RESET registers in case of warmstart
    // TODO: make it work for coldstart?
    *(unsigned int*)0x000A = (unsigned int)game_loop;

    
    game_loop();
}

// need to point to this for RESET
void game_loop() {
    engine_Boot();
    while (true) {

        
        //process gamestate
        engine_EventDispatcher();
        
        
    
        engine_StateUpdate();

        waitvsync();

        engine_Render();

        
        
    }
}


//WARNING: Use only on one object at a time!
void debug_autoMove(Transform *toMove){
    dir.x = ((toMove->worldCoords.x > 312) 
         || (toMove->worldCoords.x < 8)) ? -dir.x : dir.x;
    toMove->worldCoords.x += dir.x;
}

