from pygame import mixer

class Sound:
    """
    A class for handling the sound output of the CHIP-8
    """
    def __init__(self):
        self.sound_timer = 0 
        self.beep = mixer.Sound("./SOUND/beep.mp3")
    
    def decrement_timer(self) -> None:
        # if no beep is playing, and the sound timer is greater than 0, play the beep
        if (self.sound_timer > 0) and not mixer.get_busy():
            self.beep.play()
        
        self.sound_timer -= 1

    def set_timer(self, val: int) -> None:
        """
        Set the sound timer to <val>
        """ 
        self.sound_timer = val
