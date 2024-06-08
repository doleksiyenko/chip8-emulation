#ifndef CHIP8_H
#define CHIP8_H

class Chip8 {
    public:
        Chip8();
        void start_emulator();
        void quit_emulator();
    private:
        bool running = false;
};

#endif