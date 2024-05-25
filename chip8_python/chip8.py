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

        self.cpu = CPU()
        self.running = False

    def start(self):
        """
        Enter the main Fetch / Decode / Execute loop of the Chip8
        """
        self.running = True

        while self.running:
            self.renderer.clear_screen()

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.close()

    def close(self):
        self.renderer.quit()
        self.running = False
