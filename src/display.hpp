#include <SDL2/SDL.h>
#include <cstdint>

#pragma once

/**
	Display class. Wraps the SDL2 interactions.
*/
struct Display{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *sdlTexture = NULL;

	uint32_t pixels[2048];

	Display();

/**
	Renders a single frame based on the gfx array.
	@param gfx: chip8 video memory array.
*/
	void render_frame(uint8_t *gfx);
};