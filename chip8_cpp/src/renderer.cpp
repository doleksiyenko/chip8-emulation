#include <SDL2/SDL.h>
#include "renderer.h"
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <cstdint>
#include <iostream>

Renderer::Renderer() {
    // constructor - initialize the SDL2 components (window, renderer, etc.)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cout << "Error: "  << SDL_GetError();
        exit(-1);
    }
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    // create the window + renderer 
    window_ = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if (!window_) {
        std::cout << "Error creating window: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED); 
    if (!renderer_) {
        std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
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
    SDL_SetRenderTarget(renderer_, texture_);
    if (status == true) {
        // set the pixel to white
        SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer_, x, y);
    } 
    else {
        // set the pixel to black
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer_, x, y);
    }

    SDL_SetRenderTarget(renderer_, NULL);

    pixel_status_[x + (y * SCREEN_WIDTH)] = status;
}

void Renderer::render() {
    // copy the texture to the screen
    SDL_RenderCopy(renderer_, texture_, NULL, NULL);
    // update the window
    SDL_RenderPresent(renderer_);
}

void Renderer::quit() {
    // quit out of all SDL processes
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}