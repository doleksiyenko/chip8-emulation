#include <SDL2/SDL.h>

class Renderer {
    void createWindow() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "Error: "  << SDL_GetError();
        }
        else {
            SDL_Window* window = SDL_CreateWindow("CHIP-8", 0, 0, 800, 600, SDL_WINDOW_SHOWN);
            SDL_Delay(5000);
            SDL_DestroyWindow(window);
        }
        
        SDL_Quit();
    }
}