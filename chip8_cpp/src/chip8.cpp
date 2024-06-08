#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.h"
#include "renderer.h"

void Chip8::run() {
    // SDL event handler
    SDL_Event event; 
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            default:
                break;
        }

        SDL_Delay(5000);
        renderer.quit();
    }


    // SDL_Delay(5000);
    // // quit sdl - close the renderer and window 
    // renderer.quit();
}

