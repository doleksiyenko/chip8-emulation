import sys

from chip8 import Chip8

if __name__ == "__main__":
    # check if a ROM is specified
    if len(sys.argv) != 2:
        print("Please specify ROM")

    # create the Chip 8 system
    chip8 = Chip8(ROM=sys.argv[1])
    chip8.start()
