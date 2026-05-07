#include "gfx.h"

#pragma optimize(on)
#pragma static-locals(on)


MapData mapData;


Vector2 offsetHopper;

//align the main instance to the nearest 1kb
#pragma bss-name (push, "PMGAREA")
PMGInstance pmgMainInstance;
#pragma bss-name (pop)

const unsigned int rowOffsetTable[] = {0, 120, 240, 360, 480, 600, 720, 840, 960, 1080, 1200, 1320, 1440, 1560, 1680, 1800, 1920, 2040, 2160, 2280, 2400, 2520, 2640, 2760, 2880, 3000, 3120, 3240, 3360, 3480, 3600, 3720, 3840, 3960, 4080, 4200, 4320, 4440, 4560, 4680, 4800, 4920, 5040, 5160, 5280, 5400, 5520, 5640, 5760, 5880, 6000, 6120, 6240, 6360, 6480, 6600, 6720, 6840, 6960, 7080, 7200, 7320, 7440, 7560, 7680, 7800, 7920, 8040, 8160, 8280, 8400, 8520, 8640, 8760, 8880, 9000, 9120, 9240, 9360, 9480, 9600, 9720, 9840, 9960, 10080, 10200, 10320, 10440, 10560, 10680, 10800, 10920, 11040, 11160, 11280, 11400, 11520, 11640, 11760, 11880, 12000, 12120, 12240, 12360, 12480, 12600, 12720, 12840, 12960, 13080, 13200, 13320, 13440, 13560, 13680, 13800, 13920, 14040, 14160, 14280, 14400, 14520, 14640, 14760, 14880, 15000, 15120, 15240, 15360, 15480, 15600, 15720, 15840, 15960, 16080, 16200, 16320, 16440, 16560, 16680, 16800, 16920, 17040, 17160, };



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
        // this is expensive because of pointers (I think?)
        *(unsigned int *)(DisplayList + (i+1)) =
         (unsigned int)(gameMap[mapData.coarseOffset.y + j]) + mapData.coarseOffset.x;


        ++j;  //offset for next row
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


void map_fastAbsoluteMove(Vector2 absolutePosition){
    int16_t temp; // intermediate variable
    unsigned char dirty = 0;
    char i = 0;


    newOffset = absolutePosition;
    SET_VEC2_ANTIC_SCROLL(newOffset)

    firstLineOffset = 0;


    //TODO: mitigate the cost of a hit (a move covered by just moving h/vscrol)
    
    //calculate new coarse positions, determine if they are changed
    newCoarse.y = Y_PIXEL_TO_COARSE(newOffset.y);
    
    if(newCoarse.y  != mapData.coarseOffset.y)
    {
        temp = newCoarse.y - mapData.coarseOffset.y;
        //calc new memory start in o(1) if possible using mult table
        if(abs(temp) > 6){
            //they're moving more than half the screen, straight to jail
            map_absoluteMove(newOffset);
            return;
        }

    
        firstLineOffset += mult_gameMapHeight[temp];

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


void gfx_Init() {
    dl_Init();
    charset_Init();
    pmg_Init(&pmgMainInstance);


    //TEMP: force set playfield colors
    OS.color4 = GTIA_COLOR_GRAY3;   //BG
    OS.color0 = GTIA_COLOR_LIGHTGREEN;
    OS.color1 = GTIA_COLOR_YELLOW;
    OS.color2 = GTIA_COLOR_BLUE;
    OS.color3 = GTIA_COLOR_BLACK;

}

// TODO: reduce array accesses??????
unsigned char getTileAt(int16_t theX, int16_t theY,unsigned char boxSize) {
    unsigned char test;
    unsigned int debugX;
    unsigned int debugY;
    

    debugX = theX >> 3;
    debugY = theY >> 3;

    test = gameMap[debugY][debugX];
    if (test) {
        return test;
    }

    debugX = (theX + boxSize) >> 3;
    test = gameMap[debugY][debugX];

    if (test) {
        return test;
    }

    debugY = (theY + boxSize) >> 3;
    test = gameMap[debugY][debugX];

    if (test) {
        return test;
    }

    debugX = theX >> 3;
    test = gameMap[debugY][debugX];

    return test;




    // unsigned char tile;
    // int16_t int xL = theX >> 3;
    // int16_t int xR = (theX + boxSize) >> 3;
    // int16_t int yT = theY >> 3;
    // int16_t int yB = (theY + boxSize) >> 3;

    // if (xL != xR) {

    // }



    // return tile;

}
