#include "sound.h"

unsigned int voice_frames[4];
unsigned char index;

void play_sound(unsigned char voice, unsigned char pitch, unsigned char distortion, 
unsigned char volume, unsigned int frames) {
    voice_frames[voice] = frames;
    _sound(voice,pitch,distortion,volume);
}

void stop_sound(unsigned char voice) {
    _sound(voice,0,0,0);
}

// TODO: optimize this? (currently takes ~8.5% of a frame)
void update_voice_frames() {
    index = 0;
    for (index = 0; index < 4; index++) {
        voice_frames[index]--;
        if (voice_frames[index] < 1) {
            stop_sound(index);
        }
    }
}