import binascii

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
                
    def get_instruction(self, loc) -> int:
        return self.memory[loc]        
