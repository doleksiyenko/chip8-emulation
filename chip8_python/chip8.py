from renderer import Renderer
from memory import Memory
from clock import Clock

if __name__ == "__main__":
    memory = Memory()
    # renderer = Renderer()

    clock = Clock(1)
    for i in range(10):
        print(i)
        clock.tick()

    # renderer.quit()
