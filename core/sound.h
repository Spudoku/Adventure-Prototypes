#ifndef SOUND_H_
#define SOUND_H_
#include <atari.h>
#include <stdint.h>

#define STOP_SOUND(index) _sound(index,0,0,0)

//extern unsigned int voice_frames[];

// play a sound for n frames at a given voice
// note: do not pass a value > 15 for volume
// note: sum of all 4 channels' volumes should not exceed 32
void sound_PlayNote(unsigned char voice, unsigned char pitch, 
unsigned char distortion, unsigned char volume, unsigned int frames);


void sound_ClearVoice(unsigned char voice);

void stop_all_sound();

void sound_StateUpdate();

/*
    predefined sounds;
    these are examples of what sound functions might look like
*/
void sound_generic_buzz();

void dragon_chomp_sound();

void dragon_eat_sound();

// void sound_item_pickup();

// extern void sound_item_drop();

// extern void sound_dragon_flap();

/*
    END predefined sounds
*/



#endif