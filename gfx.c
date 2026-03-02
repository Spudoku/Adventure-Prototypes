#include "gfx.h"

void map_absoluteMove(Vector2 absolutePosition){
    //unsigned int j = y;
    unsigned int i;

    
    //go to the location, scan down in inc of three

    //start at the first graphics instr
    //the comparison is to compare memory address

    //set fine, calculate the char map pos (division by 16)
    Vector2 coarsePosition = absolutePosition;
    coarsePosition.x >>= 4;
    coarsePosition.y >>= 4;


    //TODO: transition this later to iterate the ptr directly
    for(i = 3; DisplayList[i] != DL_JVB && i < sizeof(DisplayList); i+= 3){
        //work with a window of 3 bytes, convert i+1 to an int ptr
        //foregoing the syntatic sugar here


        //treat the window like a single int
        *(unsigned int *)(DisplayList + (i+1)) =
         (unsigned int)(gameMap[absolutePosition.y]) + absolutePosition.x;


        absolutePosition.y += 1;  //offset for next row
    } 
}

//this may be slower than just setting!
void map_relativeMove(Vector2 relativePosition){
    int i = 0;
    switch(relativePosition.y) {
        case -1:
            relativePosition.y = -40;
        case 0:
            break;
        case 1:
            relativePosition.y = 40;
            break;
        default:
            relativePosition.y *= 40;
            break;
    }

    for(i = 3; DisplayList[i] != DL_JVB && i < sizeof(DisplayList); i+= 3){
        //work with a window of 3 bytes, convert i+1 to an int ptr
        //foregoing the syntatic sugar here


        
        *(unsigned int *)(DisplayList + (i+1)) += relativePosition.y + relativePosition.x;
    } 
}