#ifndef MEMORY_H
#define MEMORY_H

#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <array>
#include <cstdint>
#include <ostream>
#include <string>
#include <unordered_map>

class Memory {
    public:
        Memory();
        void load_ROM(std::string file_path);
        int get_from_memory(int memory_loc);
        void set_memory(int memory_loc, uint8_t val);
        uint8_t hex_to_keycode(uint8_t key) {return key_translation.at(key)}; // translate a hex 0-f to an SDL keycode
    private:
        std::array<uint8_t, 4096> memory_{};
        std::unordered_map<uint8_t, uint8_t> key_translation {{0x0, SDL_SCANCODE_1}, {0x1, SDL_SCANCODE_2}, {0x2, SDL_SCANCODE_3}, {0x3, SDL_SCANCODE_4},
                                                                 {0x4, SDL_SCANCODE_Q}, {0x5, SDL_SCANCODE_W}, {0x6, SDL_SCANCODE_E}, {0x7, SDL_SCANCODE_R},
                                                                 {0x8, SDL_SCANCODE_A}, {0x9, SDL_SCANCODE_S}, {0xa, SDL_SCANCODE_D}, {0xb, SDL_SCANCODE_F},
                                                                 {0xc, SDL_SCANCODE_Z}, {0xd, SDL_SCANCODE_X}, {0xe, SDL_SCANCODE_C}, {0xf, SDL_SCANCODE_V}};
    friend std::ostream& operator<<(std::ostream& stream, const Memory& obj);
};

// overload the << operator
std::ostream& operator<<(std::ostream& stream, const Memory& obj);

#endif