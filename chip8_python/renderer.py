import pygame

class Renderer:
    """
    A class for handling the graphics output of the CHIP-8
    """
    def __init__(self):
        pygame.init()
        
        # initialize the pygame window
        self.display = pygame.display.set_mode((64, 32)) 
        self.white = (255, 255, 255)
        self.black = (0, 0, 0)

    def clear_screen(self) -> None:
        self.display.fill('black')

    def set_color_at_pixel(self, row: int, col: int, val: bool) -> None:
        """
        Set the pixel at position <row, col> to white if val is True, and black
        if val is False.
        """
        if val:
            self.display.set_at((col, row), 'white')
        else:
            self.display.set_at((col, row), 'black')

    def get_color_at_pixel(self, row: int, col: int) -> bool:
        """
        Return 0 if the display is black and 1 if the display is white
        at the given pixel at location <row, col>
        """
        return True if self.display.get_at((row, col)) == self.white else False


    def render(self):
        """
        Update the whole display. This should be called at a given frame rate
        (i.e. 60 Hz) and not at every CPU clock cycle.
        """
        pygame.display.flip()

    def quit(self):
        """
        Close the renderer (quit pygame)
        """
        pygame.quit()
