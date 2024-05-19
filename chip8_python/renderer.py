import numpy as np
import cv2 as cv

class Renderer:
    """
    A class for handling the graphics output of the CHIP-8
    """
    def __init__(self):
        self.display = np.zeros(shape=(32, 64)) 

        # open up a new opencv window
        cv.imshow("Chip-8 Emulator", self.display)
        cv.namedWindow("Chip-8 Emulator",cv.WND_PROP_FULLSCREEN)
        cv.setWindowProperty("Chip-8 Emulator", cv.WND_PROP_FULLSCREEN, cv.WINDOW_FULLSCREEN)
        cv.waitKey(0)

    def update_display(self, row, col, val):
        # first update the display array with val

        self.display[row, col] = val

    def render(self):
        # depending on the FPS decided, call this function at a given frequency
        # to update the visible display from time to time

        cv.imshow("Chip-8 Emulator", self.display)

    def quit(self):
        # close down opencv
        cv.destroyAllWindows()
