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
        void decrement_timer();

    private:
        uint16_t fetch(); // fetch instruction from memory
        void decode_execute(uint16_t instruction); // decode and then execute instruction

    private:
        uint16_t pc_; // program counters
        std::stack<uint16_t> stack_;
        // registers
        uint16_t i_register_;
        std::array<uint8_t, 16> var_registers_{}; 
        uint8_t delay_timer_;

        // create pointers to all of the hardware components
        Memory* memory_;
        Renderer* renderer_; 

};

#endif