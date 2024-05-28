from pygame import mixer

class Sound:
    """
    A class for handling the sound output of the CHIP-8
    """
    def __init__(self):
        self.sound_timer = 0
        self.beep = mixer.Sound("./SOUND/beep.mp3")
    
    def play_beep(self) -> None:
        self.beep.play() 
