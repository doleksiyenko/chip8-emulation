#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <array>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

class Renderer {
    public:
        Renderer();
        void clear_screen(); // paint the screen black
        bool get_pixel_is_on(unsigned int x, unsigned int y);
        void set_pixel(unsigned int x, unsigned int y, bool status); // set the pixel on / off
        void render(); // draw out to the screen
        void quit();
    private:
        SDL_Window* window_; // window object which holds info about win pos, size, etc.
        SDL_Renderer* renderer_; // renderer object for rendering within the window obj
        std::array<bool, SCREEN_WIDTH * SCREEN_HEIGHT> pixel_status_{}; // array which holds if pixels are set on or off
};

#endif