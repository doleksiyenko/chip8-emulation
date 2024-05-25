import time
import typing

class Clock:
    """
    Clock class creates a clock with a specified framerate. Whenever the Clock method tick is called
    check if enough time has passed to run the next frame, otherwise delay.
    """
    def __init__(self, rate: int) -> None:
        self.rate = 1 / rate 
        self.prev_time = time.time()
    
    def tick(self) -> None:
        # check the current time vs the previous time, if enough time has passed since the last frame, 
        # then return
        if time.time() - self.prev_time < self.rate:
            # wait
            time.sleep(self.rate - (time.time() - self.prev_time)) 

        self.prev_time = time.time()
