#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <climits>
#include <cpu.h>
#include <cstdint>
#include <cstdlib>
#include <stack>
#include <SDL2/SDL.h>
    
#include <memory.h>
#include <renderer.h>

CPU::CPU(Memory* chip8_memory, Renderer* chip8_renderer) {
    pc_ = 0x200; // start the program counter at the beginning of the loaded ROM
    i_register_ = 0x0;

    CPU::memory_ = chip8_memory;
    CPU::renderer_ = chip8_renderer; 
}

void CPU::decrement_timer() {
    if (delay_timer_ != 0) {
        delay_timer_--;
    }
}

void CPU::cycle() {
    // first get the instruction
    uint16_t instruction = fetch();
    // using the fetched instruction, run the proper function from linked hardware
    decode_execute(instruction);
}

uint16_t CPU::fetch() {
    // read the instruction currently being pointed at by pc
    uint8_t instruction_byte1 = memory_->get_from_memory(pc_);
    uint8_t instruction_byte2 = memory_->get_from_memory(pc_ + 1);
    uint16_t instruction = (instruction_byte1 << 8) + (instruction_byte2);
    // after reading the instructions, increment the pc immediately
    pc_ += 2;
    return instruction; 
}

void CPU::decode_execute(uint16_t instruction) {
    switch (instruction & 0xf000) {
        case 0x0000:
            if (instruction == 0x00e0) {
                // 00e0: clear the screen
                renderer_->clear_screen();
            }
            else if (instruction == 0x00ee) {
                // return from subroutine function
                pc_ = stack_.top();
                stack_.pop();
            }
            break; 
        case 0x1000:
            // jump instruction to set PC to final bytes of hex 
            pc_ = instruction & 0x0fff;
            break; 
        case 0x2000:
            // call subroutine at address NNN from instruction 2NNN
            // push the current pc to the stack so that we can return later
            stack_.push(pc_);
            pc_ = instruction & 0x0fff;
            break; 
        case 0x3000:
            // skip instruction if val in register VX is equal to NN
            {
                uint8_t vx = var_registers_[(instruction & 0x0f00) >> 8];
                if (vx == (instruction & 0x00ff)) {
                    pc_ += 2;
                }
                break; 
            } 
        case 0x4000:
            // skip instruction if val in register VX is not equal to NN 
            {
                uint8_t vx = var_registers_[(instruction & 0x0f00) >> 8];
                if (vx != (instruction & 0x00ff)) {
                    pc_ += 2;
                }
                break; 
            }
        case 0x5000:
            {
                uint8_t vx = var_registers_[(instruction & 0x0f00) >> 8];
                uint8_t vy = var_registers_[(instruction & 0x00f0) >> 4];
                if (vx == vy) {
                    pc_ += 2;
                }
                break; 
            }
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
            {
                uint8_t vx = var_registers_[(instruction & 0x0f00) >> 8]; 
                uint8_t vy = var_registers_[(instruction & 0x00f0) >> 4]; 

                switch (instruction & 0x000f) {
                    case 0x0:
                        // set vx = vy
                        var_registers_[(instruction & 0x0f00) >> 8] = var_registers_[(instruction & 0x00f0) >> 4];
                        break;
                    case 0x1:
                        // set vx to the bitwise OR of vx and vy
                        var_registers_[(instruction & 0x0f00) >> 8] = vx | vy;
                        break;
                    case 0x2:
                        // bitwise AND
                        var_registers_[(instruction & 0x0f00) >> 8] = vx & vy;
                        break;
                    case 0x3:
                        // bitwise XOR 
                        var_registers_[(instruction & 0x0f00) >> 8] = vx ^ vy;
                        break;
                    case 0x4:
                        {
                            // ADD VX and VY and affect the overflow flag
                            uint16_t total = vx + vy;
                            // if there is an overflow, set the flag
                            if (total > 255) {
                                var_registers_[0xf] = 1;                            
                            }
                            else {
                                var_registers_[0xf] = 0;
                            }
                            var_registers_[(instruction & 0x0f00) >> 8] = static_cast<uint8_t>(vx + vy);
                            break;
                        }
                    case 0x5:
                        // set the carry flag to 0 if we underflow
                        if (vx > vy) {
                            var_registers_[0xf] = 0;
                        }
                        else {
                            var_registers_[0xf] = 1;
                        }
                        // subtract: vx = vx - vy
                        var_registers_[(instruction & 0x0f00) >> 8] = vx - vy;
                        break;
                    case 0x6:
                        // shift vx one bit to the right and set carry flag appropriately
                        if (vy % 2 == 0) {
                            // last bit is zero
                            var_registers_[0xf] = 0;
                        }
                        else {
                            var_registers_[0xf] = 1;
                        }

                        var_registers_[(instruction & 0x0f00) >> 8] = vy >> 1;

                        break;
                    case 0x7:
                        // set the carry flag to 0 if we underflow
                        if (vy > vx) {
                            var_registers_[0xf] = 0;
                        }
                        else {
                            var_registers_[0xf] = 1;
                        }
                        // subtract: vx = vx - vy
                        var_registers_[(instruction & 0x0f00) >> 8] = vy - vx;
                        break;
                    case 0xe:
                        if ((vy & 0x80) >> 7 == 0) {
                            // last bit is 0
                            var_registers_[0xf] = 0;
                        }
                        else {
                            var_registers_[0xf] = 1;
                        }

                        // shift vx (= vy) to the right (crop to 8 bits)
                        var_registers_[(instruction & 0x0f00) >> 8] = (vy << 1);
                        break;
                } 
            }
            break; 
        case 0x9000:
            {
                uint8_t vx = var_registers_[(instruction & 0x0f00) >> 8];
                uint8_t vy = var_registers_[(instruction & 0x00f0) >> 4];
                if (vx != vy) {
                    pc_ += 2;
                }
                break; 
            }
        case 0xa000:
            // set index register
            i_register_ = instruction & 0x0fff;
            break; 
        case 0xb000:
            // jump with NNN + offset stored in v0
            pc_ = var_registers_[0x0] + (instruction & 0x0fff);
            break; 
        case 0xc000:
            // vx = rand & NN
            var_registers_[(instruction & 0x0f00) >> 8] = (instruction & 0x00ff) & static_cast<uint8_t>((rand() % instruction & 0x00ff));
            break; 
        case 0xd000:
            {
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
                }
                break; 
            }
        case 0xe000:
            {
                // check if a key is being pressed, and if it is
                // if the key that is being pressed is a valid key in the CHIP 8 system 
                const uint8_t* state = SDL_GetKeyboardState(nullptr);
                uint8_t scancode = memory_->hex_to_scancode_translation[(instruction & 0x0f00) >> 8];
                if (((instruction & 0x000f) == 0xe) && state[scancode]) {
                    // the key being queried is being pressed
                    pc_ += 2;
                } else if (((instruction & 0x000f) == 0x1) && !state[scancode]) {
                    // TODO: this checks if the key is not being pressed, but not if it is a valid key on the CHIP-8 system
                    pc_ += 2;
                }
                break;
            }
        case 0xf000:
            switch (instruction & 0x00ff) {
                case 0x07:
                    // set the delay timer from the VX register
                    delay_timer_ = var_registers_[(instruction & 0x0f00) >> 8];
                    break;
                case 0x15:
                    // get the delay tier into the VX register
                    var_registers_[(instruction & 0x0f00) >> 8] = delay_timer_;
                    break;
                case 0x18:
                    // TODO : set the sound timer 
                    break;
                case 0x1e:
                    // add value in vx to index register
                    i_register_ += var_registers_[(instruction & 0x0f00) >> 8];
                    break;
                case 0x0a:
                    // get key (blocking call)
                    {
                        bool key_up = false;
                        SDL_Event event;
                        while (SDL_PollEvent(&event)) {
                            switch (event.type) {
                                case SDL_QUIT:
                                    renderer_->quit();
                                    exit(0);
                                case SDL_KEYUP:
                                    uint8_t key_released_scancode = event.key.keysym.scancode;
                                    if (memory_->scancode_to_hex_translation.find(key_released_scancode) != memory_->scancode_to_hex_translation.end()) {
                                        // set register VX to the hex translation
                                        var_registers_[0xf] = memory_->scancode_to_hex_translation[key_released_scancode];
                                        key_up = true;
                                        break;
                                    }
                            }
                        }

                        // made it through the event loop without triggering the SDL_KEYUP event, so made a blocking call
                        if (!key_up) {
                            pc_ -= 2;
                        }
                    }
                    break;
                case 0x29:
                    // instruction : load font character
                    // each font character is 5 bytes long, so take vx * 5 to find the appropriate character 
                    i_register_ = memory_->get_from_memory(5 * var_registers_[(instruction & 0x0f00) >> 8]);
                    break;
                case 0x33:
                    // binary-coded decimal conversion (extract digits)
                    {
                        uint8_t vx = var_registers_[(instruction & 0x0f00) >> 8];
                        memory_->set_memory(i_register_ + 0, static_cast<uint8_t>(vx / 100) % 10);
                        memory_->set_memory(i_register_ + 1, static_cast<uint8_t>(vx / 10) % 10);
                        memory_->set_memory(i_register_ + 2, static_cast<uint8_t>(vx / 1) % 10);
                    }
                    break;
                case 0x55:
                    // store registers in memory up to register VX (inclusive) 
                    {
                        uint8_t x = (instruction & 0x0f00) >> 8;
                        for (uint8_t i = 0; i <= x; i++) {
                            uint8_t vx = var_registers_[(instruction & 0x0f00) >> 8];
                            memory_->set_memory(i_register_ + i, vx);
                        }
                    }
                    break;
                case 0x65:
                    // load memory into registers
                    {
                        uint8_t x = (instruction & 0x0f00) >> 8;
                        for (uint8_t i = 0; i <= x; i++) {
                            var_registers_[i] = memory_->get_from_memory(i_register_ + i);
                        }
                    }
                    break;
            }
            break;
    }
}
