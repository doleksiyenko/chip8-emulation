#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL_error.h>
#include <cstdint>
#include <iostream>

#include "sound.h"

Sound::Sound() {
    // initialize the SDL mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
        std::cout << "Error: " << Mix_GetError();
        exit(-1);
    }

    beep = Mix_LoadWAV(BEEP_PATH);
    if (beep == NULL) {
        std::cout << "Could not load beep file: " << Mix_GetError();
        exit(-1);
    }
}

void Sound::decrement_timer() {
    if (sound_timer_ > 0) {
        if (Mix_Playing(-1) == 0) {
            Mix_PlayChannel(-1, beep, 0);
        } 
        sound_timer_--;
    }
}

void Sound::set_timer(uint8_t val) {
    sound_timer_ = val;
}

void Sound::quit() {
    Mix_FreeChunk(beep);
    Mix_CloseAudio();
    Mix_Quit();
}