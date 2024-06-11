#include <climits>
#include <cpu.h>
#include <cstdint>
#include <iostream>
#include <stack>
    
#include <memory.h>
#include <renderer.h>

CPU::CPU(Memory* chip8_memory, Renderer* chip8_renderer) {
    pc = 0x200; // start the program counter at the beginning of the loaded ROM
    i_register_ = 0x0;

    CPU::memory_ = chip8_memory;
    CPU::renderer_ = chip8_renderer; 
}

void CPU::decrement_timer() {
    delay_timer_--;
}

void CPU::cycle() {
    // first get the instruction
    uint16_t instruction = fetch();
    // using the fetched instruction, run the proper function from linked hardware
    decode_execute(instruction);
}

uint16_t CPU::fetch() {
    // read the instruction currently being pointed at by pc
    uint8_t instruction_byte1 = memory_->get_from_memory(pc);
    uint8_t instruction_byte2 = memory_->get_from_memory(pc + 1);
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
                renderer_->clear_screen();
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
            var_registers_[(instruction & 0x0f00) >> 8] = instruction & 0x00ff;
            break; 
        case 0x7000:
            // add NN to value in register VX, from instruction 7XNN
            // without setting the carry flag
            var_registers_[(instruction & 0x0f00) >> 8] += instruction & 0x00ff;
            break; 
        case 0x8000:
            break; 
        case 0x9000:
            break; 
        case 0xa000:
            i_register_ = instruction & 0x0fff;
            break; 
        case 0xb000:
            break; 
        case 0xc000:
            break; 
        case 0xe000:
            break;
        case 0xf000:
            break;
        case 0xd000:
            // draw to display instruction
            // get the X and Y coordinates from the VX and VY registers
            uint8_t vx = var_registers_[(instruction & 0x0f00) >> 8] % SCREEN_WIDTH; 
            uint8_t vy = var_registers_[(instruction & 0x00f0) >> 4] % SCREEN_HEIGHT; 

            // set the VF register to 0
            var_registers_[0xf] = 0x0;

            // cap the columns/rows to be drawn if the rows/cols exceed the screen size
            uint8_t row_len = 8;
            if (row_len + vx >= SCREEN_WIDTH) {
                row_len = SCREEN_WIDTH - vx;
            }

            // sprite to be drawn has N bytes of data col_len = N
            uint8_t col_len = instruction & 0x000f;
            if (col_len + vy >= SCREEN_HEIGHT) {
                col_len = SCREEN_HEIGHT - vy;
            }

            // for every possible column val (every byte of the sprite)
            for (int offset = 0; offset < col_len; offset++){
                uint8_t sprite_byte = memory_->get_from_memory(i_register_ + offset);
                // increment through each bit of byte of sprite data
                for (int bit = 0; bit < row_len; bit++) {
                    // get one bit at a time, starting from the MSB
                    uint8_t sprite_bit = (sprite_byte & (1 << (7 - bit))) >> (7 - bit); 
                    uint8_t y = vy + offset; // y coordinate on display
                    uint8_t x = vx + bit; // x coordinate on display

                    if (sprite_bit == 1) {
                        if (renderer_->get_pixel_is_on(x, y)) {
                            renderer_->set_pixel(x, y, false);
                            var_registers_[0xf] = 1;
                        }
                        else {
                            renderer_->set_pixel(x, y, true);
                        }
                    }
                }
                renderer_->render();
            }
            break; 
    }
}
