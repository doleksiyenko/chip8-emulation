#include <cpu.h>
#include <cstdint>
#include <stack>

CPU::CPU() {
    pc = 0x200; // start the program counter at the beginning of the loaded ROM
    i_register = 0x0;
}

void CPU::cycle() {

}

uint8_t CPU::fetch() {

}

void CPU::decode_execute(uint8_t instruction) {

}

void CPU::decrement_timer() {

}