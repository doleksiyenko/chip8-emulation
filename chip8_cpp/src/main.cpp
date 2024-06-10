#include "chip8.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Must provide path to ROM" << std::endl; 
        exit(-1);
    }

    Chip8 chip8;
    chip8.run(argv[1]);
    return 0;
}