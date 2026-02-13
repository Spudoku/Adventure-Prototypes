#ifndef SCREEN_MEMORY_H
#define SCREEN_MEMORY_H
#include "player_missile.h"  // for screen bounds
#include "util.h"
#include <string.h>

// Currently, using ANTIC mode 4
#define SCREEN_MEM_SIZE 960         // total number of bytes in screen memory
#define SCREEN_ROW_SIZE 40         // number of rows
#define SCREEN_COLUMN_SIZE 24      // number of columns
#define BUFFER_SIZE 128              // size of buffer array         

#pragma bss-name (push, "SCREENAREA")
unsigned char ScreenMemory[SCREEN_MEM_SIZE];

#pragma bss-name (pop)


/**
    BEGIN FUNCTION DECLARATIONS
**/

// methods for accessing and filling screen memory, mostly for testing
void get_row(unsigned int index); 
void get_column(unsigned int index);

void get_row_section(unsigned int rowNum, unsigned int start, unsigned int end);        // get section of indices within a row
void get_column_section(unsigned int columnNum, unsigned int start, unsigned int end);  // get section of indices within a column

void fill_row(unsigned int index, unsigned char character);
void fill_row_section(unsigned int rowNum, unsigned int start, unsigned int end, unsigned char character);

void fill_column(unsigned int index, unsigned char character);
void fill_column_section(unsigned int columnNum, unsigned int start, unsigned int end, unsigned char character);

// these ones put the "fun" in "function"
void fill_rect(unsigned int vert_pos,unsigned int horiz_pos,unsigned int l, unsigned int w, bool hollow);
void fill_circle(unsigned int vert_center,unsigned int horiz_center, unsigned int radius, bool hollow);

void clear_buffer(); // clears indexBuffer; should be called within get_row and get_column

// reads SCREEN_ROW_SIZE * SCREEN_COLUMN_SIZE bytes of information from start and puts them into screen memory
void manual_load(unsigned char* start); 
/**
    END FUNCTION DELCARATIONS
**/

/**
    BEGIN VARIABLE DECLARATIONS
**/
// variable declarations
unsigned char indexBuffer[BUFFER_SIZE]; // store indices from getRow or getColumn

/**
    END VARIABLE DECLARATIONS
**/

void clear_buffer() {
    memset(&indexBuffer[0],0,BUFFER_SIZE * sizeof(unsigned char));
}

/**
    BEGIN ROW OPERATIONS
**/

// clears indexBuffer, then populates it with the indices of a row
void get_row(unsigned int index) {
    unsigned int i;
    // make sure buffer is cleared
    // clear_buffer();


    for (i = 0; i < SCREEN_ROW_SIZE; i++) {
        indexBuffer[i] = (SCREEN_ROW_SIZE * (index % SCREEN_COLUMN_SIZE)) + i;
    }

}

void get_row_section(unsigned int rowNum, unsigned int start, unsigned int end) {
    unsigned int correctedStart = clamp_int(start,0,SCREEN_ROW_SIZE);
    unsigned int correctedEnd = clamp_int(end,0,SCREEN_ROW_SIZE);
    unsigned int i;
    unsigned int range = correctedEnd - correctedStart;
    // clear_buffer();
    for (i = 0; i < range; i++) {
        indexBuffer[i] = (SCREEN_ROW_SIZE * (rowNum % SCREEN_COLUMN_SIZE)) + start + i;
    }
}

// place characters into a row
void fill_row(unsigned int index, unsigned char character) {
    unsigned int i;
    // load indexBuffer with correct indices
    get_row(index);

    for (i = 0; i < SCREEN_ROW_SIZE; i++) {
        ScreenMemory[indexBuffer[i]] = character;
    }
}

void fill_row_section(unsigned int rowNum, unsigned int start, unsigned int end, unsigned char character) {
    unsigned int correctedStart = clamp_int(start,0,SCREEN_ROW_SIZE);
    unsigned int correctedEnd = clamp_int(end,0,SCREEN_ROW_SIZE);
    unsigned int range = correctedEnd - correctedStart;
    unsigned int i;
    get_row_section(rowNum, correctedStart, correctedEnd);

    for (i = 0; i < range; i++) {
        ScreenMemory[indexBuffer[i]] = character;
    }

}

/**
    END ROW OPERATIONS
**/


/**
    BEGIN COLUMN OPERATIONS
**/

// for column 1;
// 0 + 1
// 40 + 1
// 80 + 1
// ..
// TODO: Fix (currently incorrect indices)
    void get_column(unsigned int index) {
        unsigned int i;

        for (i = 0; i < SCREEN_COLUMN_SIZE; i++) {
            indexBuffer[i] = (SCREEN_ROW_SIZE * i) + index;
        }
    }

    void get_column_section(unsigned int columnNum, unsigned int start, unsigned int end) {
        unsigned int correctedStart = clamp_int(start,0,SCREEN_COLUMN_SIZE);
        unsigned int correctedEnd = clamp_int(end,0,SCREEN_COLUMN_SIZE);
        unsigned int i;
        unsigned int range = correctedEnd - correctedStart;
        // clear_buffer();
        for (i = 0; i < range; i++) {
            // indexBuffer[i] = ;
        }
    }

    void fill_column(unsigned int index, unsigned char character) {
        unsigned int i;
        get_column(index);
        for (i = 0; i < SCREEN_COLUMN_SIZE; i++) {
            ScreenMemory[indexBuffer[i]] = character;
        }
    }


    void fill_column_section(unsigned int columnNum, unsigned int start, unsigned int end, unsigned char character) {

    }
/**
    END COLUMN OPERATIONS
**/

void manual_load(unsigned char* start) {
    unsigned int i;
    unsigned int range = (unsigned int)SCREEN_ROW_SIZE * (unsigned int)SCREEN_COLUMN_SIZE;

    for (i = 0; i < range; i++) {
        unsigned char value = *(start + i);
        ScreenMemory[i] = value;
    }
}
#endif