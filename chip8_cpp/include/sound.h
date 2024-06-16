#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>
#include <cstdint>

#define BEEP_PATH "../../SOUND/beep.mp3"

class Sound {
    public:
        Sound();
        void decrement_timer();
        void set_timer(uint8_t val);
        void quit();
    private:
        uint8_t sound_timer_ = 0;
        Mix_Chunk* beep;
};

#endif