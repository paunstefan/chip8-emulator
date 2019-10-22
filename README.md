# chip8-emulator

CHIP-8 VM emulator

# Description

Emulator for the CHIP-8 interpreted language from the '70s. It was used to 
simplify the development of graphical applications, but it was very limited.

Specifications:
*  16 general purpose 8bit registers
*  One 16bit index register
*  4KB of memory (RAM+ROM)
*  16 key hexadecimal keyboard
*  64x32 monochrome display


# Usage

To install just download the project and run the Makefile using `make` in the directory with the files.

SDL2 is the only dependency.

# Sources

*  [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8)
*  [Instuction set](https://storage.googleapis.com/wzukusers/user-34724694/documents/5c76abc8dd207H5pAcyY/CHIP-8%20Instruction%20Set%20(Classic).pdf)
*  [How to write an emulator](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)