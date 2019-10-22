#include <SDL2/SDL.h>
#include <cstdint>
#include <cstdlib>
#include "display.hpp"

const int WIDTH = 1024;
const int HEIGHT = 512;

Display::Display(){
	window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	if(window == NULL){
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

	sdlTexture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		64, 32);


}

/**
	Renders a single frame based on the gfx array.
	@param gfx: chip8 video memory array.
*/
void Display::render_frame(uint8_t *gfx){
	for(int i = 0; i < 2048; ++i){
		pixels[i] = (0x00FFFFFF * gfx[i]) | 0xFF000000;
	}

	SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(uint32_t));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}