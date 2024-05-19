class Memory:
    """
    Class representing memory of the CHIP-8, and all associated read/write operations

    All memory on the Chip-8 system is writeable.
    """
    def __init__(self):
        # we want to store 4Kb of data, assume that each int can store at
        # most 1 byte of info.
        self.memory = [0] * 4096
    
    def load_program(self, ROM):
        """
        A Chip-8 program is loaded into memory starting at address 200 
        """
        pass

    def load_interpreter(self, interpreter):
        pass


    def __load_file(self, file, memory_location):
        pass
