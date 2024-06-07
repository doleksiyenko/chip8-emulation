#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <string>

class Memory {
    public:
        Memory();
        void load_ROM(std::string file_path);
        int get_from_memory(int memory_loc);
        void set_memory(int memory_loc);

    private:
        uint8_t memory[4096];
};

#endif