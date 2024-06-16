# Chip-8 Emulator for Python and C++
## Introduction
Chip-8 is an interpreted programming language, initially developed for the COSMAC VIP. It is commonly used a a beginner's project in hardware emulation, with the goal of emulating the behaviour of the Chip-8.

This repository contains 2 implementations of Chip-8, one in Python, one in C++.

## Installation + Running

### Python

Getting the project to run in Python is simple. From the root of the repository, ensure that you are in the chip8_python folder:

```bash
  cd chip8_python
```
Then download the requirements in requirements.txt, and run the main file, specifying the path to the ROM. Some ROMS are included in the ROMS folder.

```bash
  pip install -r requirements.txt
  python3 main.py <PATH_TO_ROM>
```

### C++

This project requires CMake, SDL2 and the SDL2-Mixer. These must be installed for the project to run.
These dependencies can be installed on Linux through this command:

```bash
  sudo apt-get install cmake libsdl2-dev libsdl2-mixer-dev
```

Navigate to the chip8_cpp folder, and create a build directory. CMake can be run inside this build folder.
```bash
  cd chip8_cpp
  mkdir build
  cd build
  cmake -S .. -B .
  make
```

Finally, the emulator can be run through:
```bash
  ./chip8emulator <PATH_TO_ROM>
```
