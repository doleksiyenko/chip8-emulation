"""
A class for handling the CPU operations for the CHIP-8
"""
class CPU:
    def __init__(self, ) -> None:
        self.pc = 0

    def get_pc(self) -> int:
        return self.pc

    def set_pc(self, val) -> None:
        self.pc = val

