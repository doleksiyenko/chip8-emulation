#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <stack>

#include <memory.h>
#include <renderer.h>


class CPU {
    public:
        CPU(Memory* chip8_memory, Renderer* chip8_renderer);
        void cycle(); // run a single CPU cycle

    private:
        uint16_t fetch(); // fetch instruction from memory
        void decode_execute(uint8_t instruction); // decode and then execute instruction
        void decrement_timer();

    private:
        uint16_t pc; // program counters
        std::stack<uint8_t> stack;
        // registers
        uint16_t i_register;
        std::array<uint8_t, 16> var_registers{}; 
        uint8_t delay_timer;

        // create pointers to all of the hardware components
        Memory* chip8_memory;
        Renderer* chip8_renderer; 

};

#endif