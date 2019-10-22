#include <cstdint>

#pragma once

/**
	chip8 struct. Keeps the chip8 vm state.
*/
typedef struct chip8{
	uint8_t memory[4096];
	uint8_t V[16];			// general purpose registers

	uint16_t I;				// index register
	uint16_t pc;			// program counter

	uint16_t stack[24];
	uint16_t sp;

	uint8_t gfx[64 * 32];	// video memory

	uint8_t delay_timer;
	uint8_t sound_timer;

	uint8_t key[16];		// keyboard state array (for the 16 keys)

	bool draw;
} chip8;

/**
	Executes a single chip8 instruction, increments PC by 2 (or 4, depending on the instruction).
	@param state: chip8 state
*/
void execute_instruction(chip8* state);

/**
	Pushes a value on the chip8 stack.
	@param state: chip8 state
	@param value: value to store
*/
void push_stack(chip8* state, uint16_t value);

/**
	Pops a value from the stack.
	@param state: chip8 state
	@return value poped
*/
uint16_t pop_stack(chip8* state);