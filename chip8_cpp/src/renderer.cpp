#include <SDL2/SDL.h>
#include "renderer.h"
#include <iostream>

Renderer::Renderer() {
    // constructor - initialize the SDL2 components (window, renderer, etc.)
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: "  << SDL_GetError();
        exit(-1);
    }

    // create the window + renderer 
    window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
}

void Renderer::clear_screen() {
    // set draw color to blue and then paint the screen
    SDL_SetRenderDrawColor(renderer, 3, 123, 117, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

void Renderer::quit() {
    // quit out of all SDL processes
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}