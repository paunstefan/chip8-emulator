#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "chip8.hpp"

#define PRINTOP 1

/**
	Executes a single chip8 instruction, increments PC by 2 (or 4, depending on the instruction).
	@param state: chip8 state
*/
void execute_instruction(chip8* state){
	// instructions span 2 bytes
	uint16_t opcode = (state->memory[state->pc] << 8) | state->memory[state->pc + 1];

	#if PRINTOP
	
		printf("0x%04x %04x\n", state->pc, opcode);
		for(int i = 0; i < 16; i++){
			printf("%d ", state->V[i]);
		}
		printf("I: %d  PC: %d\n", state->I, state->pc);

		//getchar();
	#endif

	state->pc += 2;

	switch(opcode >> 12){
		case 0x00:
			switch(opcode & 0xFF){
				case 0x00:
					// NOP
					break;
				case 0xE0:
					// CLS
					for(int i = 0; i < 2048; i++){
						state->gfx[i] = 0;
					}
					state->draw = true;
					break;
				case 0xEE:
					// RET
					state->pc = pop_stack(state);
					break;
			}
			break;
		case 0x01:
			// JMP
			state->pc = opcode & 0x0FFF;
			break;

		case 0x02:
			// CALL
			push_stack(state, state->pc);
			state->pc = opcode & 0x0FFF;
			break;

		case 0x03:
			// SKEQ
			if(state->V[(opcode >> 8) & 0x0F] == (opcode & 0xFF)){
				state->pc += 2;
			}
			break;

		case 0x04:
			// SKNE
			if(state->V[(opcode >> 8) & 0x0F] != (opcode & 0xFF)){
				state->pc += 2;
			}
			break;

		case 0x05:
			// SKEQ V, V
			if(state->V[(opcode >> 8) & 0x0F] == state->V[(opcode >> 4) & 0x0F]){
				state->pc += 2;
			}
			break;

		case 0x06:
			//MOV V, imm
			state->V[(opcode >> 8) & 0x0F] = opcode & 0xFF;
			break;

		case 0x07:
			// ADD V, imm
			state->V[(opcode >> 8) & 0x0F] += opcode & 0xFF;
			break;

		case 0x08:
		{
			switch(opcode & 0x0F){
				case 0x00:
					// MOV V, V
					state->V[(opcode >> 8) & 0x0F] = state->V[(opcode >> 4) & 0x0F];
					break;
				case 0x01:
					// OR V, Vs
					state->V[(opcode >> 8) & 0x0F] = state->V[(opcode >> 4) & 0x0F] | state->V[(opcode >> 8) & 0x0F];
					break;
				case 0x02:
					// AND V, V
					state->V[(opcode >> 8) & 0x0F] = state->V[(opcode >> 4) & 0x0F] & state->V[(opcode >> 8) & 0x0F];
					break;
				case 0x03:
					// XOR V, V
					state->V[(opcode >> 8) & 0x0F] = state->V[(opcode >> 4) & 0x0F] ^ state->V[(opcode >> 8) & 0x0F];
					break;
				case 0x04:
					// ADD V, V
					state->V[0xF] = 0;
					if(state->V[(opcode >> 8) & 0x0F] + state->V[(opcode >> 4) & 0x0F] > 0xFF){
						state->V[0xF] = 1;
					}
					state->V[(opcode >> 8) & 0x0F] = state->V[(opcode >> 8) & 0x0F] + state->V[(opcode >> 4) & 0x0F];
					break;
				case 0x05:
					// SUB V, V
					state->V[0xF] = 1;
					if(state->V[(opcode >> 4) & 0x0F] > state->V[(opcode >> 8) & 0x0F]){
						state->V[0xF] = 0;
					}
					state->V[(opcode >> 8) & 0x0F] = state->V[(opcode >> 8) & 0x0F] - state->V[(opcode >> 4) & 0x0F];
					break;
			}
		}
			break;

		case 0x09:
			// SKNE V, V
			if(state->V[(opcode >> 8) & 0x0F] != state->V[(opcode >> 4) & 0x0F]){
				state->pc += 2;
			}
			break;

		case 0x0A:
			// MVI
			state->I = opcode & 0x0FFF;
			break;

		case 0x0B:
			// JMI
			state->pc = (opcode & 0x0FFF) + state->V[0];
			break;

		case 0x0C:
			// RAND
			state->V[(opcode >> 8) & 0x0F] = (rand() % (0xFF+1)) & (opcode & 0xFF);
			break;

		case 0x0D:
			// SPRITE
		{
			uint8_t x = state->V[(opcode >> 8) & 0x0F];
			uint8_t y = state->V[(opcode >> 4) & 0x0F];
			uint8_t n = opcode & 0x0F;
			uint8_t pixel;

			state->V[0xF] = 0;
			for(int yl = 0; yl < n; yl++){
				pixel = state->memory[state->I + yl];
				for(int xl = 0; xl < 8; xl++){
					if((pixel & (0x80 >> xl)) != 0){
						// detect collisions
						if(state->gfx[x + xl + ((y + yl) * 64)] == 1){
							state->V[0xF] = 1;
						}

						state->gfx[x + xl + ((y + yl) * 64)] ^= 1;
					}
				}
			}

			state->draw = true;
		}
			break;

		case 0x0E:
		{
			switch(opcode & 0xFF){
				case 0x9E:
					// SKPR
					if(state->key[state->V[(opcode >> 8) & 0x0F]] != 0){
						state->pc += 2;
					}
					break;
				case 0xA1:
					// SKUP
					if(state->key[state->V[(opcode >> 8) & 0x0F]] == 0){
						state->pc += 2;
					}
					break;
			}
		}
			break;

		case 0x0F:
		{
			switch(opcode & 0xFF){
				case 0x00:
					// STOP
					break;
				case 0x07:
					//GDELAY
					state->V[(opcode >> 8) & 0x0F] = state->delay_timer;
					break;
				case 0x0A:
					// KEY
					{
					bool key_pressed = false;

					
					for(int i = 0; i < 16; i++){
						if(state->key[i] != 0){
							state->V[(opcode >> 8) & 0x0F] = i;
							key_pressed = true;
						}
					}

					if(!key_pressed){
						state->pc -= 2;
						return;
					}
					

					}
					break;
				case 0x15:
					// SDELAY
					state->delay_timer = state->V[(opcode >> 8) & 0x0F];
					break;
				case 0x17:
					// SPITCH
					break;
				case 0x18:
					// STONE
					state->sound_timer = state->V[(opcode >> 8) & 0x0F];
					break;
				case 0x1E:
					// ADI
					state->V[0xF] = 0;
					if(state->I + state->V[(opcode >> 8) & 0x0F] > 0xFFF){
						state->V[0xF] = 1;
					}
					state->I += state->V[(opcode >> 8) & 0x0F];
					break;
				case 0x29:
					// FONT
					state->I = state->V[(opcode >> 8) & 0x0F] * 5;
					break;
				case 0x33:
					// BCD
					state->memory[state->I] = state->V[(opcode >> 8) & 0x0F] / 100;
					state->memory[state->I + 1] = (state->V[(opcode >> 8) & 0x0F] / 10) % 10;
					state->memory[state->I] = state->V[(opcode >> 8) & 0x0F] % 10;
					break;
				case 0x55:
					// STR
					for(int i = 0; i <= ((opcode >> 8) & 0x0F); i++){
						state->memory[state->I++] = state->V[i];
					}
					break;
				case 0x65:
					// LDR
					for(int i = 0; i <= ((opcode >> 8) & 0x0F); i++){
						state->V[i] = state->memory[state->I++];
					}
					break;
				case 0x70:
					// OUT
					break;
				case 0x71:
					// IN
					break;
				case 0x72:
					// BAUD
					break;

			}
		}
			break;
	}

	if(state->delay_timer > 0){
		state->delay_timer--;
	}
	if(state->sound_timer > 0){
		//BEEP
		state->sound_timer--;
	}
}

/**
	Pushes a value on the chip8 stack.
	@param state: chip8 state
	@param value: value to store
*/
void push_stack(chip8* state, uint16_t value){
	state->stack[state->sp++] = value;
}

/**
	Pops a value from the stack.
	@param state: chip8 state
	@return value poped
*/
uint16_t pop_stack(chip8* state){
	return state->stack[--state->sp];
}