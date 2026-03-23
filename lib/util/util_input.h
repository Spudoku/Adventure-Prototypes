#ifndef UTIL_INPUT_H_
#define UTIL_INPUT_H_

#include <atari.h>
#include <joystick.h>

//extra grandular masks that atari.h doesnt have
//used for speeding up input processing
#define JOY_UPDOWN_MASK 0x03

#define JOY_UPDOWN(v) ((v) & JOY_UPDOWN_MASK)

#define JOY_LEFTRIGHT_MASK 0x0C

#define JOY_LEFTRIGHT(v) ((v) & JOY_LEFTRIGHT_MASK)

#endif