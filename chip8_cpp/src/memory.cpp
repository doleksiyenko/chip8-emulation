#include "memory.h"

#include <_types/_uint8_t.h>
#include <cstdint>
#include <fstream>
#include <ios>

Memory::Memory() {

}

int Memory::get_from_memory(int memory_loc) {
    return memory[memory_loc];
}

void Memory::set_memory(int memory_loc, uint8_t val) {
    memory[memory_loc] = val; 
}

void Memory::load_ROM(std::string file_path) {
    std::ifstream rom_file;
    rom_file.open(file_path, std::ios::binary | std::ios::in); 
    rom_file.close();
}
