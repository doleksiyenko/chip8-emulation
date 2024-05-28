import pygame

from renderer import Renderer
from memory import Memory
from clock import Clock
from cpu import CPU
from sound import Sound

class Chip8:
    def __init__(self, ROM):
        # create RAM and load the ROM into RAM
        self.memory = Memory()
        self.memory.load_program(ROM=ROM)

        # create Renderer, CPU and Sound modules
        self.renderer = Renderer()
        self.sound = Sound()
        self.cpu = CPU(memory=self.memory, renderer=self.renderer, sound=self.sound)

        # run the CPU at 720 instructions per second (720 / 12 == 60, for 
                                                    # renderer clock)
        self.clock = Clock(rate=720)

        # run the renderer clock at 60 fps
        self.renderer_clock = Clock(rate=60)

        self.running = False

    def start(self):
        """
        Enter the main Fetch / Decode / Execute loop of the Chip8
        """
        self.running = True
        # set the screen to black to start with
        self.renderer.clear_screen()
        self.renderer.render()

        while self.running:
            # fetch, decode and execute the instruction at the pc
            self.cpu.cycle()
            
            # update all modules that rely on 60Hz clock
            if self.renderer_clock.elapsed():
                self.renderer.render()
                self.sound.decrement_timer()
                self.cpu.decrement_timer()

            # run a clock that 
            # if the 'x' is pressed on the window, close the emulator 
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.close()

            # wait if sufficient time has not passed (for a CPU cycle)
            self.clock.tick()

    def close(self):
        """
        Stop running the emulator - exit running loop and quit out of pygame
        """
        self.renderer.quit()
        self.running = False
