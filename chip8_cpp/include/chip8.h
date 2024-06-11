#ifndef CHIP8_H
#define CHIP8_H

#include "renderer.h" 
#include "memory.h"
#include "cpu.h"

class Chip8 {
    public:
        void run(std::string file_path);
    private:
        bool running_ = true; // when the Chip8 system is created, start it running by default
        
        // hardware components
        Renderer renderer_;
        Memory memory_;
        CPU cpu_{&memory_, &renderer_};
};

#endif