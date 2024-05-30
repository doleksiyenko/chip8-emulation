from typing import Tuple
import random
import pygame

from memory import Memory
from renderer import Renderer
from sound import Sound
from clock import Clock

"""
A class for handling the CPU operations for the CHIP-8
"""
class CPU:
    def __init__(self, memory: Memory, renderer: Renderer, sound: Sound) -> None:
        self._pc = 0x200 # program counter register, program always loads in at
                         # address 200
        self.i_register = 0x0 # the index register 
        self.registers = [0x0] * 16

        self._delay_timer = 0
        # create pointers to the rest of the hardware components
        self.memory = memory
        self.renderer = renderer 
        self.sound = sound

    def cycle(self) -> None:
        """
        The main fetch-decode-execute cycle of the CPU
        """
        # fetch the instruction at the current PC
        instruction = self.__fetch()
        # decode and execute the instruction
        self.__decode_execute(instruction)

    def decrement_timer(self) -> None:
        """
        Decrement the delay timer
        """
        self._delay_timer -= 1

    def __fetch(self) -> int:
        # chip 8 instuctions are two bytes long
        instruction_byte1 = self.memory.get_from_memory(self._pc)
        instruction_byte2 = self.memory.get_from_memory(self._pc + 1)

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
                self.registers[x] = (self.registers[x] + (instruction & 0x00ff)) % 256
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
                self._pc = self.registers[0] + instruction & 0x0fff
            case 0xc000:
                # generate a random number and bitwise and it with NN 
                rand_val = (instruction & 0x00ff) & (random.random() * (instruction & 0x00ff))
                self.registers[(instruction & 0x0f00) >> 8] = rand_val
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
                    sprite_byte = self.memory.get_from_memory(memory_address) 
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
                # e based instructions are skip-if key is being pressed
                # first, get the hexadecimal code for the key being stored in vx, 0-f
                instruction_key = (instruction & 0x0f00) >> 8 
                for event in pygame.event.get():
                    if event.type == pygame.KEYDOWN:
                        # get the key that the hexidecimal key corresponds to
                        if (instruction_key in self.memory.valid_keys):
                            # check the key being pressed is the key that vx holds
                            match instruction & 0x000f:
                                case 0x000e:
                                    # instruction skips if key is equal to the instruction key
                                    if event.key == self.memory.valid_keys[instruction_key]: 
                                        # skip the 2 byte instruction
                                        self._pc += 2 
                                case 0x0001:
                                    # instruction skips if key is NOT equal to the instruction key
                                    if event.key != self.memory.valid_keys[instruction_key]: 
                                        # skip the 2 byte instruction
                                        self._pc += 2 
            case 0xf000:
                
                vx = self.registers[(instruction & 0x0f00) >> 8]

                match instruction & 0x00ff:
                    case 0x0007:
                        # set vx register equal to the delay timer
                        self.registers[(instruction & 0x0f00) >> 8] = self._delay_timer
                    case 0x0015:
                        # set the delay timer to value in register
                        self._delay_timer = vx 
                    case 0x0018:
                        # set the sound timer to value in register
                        self.sound.set_timer(vx) 
                    case 0x001e:
                        # add vx to the index register (check for overflow from 0x0fff to 0x1000, set vf accordingly)
                        if (self.i_register + vx > 0x0fff):
                            self.registers[0xf] = 1
                        else:
                            self.register[0xf] = 0
                        self.i_register += vx
                    case 0x000a:
                        # a blocking instruction until a key is pressed.
                        for event in pygame.event.get():
                            if event.type == pygame.QUIT:
                                pygame.quit()
                                exit(0)
                            elif event.type == pygame.KEYUP:
                                # get the key that the hexidecimal key corresponds to
                                if (event.key in self.memory.key_hex):
                                    # set vx to the key being held
                                    self.registers[(instruction & 0x0f00) >> 8] = self.memory.key_hex[event.key] 
                                    # do not execute the else statement
                                    break
                        else:
                            # go back to this same instruction (block)
                            self._pc -= 2

                    case 0x0029:
                        # set index register to location of font of hexadecimal character in memory
                        # each font character is 5 bytes ()
                        self.i_register = self.memory.get_from_memory(5 * vx)

                    case 0x0033:
                        # binary coded decimal conversion (convert the number stored in vx to 3
                        # decimal digits)
                        self.memory.set_memory(loc=self.i_register + 0, val=(vx // 100) % 10)
                        self.memory.set_memory(loc=self.i_register + 1, val=(vx // 10) % 10)
                        self.memory.set_memory(loc=self.i_register + 2, val=(vx // 1) % 10)

                    case 0x0055:
                        registers = self.registers[0: ((instruction & 0x0f00) >> 8) + 1] 
                        for i, register in enumerate(registers):
                            self.memory.set_memory(loc=self.i_register + i, val=register)

                    case 0x0065:
                        x = (instruction & 0x0f00) >> 8
                        for i in range(x + 1):
                            self.registers[i] = self.memory.get_from_memory(loc=self.i_register + i)


                        
