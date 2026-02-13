#include "util_input.h"


unsigned char joystickError;

unsigned char InitializeJoystick() {
  //fastcall (non ANSI C standard feature in cc65) requires an assignment, 
  //intellitype doesnt know how it works so it will freak out
  joystickError = joy_install(&atrstd_joy);  


  assert(joystickError != JOY_ERR_CANNOT_LOAD);
  assert(joystickError != JOY_ERR_NO_DEVICE);
  assert(joystickError == JOY_ERR_OK);
  

  return joystickError; //TODO: may remove init func later; seems pointless
};
