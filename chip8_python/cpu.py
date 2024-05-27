from typing import Tuple

from memory import Memory
from renderer import Renderer
"""
A class for handling the CPU operations for the CHIP-8
"""
class CPU:
    def __init__(self, memory: Memory, renderer: Renderer) -> None:
        self._pc = 0x200 # program counter register, program always loads in at
                         # address 200
        self.i_register = 0x0 # the index register 

        self.registers = [0x0] * 16
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
        return int(hex_val, 16) 


    def __decode_execute(self, instruction: int) -> None:
        # first, divide the instruction into bigger categories based on 
        # the first value in the instruction
        match instruction & 0xf000:
            case 0x0000:
                match instruction:
                    case 0x00E0:
                        # clear the screen
                        self.renderer.clear_screen() 
                    case 0x00EE:
                        # return from subroutine
                        pass
            case 0x1000:
                # jump instruction, set the program counter to final three 
                # numbers of hex
                self._pc = instruction & 0x0fff 
            case 0x2000:
                pass
            case 0x3000:
                pass
            case 0x4000:
                pass
            case 0x5000:
                pass
            case 0x6000:
                # set register VX to NN where instruction in form 0x6XNN 
                x = (instruction & 0x0f00) >> 8
                self.registers[x] = instruction & 0x00ff
            case 0x7000:
                # add value to register VX, where instruction in form 0x7XNN 
                x = (instruction & 0x0f00) >> 8
                self.registers[x] += instruction & 0x00ff
            case 0x8000:
                pass
            case 0x9000:
                pass
            case 0xa000:
                # set the index register to NNN, where instruction is 0xANNN  
                self.i_register = instruction & 0x0fff
            case 0xb000:
                pass
            case 0xc000:
                pass
            case 0xd000:
                # set the display at coordinates X and Y, instruction is DXYN
                # x and y coords from the registers, modulo the size of 
                # the screen
                vx = self.registers[(instruction & 0x0f00) >> 8] % 64 
                vy = self.registers[(instruction & 0x00f0) >> 4] % 32 
                
                # set vf to 0
                self.registers[0xf] = 0x0

                # the sprite to be drawn to the display has N bytes of data
                n = instruction & 0x000f
                
                # the length of a row is 8, see if this row goes off the right
                # edge of the screen, and if it does, cap it
                row_len = 8
                if vx + 8 >= 64:
                    row_len = 64 - vx

                # the number of rows is n, see if the y coord plus this value
                # exceeds the bottom row of the screen, cap
                col_len = n
                if vy + n >= 32:
                    col_len = 32 - vy


                for offset in range(col_len):
                    # memory address of Nth byte of sprite data
                    memory_address = self.i_register + offset 
                    sprite_byte = self.memory.get_instruction(memory_address) 
                    sprite_byte = format(sprite_byte, '08b') 

                    # increment through all of the bits of this byte of sprite 
                    # data
                    for bit in range(row_len): 
                        sprite_bit = sprite_byte[bit]

                        y_coord = vy + offset # y coordinate on screen
                        x_coord = vx + bit # x coordinate on screen

                        if sprite_bit == '1':
                            # if the pixel at that coordinate is also on, turn
                            # the pixel off
                            if self.renderer.get_color_at_pixel(row=y_coord, col=x_coord):
                                self.renderer.set_color_at_pixel(row=y_coord, col=x_coord, val=False)
                                # set vf to 1
                                self.registers[0xf] = 1 
                            else:
                                # the pixel is off, so turn it on, keep vf at 0
                                self.renderer.set_color_at_pixel(row=y_coord, col=x_coord, val=True)
            case 0xe000:
                pass
            case 0xf000:
                pass

