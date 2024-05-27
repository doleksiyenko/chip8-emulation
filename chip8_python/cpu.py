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
                        # return from subroutine by getting the saved pc from the stack
                        self._pc = self.memory.stack.pop() 
            case 0x1000:
                # jump instruction, set the program counter to final three 
                # numbers of hex
                self._pc = instruction & 0x0fff 
            case 0x2000:
                # call to a subroutine - first add the pc to the stack, then set pc
                self.memory.stack.append(self._pc)
                self._pc = instruction & 0x0fff
            case 0x3000:
                # skip one 2 byte instruction if vx is equal to NN in 3XNN
                val = instruction & 0x00ff 
                vx = self.registers[(instruction & 0x0f00) >> 8]

                if val == vx:
                    self._pc += 2
            case 0x4000:
                # skip one 2 byte instruction if vx is NOT equal to NN in 4XNN 
                val = instruction & 0x00ff
                vx = self.registers[(instruction & 0x0f00) >> 8]

                if val != vx:
                    self._pc += 2
            case 0x5000:
                # skip one 2 byte instruction if VX and VY are equal in 5XY0 
                vx = self.registers[(instruction & 0x0f00) >> 8]
                vy = self.registers[(instruction & 0x00f0) >> 4]

                if vx == vy:
                    self._pc += 2
            case 0x6000:
                # set register VX to NN where instruction in form 0x6XNN 
                x = (instruction & 0x0f00) >> 8
                self.registers[x] = instruction & 0x00ff
            case 0x7000:
                # add value to register VX, where instruction in form 0x7XNN 
                x = (instruction & 0x0f00) >> 8
                self.registers[x] += instruction & 0x00ff
            case 0x8000:
                # the 8XYn instructions are logical + arithmetic instructions
                # decoded based on the last 4 bits
                
                vx = self.registers[(instruction & 0x0f00) >> 8]
                vy = self.registers[(instruction & 0x00f0) >> 4]

                match instruction & 0x000f:
                    case 0x0000:
                        # set VX to VY
                        self.registers[(instruction & 0x0f00) >> 8] = vy
                    case 0x0001:
                        # vx = vx | vy
                        self.registers[(instruction & 0x0f00) >> 8] = vx | vy
                    case 0x0002:
                        # vx = vx & vy
                        self.registers[(instruction & 0x0f00) >> 8] = vx & vy
                    case 0x0003:
                        # vx = vx xor vy
                        self.registers[(instruction & 0x0f00) >> 8] = vx ^ vy
                    case 0x0004:
                        # vx = vx + vy (with overflow detection)
                        total = vx + vy

                        if total > 255:
                            # set vf to 1
                            self.registers[0xf] = 1
                        else:
                            self.registers[0xf] = 0

                        self.registers[(instruction & 0x0f00) >> 8] = (vx + vy) % 256
                    case 0x0005:
                        # vx = vx - vy 
                        self.registers[(instruction & 0x0f00) >> 8] = (vx - vy) % 256

                        # if we underflow, then set the VF register to 0
                        if vx > vy:
                            self.registers[0xf] = 1
                        else:
                            self.registers[0xf] = 0

                    case 0x0006:
                        # shift one bit to the right
                        # furthermore, implements original CHIP-8 for COSMIC VIP instruction, first
                        # setting VX to the value at VY
                        
                        # get the bit to be shifted out of vx (last bit)
                        if vy % 2 == 0:
                            # then last bit is 0
                            self.registers[0xf] = 0
                        else:
                            # then last bit is 1
                            self.registers[0xf] = 1

                        # shift vx (= vy) to the right
                        self.registers[(instruction & 0x0f00) >> 8] = vy >> 1

                    case 0x0007:
                        # vx = vy - vx 
                        self.registers[(instruction & 0x0f00) >> 8] = (vy - vx) % 256

                        # if we underflow, then set the VF register to 0
                        if vy > vx:
                            self.registers[0xf] = 1
                        else:
                            self.registers[0xf] = 0
                    case 0x000e:
                        # shift one bit to the left  
                        # furthermore, implements original CHIP-8 for COSMIC VIP instruction, first
                        # setting VX to the value at VY

                        # get the bit to be shifted out of vx (first bit), and we know this register is
                        # 8 bits
                        if (vy & 0x80) >> 7 == 0:
                            # then last bit is 0
                            self.registers[0xf] = 0
                        else:
                            # then last bit is 1
                            self.registers[0xf] = 1

                        # shift vx (= vy) to the right (crop to 8 bits)
                        self.registers[(instruction & 0x0f00) >> 8] = (vy << 1) & 0xff
            case 0x9000:
                # skip one 2 byte instruction if VX and VY are NOT equal in 9XY0 
                vx = self.registers[(instruction & 0x0f00) >> 8]
                vy = self.registers[(instruction & 0x00f0) >> 4]

                if vx != vy:
                    self._pc += 2
            case 0xa000:
                # set the index register to NNN, where instruction is 0xANNN  
                self.i_register = instruction & 0x0fff
            case 0xb000:
                # instruction BNNN - jump with offset, jump to address NNN with offset stored in v0 
                v0 = self.registers[0]
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

