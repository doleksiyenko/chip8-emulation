#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.h"
#include "renderer.h"

void Chip8::run(std::string file_path) {
    // load in the ROM provided as a command line argument
    // memory.load_ROM(file_path);
    std::cout << memory << std::endl;
    // main emulation loop
    // running starts intialized as true
    while (running) {
        // event handling
        SDL_Event event; 
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                default:
                    break;
            }
        }

        // drawing
        renderer.clear_screen();
        renderer.render();

        // delay so that game runs at reasonable frame rate
        SDL_Delay(16);
    }

    // quit sdl - close the renderer and window 
    renderer.quit();
}