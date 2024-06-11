#include <cpu.h>
#include <cstdint>
#include <iostream>
#include <stack>
    
#include <memory.h>

CPU::CPU(Memory* chip8_memory, Renderer* chip8_renderer) {
    pc = 0x200; // start the program counter at the beginning of the loaded ROM
    i_register = 0x0;

    CPU::chip8_memory = chip8_memory;
    CPU::chip8_renderer = chip8_renderer; 
}

void CPU::decrement_timer() {
    delay_timer--;
}

void CPU::cycle() {
    // first get the instruction
    uint16_t instruction = fetch();
    // using the fetched instruction, run the proper function from linked hardware
    decode_execute(instruction);
}

uint16_t CPU::fetch() {
    // read the instruction currently being pointed at by pc
    uint8_t instruction_byte1 = chip8_memory->get_from_memory(pc);
    uint8_t instruction_byte2 = chip8_memory->get_from_memory(pc + 1);
    uint16_t instruction = (instruction_byte1 << 8) + (instruction_byte2);
    // after reading the instructions, increment the pc immediately
    pc += 2;
    return instruction; 
}

void CPU::decode_execute(uint16_t instruction) {
    switch (instruction & 0xf000) {
        case 0x0000:
            if (instruction == 0x00e0) {
                // 00e0: clear the screen
                chip8_renderer->clear_screen();
            }
            break; 
        case 0x1000:
            // jump instruction to set PC to final bytes of hex 
            pc = instruction & 0x0fff;
            break; 
        case 0x2000:
            break; 
        case 0x3000:
            break; 
        case 0x4000:
            break; 
        case 0x5000:
            break; 
        case 0x6000:
            // set register VX to NN, from instruction 6XNN
            var_registers[(instruction & 0x0f00) >> 8] = instruction & 0x00ff;
            break; 
        case 0x7000:
            // add NN to value in register VX, from instruction 7XNN
            // without setting the carry flag
            var_registers[(instruction & 0x0f00) >> 8] += instruction & 0x00ff;
            break; 
        case 0x8000:
            break; 
        case 0x9000:
            break; 
        case 0xa000:
            i_register = instruction & 0x0fff;
            break; 
        case 0xb000:
            break; 
        case 0xc000:
            break; 
        case 0xd000:
            break; 
        case 0xe000:
            break;
        case 0xf000:
            break; 
    }
}
