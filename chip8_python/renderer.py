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

    def set_color_at_pixel(self, row: int, col: int, val: bool):
        """
        Set the pixel at position <row, col> to on/off depending on the value of
        <val>
        """
        pass

    def get_color_at_pixel(self, row: int, col: int):
        pass 

    def quit(self):
        # quit pygame
        pygame.quit()
