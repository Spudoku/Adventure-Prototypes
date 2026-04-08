#include "sound.h"

uint8_t voice_frames[4] = {0,0,0,0};
//unsigned char index


// from cc65 docs:
// The function is available only as a fastcall function; so, it may be used only in the presence of a prototype.
// The function is specific to the Atari 8 bit.
// Voice can be any of 0-3 different sound channels.
// Pitch goes from 0-255 (about 125 Hz to 32 Khz).
// Distortion (0-14) uses poly dividers to reshape wave in order to create a noise effect. Use 10 for a "pure" square-wave sound.
// Volume (0-15) is the intensity for the wave.
// Extra bits in those parameters will be ignored.
// Parameters are the same as for the AtariBASIC SOUND statement.

void sound_PlayNote(unsigned char voice, unsigned char pitch, unsigned char distortion, 
unsigned char volume, unsigned int frames) {
    voice_frames[voice] = frames;
    _sound(voice,pitch,distortion,volume);
}

void sound_ClearVoice(unsigned char voice) {
    _sound(voice,0,0,0);
}

// TODO: optimize this? (currently takes ~8.5% of a frame)
void sound_StateUpdate() {
    register uint8_t index = 0;
    for (; index < 4; index++) {
        switch(voice_frames[index]){
            case 0:
                break;
            case 1:
                STOP_SOUND(index);  //fallthrough
            default:
                voice_frames[index]--;
                break;
        }
        
    }
}

void sound_generic_buzz() {
    
    sound_PlayNote(1,46,10,4,20);
}

 void dragon_chomp_sound() {
    // remember: voice, pitch, distortion, volume, frames
    sound_PlayNote(1,150,2,3,20);
}

void dragon_eat_sound() {
    sound_PlayNote(1,20,10,3,90);
}