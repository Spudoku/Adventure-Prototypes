#ifndef UTIL_INPUT_H_
#define UTIL_INPUT_H_

#include <atari.h>
#include "util_structs.h"
#include <joystick.h>
#include <assert.h>


unsigned char InitializeJoystick();

//extra grandular masks that atari.h doesnt have
#define JOY_UPDOWN_MASK 0x03

#define JOY_UPDOWN(v) ((v) & JOY_UPDOWN_MASK)

#define JOY_LEFTRIGHT_MASK 0x0C

#define JOY_LEFTRIGHT(v) ((v) & JOY_LEFTRIGHT_MASK)

#endif