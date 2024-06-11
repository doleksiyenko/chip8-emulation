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
    window_ = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED); 
}

void Renderer::clear_screen() {
    // set draw color to black and then paint the screen
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);
}

void Renderer::render() {
    SDL_RenderPresent(renderer_);
}

void Renderer::quit() {
    // quit out of all SDL processes
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}