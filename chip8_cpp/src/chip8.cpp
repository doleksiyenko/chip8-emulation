#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.h"
#include "renderer.h"
#include "cpu.h"

void Chip8::run(std::string file_path) {
    // load in the ROM provided as a command line argument
    memory_.load_ROM(file_path);
    // show the contents of memory in the terminal
    std::cout << memory_ << std::endl;
    // main emulation loop
    // running starts intialized as true
    while (running_) {
        // run a single cycle of the CPU (read one 16 byte instruction)
        cpu_.cycle(); 
        // event handling
        SDL_Event event; 
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running_ = false;
                    break;
                default:
                    break;
            }
        }

        // drawing
        renderer_.clear_screen(); // fill the screen with black
        renderer_.render(); // load the updated texture to the blank screen

        // decrement sound and delay timer
        cpu_.decrement_timer();
        // delay so that game runs at reasonable frame rate
        SDL_Delay(16);
    }

    // quit sdl - close the renderer and window 
    renderer_.quit();
}