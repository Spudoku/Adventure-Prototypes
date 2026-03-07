#include "gfx.h"


MapData mapData;


Vector2 offsetHopper;




//WARNING: any use of these move methods MUST be as close as possible to vblank.
//h/vscrol getting adjusted during the frame could cause graphical issues


//reset map gfx offset to position in whole pixels
//NOTE: in antic mode 2, 1 pixel is 1 hscroll unit, and 2 vscroll units
//WARNING: THIS FUNCTION TAKES UP 50% OF TOTAL FRAME TIME
void map_absoluteMove(Vector2 absolutePosition){
    unsigned int i;
    unsigned int j = 0;


    SET_VEC2_ANTIC_SCROLL(absolutePosition)

    

    //calc the coarse offset. this is highly dependent on the antic mode
    mapData.coarseOffset.y = Y_PIXEL_TO_COARSE(absolutePosition.y);
    //offset needs to accomodate two lines per pixel
    mapData.coarseOffset.x = X_PIXEL_TO_COARSE(absolutePosition.x);
    

    //set fine
    mapData.offset = absolutePosition;


    //TODO: init display list that just takes this
    for(i = 3; i < (sizeof(DisplayList) - 3); i+= 3){
        //work with a window of 3 bytes, convert i +1 to an int ptr
        //foregoing the syntatic sugar here


        //treat the window like a single int
        *(unsigned int *)(DisplayList + (i+1)) =
         (unsigned int)(gameMap[mapData.coarseOffset.y + j]) + mapData.coarseOffset.x;


        j += 1;  //offset for next row
    } 

    
    

    

    return;
}

unsigned int startAddress;
int firstLineOffset = 0;
Vector2 newCoarse;
Vector2 newOffset;

//move by delta pixels, takes up ~4% of the frame time on gradual movements
//TODO: further optimization
void map_relativeMove(Vector2 relativePosition){


    //expand out to new absolute position, put into antic ASAP
    newOffset = mapData.offset;
    ADD_ASSIGN_VEC2(newOffset, relativePosition)
    map_fastAbsoluteMove(newOffset);

    
}

//TODO: map_fastAbsoluteMove that is mostly just relative move but without the
//delta

void map_fastAbsoluteMove(Vector2 absolutePosition){
    char dirty = 0;
    char i = 0;


    newOffset = absolutePosition;
    SET_VEC2_ANTIC_SCROLL(newOffset)

    firstLineOffset = 0;


    //TODO: mitigate the cost of a hit (a move covered by just moving h/vscrol)
    //if(mapData.coarseOffset !=)   //idea that didnt work
    // if((((newOffset.y & ~mapData.offset.y) //changes to coarse Y exist?
    //         & IGNORE_VSCROL_PIXELS_BITMASK) != 0))  
    
    //calculate new coarse positions, determine if they are changed
    newCoarse.y = Y_PIXEL_TO_COARSE(newOffset.y);
    if(newCoarse.y  != mapData.coarseOffset.y)
    {
        //calc new memory start in o(1) if possible using mult table
        if(abs(newCoarse.y - mapData.coarseOffset.y) > 6){
            //they're moving more than half the screen, straight to jail
            map_absoluteMove(newOffset);
            return;
        }

    
        firstLineOffset += 
            mult_gameMapHeight[newCoarse.y - mapData.coarseOffset.y];

        mapData.coarseOffset.y = newCoarse.y;
        dirty = 1;
    } 


    newCoarse.x = X_PIXEL_TO_COARSE(newOffset.x);
    if(newCoarse.x != mapData.coarseOffset.x)  
    {

        //offset by coarse x delta
        firstLineOffset += newCoarse.x - mapData.coarseOffset.x;
        mapData.coarseOffset.x = newCoarse.x;
        dirty = 1;
    }
    

    //no more work needed, update and leave
    if(dirty != 1) {
        mapData.offset = newOffset;
        return;
    }


    //more work required
    //the second element is conveniently the location of the first scrMem byte
    startAddress = ((unsigned int *)DisplayList)[2];
    startAddress += firstLineOffset;


    for(i = 3; i < (sizeof(DisplayList) - 3); i+= 3){
        //work with a window of 3 bytes, convert i +1 to an int ptr
        //foregoing the syntatic sugar here


        //treat the window like a single int
        *(unsigned int *)(DisplayList + (i+1)) = startAddress;
        startAddress += MAP_LENGTH_BYTES;
    } 




    mapData.offset = newOffset;
    return;
}