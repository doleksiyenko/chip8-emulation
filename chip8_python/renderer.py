import pygame

class Renderer:
    """
    A class for handling the graphics output of the CHIP-8
    """
    def __init__(self):
        pygame.init()
        
        # initialize the pygame window
        self.display = pygame.display.set_mode((64, 32)) 
        
    def clear_screen(self):
        self.display.fill('black')
        pygame.display.flip()

    def update_display(self, row, col, val):
        # first update the display array with val
        pass

    def quit(self):
        # quit pygame
        pygame.quit()
