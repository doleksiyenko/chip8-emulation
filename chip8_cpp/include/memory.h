#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>
#include <ostream>
#include <string>

class Memory {
    public:
        Memory();
        void load_ROM(std::string file_path);
        int get_from_memory(int memory_loc);
        void set_memory(int memory_loc, uint8_t val);
    private:
        std::array<uint8_t, 4096> memory;
    friend std::ostream& operator<<(std::ostream& stream, const Memory& obj);
};

// overload the << operator
std::ostream& operator<<(std::ostream& stream, const Memory& obj);

#endif