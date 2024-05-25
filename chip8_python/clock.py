import time
import typing

class Clock:
    """
    Clock class creates a clock with a specified framerate. Whenever the Clock method tick is called
    check if enough time has passed to run the next frame, otherwise delay.
    """
    def __init__(self, frame_rate: int) -> None:
        self.rate = 1 / frame_rate 
        self.prev_time = time.time()
    
    def tick(self) -> None:
        # check the current time vs the previous time, if enough time has passed since the last frame, 
        # then return
        if time.time() - self.prev_time < self.frame_rate:
            # wait
            time.sleep(self.frame_rate - (time.time() - self.prev_time)) 

        self.prev_time = time.time()
