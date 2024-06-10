#include "memory.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>

Memory::Memory() {
    // load the font data into memory
    std::array<uint8_t, 75> font = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2 0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F 
    };
    
    // copy font into the beginning of ram
    std::copy(font.begin(), font.end(), memory.begin());
}

// overload the << operator so that we can print a representation of the memory object
std::ostream& operator<<(std::ostream& stream, const Memory& obj) {
    for (int i = 0; i < obj.memory.size(); i++) {
        if (i % 8 == 0) {
            stream << std::endl << "0x" << std::hex << i << ":";
        }

        uint8_t byte = obj.memory[i];
        stream << std::hex << unsigned(byte) << " ";
        
    } 

    return stream;
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

    if (!rom_file) {
        // error opening the file
        std::cout << "Error: could not find specified ROM file." << std::endl;
        exit(-1);
    }

    // start loading into address 0x200 (after font + system) 
    int location = 0x200;
    uint8_t byte;
    
    // read bytes from file one by one, and insert them into <location>
    while (rom_file >> byte) {
        memory[location] = byte;
        location++;
    } 

    rom_file.close();
}
