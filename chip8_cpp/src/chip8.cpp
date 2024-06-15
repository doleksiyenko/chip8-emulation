#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_timer.h>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

#include "chip8.h"
#include "renderer.h"
#include "cpu.h"

void Chip8::run(std::string file_path) {
    // load in the ROM provided as a command line argument
    memory_.load_ROM(file_path);
    // show the contents of memory in the terminal
    std::cout << memory_ << std::endl;

    // define clocks so they are valid
    std::chrono::duration<double> sleep_time(0);
    std::chrono::duration<double> between_frames(0);
    const std::chrono::duration<double> cpu_rate(1.0 / 700.0); // corresponds to 1/700 instructions / second
    const std::chrono::duration<double> frame_rate(1.0 / 60.0); // render only every 60 frames per second
    auto last_render = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> last_render(0);

    // main emulation loop
    // running starts intialized as true
    while (running_) {
        // run a single cycle of the CPU (read one 16 byte instruction)
        cpu_.cycle(); 
        // time when we finished the last cpu cycle
        auto cycle_timepoint_1 = std::chrono::high_resolution_clock::now();


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

        // check if the time between when we finished the last render and now has or has not exceeded the render frame rate
        auto time_before_render = std::chrono::high_resolution_clock::now();
        between_frames = time_before_render - last_render;
        if (between_frames > frame_rate) {
            renderer_.clear_screen(); // fill the screen with black
            renderer_.render(); // load the updated texture to the blank screen
            last_render = std::chrono::high_resolution_clock::now(); 
        }

        // decrement sound and delay timer
        cpu_.decrement_timer();

        // delay so that game runs at reasonable frame rate
        auto cycle_timepoint_2 = std::chrono::high_resolution_clock::now();

        // check the time between the last completion of a cpu cycle and now, if insufficient time has passed for an effective
        // cpu clock speed of 700 instructions / second, then delay
        sleep_time = (cpu_rate - (cycle_timepoint_2 - cycle_timepoint_1));

        // sleep for enough time so that no more than 7000 instructions will be completed every second
        if (sleep_time > std::chrono::seconds(0)) {
            std::this_thread::sleep_for(sleep_time);
        }
    }

    // quit sdl - close the renderer and window 
    renderer_.quit();
}