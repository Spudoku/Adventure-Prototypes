#include "sound.h"

uint8_t voice_frames[4] = {0,0,0,0};
//unsigned char index;

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
    // remember: voice, pitch, distortion, volume, frames
    sound_PlayNote(1,46,10,4,20);
}