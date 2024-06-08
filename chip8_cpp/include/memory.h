#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>
#include <string>

class Memory {
    public:
        Memory();
        void load_ROM(std::string file_path);
        int get_from_memory(int memory_loc);
        void set_memory(int memory_loc, uint8_t val);
    private:
        std::array<uint8_t, 4096> memory;
};

#endif