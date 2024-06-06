#include <iostream>
#include "renderer.h"
#include "chip8.h"

Chip8::Chip8() { 
    // initialize the chip 8 system by creating all the "hardware" components
    Renderer renderer;
    renderer.createWindow();
}

