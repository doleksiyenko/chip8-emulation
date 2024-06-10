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

void CPU::cycle() {
    uint16_t instruction = fetch();
    std::cout << std::hex << int(instruction) << std::endl;
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

void CPU::decode_execute(uint8_t instruction) {

}

void CPU::decrement_timer() {

}