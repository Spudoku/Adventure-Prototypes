#include "gfx.h"


MapData mapData;

//debug intermediary value holders to make memory inspection easier
Vector2 coarseViewer;
Vector2 offsetViewer;
Vector2 fineViewer;


//reset map gfx offset to position in whole pixels
//NOTE: in antic mode 2, 1 pixel is 1 hscroll unit, and 2 vscroll units
void map_absoluteMove(Vector2 absolutePosition){
    //unsigned int j = y;
    unsigned int i;

    coarseViewer = mapData.coarseOffset;
    offsetViewer = mapData.offset;
    
    //go to the location, scan down in inc of three

    //start at the first graphics instr
    //the comparison is to compare memory address

    //calc the coarse offset. this is highly dependent on the antic mode


    mapData.coarseOffset.y = absolutePosition.y >> 3;
    //offset needs to accomodate two lines per pixel
    mapData.coarseOffset.x = (((absolutePosition.x - 1) >> 4) << 1);
    
    coarseViewer = mapData.coarseOffset;
    offsetViewer = mapData.offset;


    //set fine
    mapData.offset = absolutePosition;

    coarseViewer = mapData.coarseOffset;
    offsetViewer = mapData.offset;


    //TODO: init display list that just takes this
    for(i = 3; DisplayList[i] != DL_JVB && i < sizeof(DisplayList); i+= 3){
        //work with a window of 3 bytes, convert i +1 to an int ptr
        //foregoing the syntatic sugar here


        //treat the window like a single int
        *(unsigned int *)(DisplayList + (i+1)) =
         (unsigned int)(gameMap[mapData.coarseOffset.y]) + mapData.coarseOffset.x;


        mapData.coarseOffset.y += 1;  //offset for next row
    } 

    //antic will auto truncate a value above 15.. but also go right to go left
    
    //1 color cycle per pixel
    ANTIC.hscrol = -absolutePosition.x;

    //two scanlines per pixel
    //NOTE: possible overflow into negative bit when y > 2^14.
    ANTIC.vscrol = absolutePosition.y << 1;  

    return;
}


//move by delta pixels
void map_relativeMove(Vector2 relativePosition){

    //expand out to new absolute position
    Vector2 newOffset;
    
    newOffset = mapData.offset;

    
    ADD_ASSIGN_VEC2(newOffset, relativePosition)

    
    //calculate new coarse positions, determine if they are changed

    //just checking if the bits have CHANGED, not the value (to calc later)
    if((((newOffset.x & ~mapData.offset.x) & HSCROL_PR_INT_BITMASK_INV) != 0) ||
      (((newOffset.y & ~mapData.offset.y) & VSCROL_PR_INT_BITMASK_INV) != 0) ) {
        
        //if they have, just send them to the abs handler
        map_absoluteMove(newOffset);
        return;
    }

    //set antic if only fine motion has changed
    mapData.offset = newOffset;
    ANTIC.hscrol = -newOffset.x;
    ANTIC.vscrol = newOffset.y << 1;
    return;
}