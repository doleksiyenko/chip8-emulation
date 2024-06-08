#ifndef CHIP8_H
#define CHIP8_H

#include "renderer.h" 

class Chip8 {
    public:
        void run();
    private:
        bool running = true; // when the Chip8 system is created, start it running by default
        
        // hardware components
        Renderer renderer;
};

#endif