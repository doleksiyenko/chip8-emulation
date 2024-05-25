import pygame

from renderer import Renderer
from memory import Memory
from clock import Clock
from cpu import CPU
from sound import Sound

class Chip8:
    def __init__(self):
        self.memory = Memory()
        self.renderer = Renderer()
        self.sound = Sound()

        # run the CPU at 700 instructions per second
        self.clock = Clock(rate=700)

        self.cpu = CPU(memory=self.memory)
        self.running = False

    def start(self):
        """
        Enter the main Fetch / Decode / Execute loop of the Chip8
        """
        self.running = True

        # set the screen to black to start with
        self.renderer.clear_screen()

        while self.running:
            # fetch instruction from memory
            pc = self.cpu.get_pc()

            # each instruction is 2 bytes long, get the first and second bytes
            instruction_b1 = self.memory.get_instruction(loc=pc)
            instruction_b2 = self.memory.get_instruction(loc=pc + 1)

            # increment the program counter by 2
            self.cpu.set_pc(pc + 2)



            # if the 'x' is pressed on the window, close the emulator 
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.close()


            # wait if sufficient time has not passed
            self.clock.tick()

    def close(self):
        """
        Stop running the emulator - exit running loop and quit out of pygame
        """
        self.renderer.quit()
        self.running = False
