#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <stack>

class CPU {
    public:
        CPU();
        void cycle(); // run a single CPU cycle
    private:
        uint8_t fetch(); // fetch instruction from memory
        void decode_execute(uint8_t instruction); // decode and then execute instruction
        void decrement_timer();
    private:
        uint16_t pc; // program counters
        std::stack<uint8_t> stack;
        // registers
        uint16_t i_register;
        std::array<uint8_t, 16> var_registers{}; 
        uint8_t delay_timer;

};

#endif