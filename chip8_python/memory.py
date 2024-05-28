import binascii
import pygame

class Memory:
    """
    Class representing memory of the CHIP-8, and all associated read/write operations

    All memory on the Chip-8 system is writeable.
    """
    def __init__(self):
        # we want to store 4Kb of data, assume that each int can store at
        # most 1 byte of info.
        self.memory = [0x0] * 4096

        # the limitation in previous interpreters is that the stack would be of limited
        # size stored in the 4 Kb of memory, but don't need to face this limitation since
        # we're simply emulating the function
        self.stack = []

        # when the memory is created, load the system font into memory
        # reserve first 80 bytes for font
        
        font = [0xF0, 0x90, 0x90, 0x90, 0xF0,
                0x20, 0x60, 0x20, 0x20, 0x70, 
                0xF0, 0x10, 0xF0, 0x80, 0xF0, 
                0xF0, 0x10, 0xF0, 0x10, 0xF0, 
                0x90, 0x90, 0xF0, 0x10, 0x10, 
                0xF0, 0x80, 0xF0, 0x10, 0xF0, 
                0xF0, 0x80, 0xF0, 0x90, 0xF0, 
                0xF0, 0x10, 0x20, 0x40, 0x40, 
                0xF0, 0x90, 0xF0, 0x90, 0xF0, 
                0xF0, 0x90, 0xF0, 0x10, 0xF0, 
                0xF0, 0x90, 0xF0, 0x90, 0x90, 
                0xE0, 0x90, 0xE0, 0x90, 0xE0, 
                0xF0, 0x80, 0x80, 0x80, 0xF0, 
                0xE0, 0x90, 0x90, 0x90, 0xE0, 
                0xF0, 0x80, 0xF0, 0x80, 0xF0, 
                0xF0, 0x80, 0xF0, 0x80, 0x80] 
        
        # these are the valid keypresses for the keyboard (with their correspondences in hexadecimal)
        self.valid_keys = {0x0: pygame.key.key_code("1"), 0x1: pygame.key.key_code("2"), 0x2: pygame.key.key_code("3"), 0x3: pygame.key.key_code("4"), \
                           0x4: pygame.key.key_code("q"), 0x5: pygame.key.key_code("w"), 0x6: pygame.key.key_code("e"), 0x7: pygame.key.key_code("r"), \
                           0x8: pygame.key.key_code("a"), 0x9: pygame.key.key_code("s"), 0xa: pygame.key.key_code("d"), 0xb: pygame.key.key_code("f"), \
                           0xc: pygame.key.key_code("z"), 0xd: pygame.key.key_code("x"), 0xe: pygame.key.key_code("c"), 0xf: pygame.key.key_code("v")}

        # create a hex loop up for a keycode
        self.key_hex = dict((v,k) for k,v in self.valid_keys.items())

        # store font in the front of the memory
        for i in range(len(font)):
            self.memory[i] = font[i]

    def __str__(self):
        string = f""
        for row_i in range(len(self.memory) // 8):
            string += f"{hex(row_i * 8)}: {str(self.memory[row_i * 8: row_i * 8 + 8])}\n"
        return string

    def load_program(self, ROM: str) -> None:
        """
        A Chip-8 program is loaded into memory starting at address 0x200 (512) 
        """
        with open(ROM, 'rb') as ROM_file:
            data = binascii.hexlify(ROM_file.read())
            for byte_i in range(len(data) // 2):
                byte = data[byte_i * 2: (byte_i * 2) + 2]
                byte = "0x" + byte.decode("utf-8")
                
                # put the hex value into memory starting at 0x200
                self.memory[0x200 + byte_i] = int(byte, 16) 

        print(self)
                
    def get_from_memory(self, loc) -> int:
        return self.memory[loc]        
    
    def set_memory(self, loc, val) -> None:
        self.memory[loc] = val
