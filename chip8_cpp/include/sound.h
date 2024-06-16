#ifndef SOUND_H
#define SOUND_H

#include <cstdint>

class Sound {
    public:
        Sound();
        void decrement_timer();
        void set_timer(uint8_t val);
    private:
        uint8_t sound_timer_;
}

#endif