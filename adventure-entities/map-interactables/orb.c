#include "orb.h"
#pragma optimize(on)
#pragma static-locals(on)

#define ROW_LENGTH 21

Orb orb_singleton = {
  {NULL, NULL, orb_OnCollision, (void *)&orb_singleton, NULL, //entity
    {{0,0}, {0,0}, {0,0},{0,0}}}, //entity.transform
  ORB_DELAY
};


// first line of message
// see charmap.c for the list of character codes
// which is NOT standard ASCII
// each line is a separate message!
unsigned char messageLine1[] = {
  21,9,12,13,0,10,33,0,11,9,20,13,10,0,27,16,23,26,28,0,0, // "made by caleb short"
  27,23,21,13,28,17,21,13,46,0,17,0,12,26,13,9,21,0,0,0,0, // "sometimes, I dream"
  17,22,27,24,17,26,13,12,0,10,33,0,31,9,26,26,13,22,0,0,0, // "inspired by warren"
  16,9,30,13,0,14,29,22,0,27,28,23,26,21,17,22,15,0,28,16,13, // "have fun storming the"
  28,16,13,0,14,9,11,13,0,17,56,21,0,21,9,19,17,22,15,0,0, // "the face I'm making"
  10,9,27,13,12,47,0,10,9,27,13,12,0,23,22,0,31,16,9,28,47, // "based? based on what?"
  10,26,23,45,45,45,0,31,13,56,26,13,0,27,23,0,10,9,11,19,0, // "bro... we're so back"
  10,33,0,26,23,33,9,20,0,27,24,29,12,0,9,22,12,0,0,0,0, // "by royal spud and"
  31,16,13,22,0,21,9,33,0,17,0,15,23,47,0,0,0,0,0,0,0, // "when may I go?"
  10,13,28,28,13,26,0,28,16,9,22,0,10,9,11,23,22,48,0,0,0, // "better than bacon!"
  33,23,29,0,11,9,21,13,0,9,20,20,0,28,16,13,0,31,9,33,0, //  "you came all the way"
  31,9,17,28,46,0,9,0,27,16,26,17,21,24,0,14,26,17,13,12,0, // "wait, a shrimp fried"
  33,23,29,0,9,26,13,0,27,23,0,27,21,9,20,20,48,0,17,27,0, // "you are so small! is"
  49,29,22,17,22,28,13,20,20,17,15,17,10,20,13,50,0,0,0,0,0, // "(unintelligible)"
  15,23,0,28,23,29,11,16,0,15,26,9,27,27,48,0,0,0,0,0,0, // "go touch grass!"
  31,9,17,28,48,0,17,0,11,9,22,0,27,9,33,0,35,39,0,0,0, // "wait! I can say 15"
};

// the second line of messages
unsigned char messageLine2[] = {
  9,22,12,0,18,29,22,13,0,27,11,16,23,10,13,26,0,0,0,0,0, // ... "and june schober"
  9,10,23,29,28,0,11,16,13,13,27,13,45,45,45,0,0,0,0,0,0, // ... "about cheese..."
  26,23,10,17,22,13,28,28,0,0,0,0,0,0,0,0,0,0,0,0,0, // ... "robinett"
  11,9,27,28,20,13,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // ... "castle!"
  17,28,56,27,0,45,45,45,0,45,45,45,0,58,37,0,0,0,0,0,0, // ... "it's ... ... :3"
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // (space intentionally left blank)
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // (space intentionally left blank)
  21,17,27,11,16,13,30,17,23,29,27,0,10,13,13,0,0,0,0,0,0, // ... "mischevious bee"
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // (space intentionally left blank)
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // (space intentionally left blank)
  16,13,26,13,0,14,23,26,0,28,16,17,27,47,0,0,0,0,0,0,0, // "here for this?"
  28,16,17,27,0,26,17,11,13,47,0,0,0,0,0,0,0,0,0,0,0, // "this rice?"
  14,29,22,22,33,0,28,23,0,21,13,48,0,0,0,0,0,0,0,0,0, // "funny to me!"
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // (space intentionally left blank)
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // (space intentionally left blank)
  23,28,16,13,26,0,28,16,17,22,15,27,48,0,0,0,0,0,0,0,0, // "other things!"
};

void orb_OnCollision(Entity* thisEntity, Entity* otherEntity){
  unsigned char entry;
  // print a message every 10 seconds or so
  if(--orb_singleton.counter < 0){
    orb_singleton.counter = ORB_DELAY;

    entry = get_random_value(16);
    orb_sound();
    // open a silly door for easter egg 2
    memset(&gameMap[TEXT_OFFSET_1] - 240,0,2);


    memcpy(&gameMap[TEXT_OFFSET_1],messageLine1 + (entry * ROW_LENGTH), ROW_LENGTH);
    memcpy(&gameMap[TEXT_OFFSET_2],messageLine2 + (entry * ROW_LENGTH), ROW_LENGTH);
    
    
  }
  
}