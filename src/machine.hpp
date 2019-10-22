#include <cstdint>
#include <cstdlib>
#include "chip8.hpp"
#include "display.hpp"

#pragma once

/**
	chip8Machine class. Implements the CHIP-8 VM functionality, other than the actual operations.
*/
struct chip8Machine{
	chip8 *state;

	Display *display;

	chip8Machine(const char *filename);

	~chip8Machine();

/**
	Reads a ROM file to the chip8 memory.
	@param filename: filename
*/
	void read_to_memory(const char *filename);

/**
	Runs an infinite loop of the program.
*/
	void run();
};