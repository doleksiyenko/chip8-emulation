#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

class Renderer {
    public:
        Renderer();
        void clear_screen(); // paint the screen black
        void render(); // draw out to the screen
        void quit();
    private:
        SDL_Window* window_; // window object which holds info about win pos, size, etc.
        SDL_Renderer* renderer_; // renderer object for rendering within the window obj
};

#endif