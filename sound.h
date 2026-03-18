#ifndef SOUND_H_
#define SOUND_H_
#include <atari.h>

extern unsigned int voice_frames[];

// play a sound for n frames at a given voice
// note: do not pass a value > 15 for volume
// note: sum of all 4 channels' volumes should not exceed 32
extern void play_sound(unsigned char voice, unsigned char pitch, 
unsigned char distortion, unsigned char volume, unsigned int frames);


extern void stop_sound(unsigned char voice);

extern void stop_all_sound();

extern void update_voice_frames();




#endif