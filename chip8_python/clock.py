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
        time_diff = time.time() - self.prev_time 
        if time_diff < self.rate:
            # wait
            time.sleep(self.rate - (time_diff)) 

        self.prev_time = time.time()

    def elapsed(self) -> bool:
        """
        Return True on the condition that enough time has passed since the last frame, otherwise Return False
        """
        if time.time() - self.prev_time < self.rate:
            return False
        else:
            # update the time so that next time we check elapsed() we do not always return True
            self.prev_time = time.time()
            return True
