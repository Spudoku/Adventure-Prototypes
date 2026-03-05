#include "gfx.h"


MapData mapData;

//debug intermediary value holders to make memory inspection easier
Vector2 coarseViewer;
Vector2 offsetViewer;
Vector2 fineViewer;

Vector2 offsetHopper;


//WARNING: any use of these move methods MUST be as close as possible to vblank.
//


//reset map gfx offset to position in whole pixels
//NOTE: in antic mode 2, 1 pixel is 1 hscroll unit, and 2 vscroll units
//WARNING: THIS FUNCTION TAKES UP 50% OF TOTAL FRAME TIME
void map_absoluteMove(Vector2 absolutePosition){
    //unsigned int j = y;
    unsigned int i;

    unsigned int j = 0;
    //1 color cycle per pixel
    //ANTIC.hscrol = -absolutePosition.x;

    //two scanlines per pixel
    //NOTE: possible overflow into negative bit when y > 2^14.
    //ANTIC.vscrol = absolutePosition.y << 1;  



    // (*(u16Vector2 *)(&(ANTIC.hscrol))).x = -absolutePosition.x;
    // (*(u16Vector2 *)(&(ANTIC.hscrol))).y = absolutePosition.y << 1;

    SET_VEC2_ANTIC_SCROLL(absolutePosition)

    coarseViewer = mapData.coarseOffset;
    offsetViewer = mapData.offset;
    
    //go to the location, scan down in inc of three

    //start at the first graphics instr
    //the comparison is to compare memory address

    //calc the coarse offset. this is highly dependent on the antic mode


    mapData.coarseOffset.y = Y_PIXEL_TO_COARSE(absolutePosition.y);
    //offset needs to accomodate two lines per pixel
    mapData.coarseOffset.x = X_PIXEL_TO_COARSE(absolutePosition.x);
    
    coarseViewer = mapData.coarseOffset;
    offsetViewer = mapData.offset;


    //set fine
    mapData.offset = absolutePosition;

    coarseViewer = mapData.coarseOffset;
    offsetViewer = mapData.offset;


    //TODO: init display list that just takes this
    for(i = 3; i < (sizeof(DisplayList) - 3); i+= 3){
        //work with a window of 3 bytes, convert i +1 to an int ptr
        //foregoing the syntatic sugar here


        //treat the window like a single int
        *(unsigned int *)(DisplayList + (i+1)) =
         (unsigned int)(gameMap[mapData.coarseOffset.y + j]) + mapData.coarseOffset.x;


        j += 1;  //offset for next row
    } 

    //antic will auto truncate a value above 15.. but also go right to go left
    

    

    return;
}

int foo;
unsigned int startAddress;
int firstLineOffset = 0;
Vector2 newCoarse;
Vector2 newOffset;

//move by delta pixels
void map_relativeMove(Vector2 relativePosition){

    
    
 
    char dirty = 0;
    char i = 0;
    //expand out to new absolute position
    newOffset = mapData.offset;
    ADD_ASSIGN_VEC2(newOffset, relativePosition)
    //assign it ASAP to hscrol

    SET_VEC2_ANTIC_SCROLL(newOffset)
    // ANTIC.hscrol = -newOffset.x;
    // ANTIC.vscrol = abs(newOffset.y << 1);
    
    //t
    
    firstLineOffset = 0;

    //calculate the y


    //get start address 


    //precompute the y offset


    
    //calculate new coarse positions, determine if they are changed

    //checking if the coarse bits havent CHANGED, 
    //not the value (to calc later as needed)
    // if((((newOffset.x & ~mapData.offset.x) & IGNORE_HSCROL_PIXELS_BITMASK) != 0) 
    //     &&
    //   (((newOffset.y & ~mapData.offset.y) & IGNORE_VSCROL_PIXELS_BITMASK) != 0) ) 
    // {    
    //     mapData.offset = newOffset;
    //     return;
    // }
        
    //the coarse bits have changed, time to do the hard work

    //calculate the offset per-line interval,

    //if(mapData.coarseOffset !=)
    // if((((newOffset.y & ~mapData.offset.y) 
    //         & IGNORE_VSCROL_PIXELS_BITMASK) != 0))  //changes to coarse Y exist

    newCoarse.y = Y_PIXEL_TO_COARSE(newOffset.y);
    if(newCoarse.y  != mapData.coarseOffset.y)
    {
        //calc new memory start in o(1) if possible
        if(abs(newCoarse.y - mapData.coarseOffset.y) > 6){
            //they're moving more than half the screen, straight to jail
            map_absoluteMove(newOffset);
            return;
        }


        firstLineOffset += mult_gameMapHeight[newCoarse.y - mapData.coarseOffset.y];
        mapData.coarseOffset.y = newCoarse.y;

        
        // if(relativePosition.y > 0){
        //     firstLineOffset += 
        //         mult_gameMapHeight[Y_PIXEL_TO_COARSE(relativePosition.y) + 1];
        // } else{
        //     firstLineOffset += 
        //         mult_gameMapHeight[Y_PIXEL_TO_COARSE(relativePosition.y) - ];
        // }
        
        //lineInterval = MAP_LENGTH_BYTES;

        dirty = 1;
    } 

    //optimizing for faster hits here
    // if(((((newOffset.x - 1) & ~(mapData.offset.x - 1)) 
    //         & IGNORE_HSCROL_PIXELS_BITMASK) != 0))  //changes to coarse X exist


    //compiler should convert this to an and

    // if((mapData.offset.x % HSCROL_PIXEL_RANGE) )

    // if(mapData.offset.x - newOffset.x)    


    newCoarse.x = X_PIXEL_TO_COARSE(newOffset.x);

    if(newCoarse.x  != mapData.coarseOffset.x)  //changes to coarse X exist
    {
        //coarse offset delta x is guaranteed to have a magnitude of atleast 
        //1 set of two chars
        //get the total number of sets, multiply later to get x delta offset


        // firstLineOffset += (
        //     (int)(relativePosition.x & IGNORE_HSCROL_PIXELS_BITMASK) >> 
        //             COARSE_SCROLL_IGNORABLE_BITS);


        firstLineOffset += newCoarse.x - mapData.coarseOffset.x;

        // if(relativePosition.x > 0){
        //     firstLineOffset++;
        // } else{
        //     firstLineOffset--;
        // }

        mapData.coarseOffset.x = newCoarse.x;

        dirty = 1;
    }
    

    //no more work needed
    if(dirty != 1) {
        mapData.offset = newOffset;
        return;
    }


    //more work required
    startAddress = ((unsigned int *)DisplayList)[2];
    startAddress += firstLineOffset;


    //reuse dirty
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