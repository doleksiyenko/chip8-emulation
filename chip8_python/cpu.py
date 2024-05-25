from typing import Tuple

from memory import Memory
from renderer import Renderer
"""
A class for handling the CPU operations for the CHIP-8
"""
class CPU:
    def __init__(self, memory: Memory, renderer: Renderer) -> None:
        self._pc = 0 # program counter register

        # create pointers to the rest of the hardware components
        self.memory = memory
        self.renderer = renderer 

    def cycle(self) -> None:
        """
        The main fetch-decode-execute cycle of the CPU
        """
        # fetch the instruction at the current PC
        instruction = self.__fetch()
        # decode and execute the instruction
        self.__decode_execute(instruction)

    def __fetch(self) -> int:
        # chip 8 instuctions are two bytes long
        instruction_byte1 = self.memory.get_instruction(self._pc)
        instruction_byte2 = self.memory.get_instruction(self._pc + 1)

        # increment the pc by two bytes 
        self._pc += 2

        # get combine the two 1 byte instructions into one 2 byte instruction
        # concatenate the two hexadecimal bytes into one instruction string
        hex_val = "0x" + format(instruction_byte1, '02x') + format(instruction_byte2, '02x') 

        # return the 2 byte instruction as a hex value (int)
        return int(hex_val) 


    def __decode_execute(self, instruction: int) -> None:
        # first, divide the instruction into bigger categories based on 
        # the first value in the instruction
        match instruction & 0xf000:
            case 0x0000:
                pass
            case 0x1000:
                pass
            case 0x2000:
                pass
            case 0x3000:
                pass
            case 0x4000:
                pass
            case 0x5000:
                pass
            case 0x6000:
                pass
            case 0x7000:
                pass
            case 0x8000:
                pass
            case 0x9000:
                pass
            case 0xa000:
                pass
            case 0xb000:
                pass
            case 0xc000:
                pass
            case 0xd000:
                pass
            case 0xe000:
                pass
            case 0xf000:
                pass

