#ifndef UTIL_INPUT_H_
#define UTIL_INPUT_H_

#include <atari.h>
#include "util_structs.h"
#include <joystick.h>

unsigned char joystickError;

unsigned char InitializeJoystick() {
  //fastcall (non ANSI C standard feature in cc65) requires an assignment, 
  //intellitype doesnt know how it works so it will freak out
  joystickError = joy_load_driver(atrstd_joy);  

  return joystickError; //TODO: may remove init func later; seems pointless
};







#endif //UTIL_INPUT_H_