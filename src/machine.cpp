#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include "machine.hpp"
#include "display.hpp"

uint8_t chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

uint8_t keymap[16] = {
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_4,
	SDLK_q,
	SDLK_w,
	SDLK_e,
	SDLK_r,
	SDLK_a,
	SDLK_s,
	SDLK_d,
	SDLK_f,
	SDLK_z,
	SDLK_x,
	SDLK_c,
	SDLK_v,
};

chip8Machine::chip8Machine(const char *filename){
	state = new chip8();

	// initialize state
	state->sp = 0;
	state->pc = 0x200;		// program starts at address 0x200
	state->delay_timer = 0;
	state->sound_timer = 0;
	state->draw = false;
	state->I = 0;

	for(int i = 0; i < 16; i++){
		state->key[i] = 0;
		state->V[i] = 0;
	}

	for(int i = 0; i < 2048; i++){
		state->gfx[i] = 0;
	}

	for(int i = 0; i < 80; i++){
		state->memory[i] = chip8_fontset[i];
	}

	read_to_memory(filename);

	srand(time(NULL));

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		exit(1);
	}

	display = new Display();
}

chip8Machine::~chip8Machine(){
	delete state;
	delete display;
}

/**
	Reads a ROM file to the chip8 memory.
	@param filename: filename
*/
void chip8Machine::read_to_memory(const char *filename){
	FILE* f = fopen(filename, "rb");
	if(f == NULL){
		printf("ERROR: couldn't open file\n");
		exit(1);
	}

	fseek(f, 0, SEEK_END);
	uint32_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	fread(state->memory + 0x200, size, 1, f);
	fclose(f);
}

/**
	Runs an infinite loop of the program.
*/
void chip8Machine::run(){
	using namespace std::this_thread;
	using namespace std::chrono;

	while(true){

		SDL_Event event;

		if(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					SDL_DestroyWindow(this->display->window);
					SDL_Quit();
					exit(1);
				case SDL_KEYDOWN:
					{
					for(int i = 0; i < 16; ++i){
						if(event.key.keysym.sym == keymap[i]){
							state->key[i] = 1;
						}
					}

					}
					break;

				case SDL_KEYUP:
					{
					for(int i = 0; i < 16; ++i){
						if(event.key.keysym.sym == keymap[i]){
							state->key[i] = 0;
						}
					}

					}
					break;

			}
		}

		execute_instruction(state);

		if(state->draw){
			display->render_frame(state->gfx);
			state->draw = false;
		}
		if(state->sound_timer > 0){
			// BEEP 
		}

		sleep_for(milliseconds(8));
	}
}