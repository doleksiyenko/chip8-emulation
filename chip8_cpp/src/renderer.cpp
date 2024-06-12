#include <SDL2/SDL.h>
#include "renderer.h"
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <cstdint>
#include <iostream>

Renderer::Renderer() {
    // constructor - initialize the SDL2 components (window, renderer, etc.)
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: "  << SDL_GetError();
        exit(-1);
    }
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    // create the window + renderer 
    window_ = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (!window_) {
        std::cout << "Error creating window: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED); 
    if (!renderer_) {
        std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
        exit(-1);
    }
}

void Renderer::clear_screen() {
    // set draw color to black and then paint the screen
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);
}

bool Renderer::get_pixel_is_on(unsigned int x, unsigned int y) {
    return pixel_status_[x + (y * SCREEN_WIDTH)]; 
}

void Renderer::set_pixel(unsigned int x, unsigned int y, bool status) {
    if (status == true) {
        // set the pixel to white
        SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer_, x, y);
        std::cout << SDL_GetError(); 
    } 
    else {
        // set the pixel to black
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer_, x, y);
    }

    pixel_status_[x + (y * SCREEN_WIDTH)] = status;
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