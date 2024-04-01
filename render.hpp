#pragma once

#include <iostream>

#include <SDL.h>

struct RGBA_t {
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t A;

	bool operator==(const RGBA_t& other) const {
		return (
			(R == other.R) &&
			(G == other.G) &&
			(B == other.B) 
		);
	}
	bool operator!=(const RGBA_t& other) const {
		return !(*this == other);
	}

	operator uint32_t() const {
		// "erm you should actually use SDL_MapRGB() and SDL_GetRGB()"
		// shut up nerd
		// ASSUMING PIXELS ARE STORED IN ARGB FORMAT
		return 
			  ((A&0xff) << 24)
			| ((R&0xff) << 16)
			| ((G&0xff) << 8 )
			| ((B&0xff)      );
	}
};

namespace Colors {
	static RGBA_t white   = {255,255,255,255};
	static RGBA_t black   = {0  ,0  ,0  ,255};
	static RGBA_t red     = {255,0  ,0  ,255};
	static RGBA_t green   = {0  ,255,0  ,255};
	static RGBA_t blue    = {0  ,0  ,255,255};
	static RGBA_t orange  = {255,127,0  ,255};
	static RGBA_t yellow  = {255,255,0  ,255};
	static RGBA_t purple  = {127,0  ,255,255};
	static RGBA_t magenta = {255,0  ,255,255};
	static RGBA_t teal    = {0  ,255,255,255};
	static RGBA_t brown   = {127,63 ,0  ,255};
}


class Renderer {
public:
	// yeah i stole these two functions

	uint32_t getPixel(SDL_Surface* surface, uint32_t x, uint32_t y) {
		//Convert the pixels to 32 bit
		uint32_t* pixels = (uint32_t*)surface->pixels;
		//Get the requested pixel
		return pixels[(y*surface->w) + x];
	}
	void putPixel(SDL_Surface* surface, uint32_t x, uint32_t y, uint32_t pixel) {
		//Convert the pixels to 32 bit
		uint32_t* pixels = (uint32_t*)surface->pixels;
		//Set the pixel
		pixels[(y*surface->w) + x] = pixel;
	}

	template <uint32_t width, uint32_t height>
	void putPixelBlock(SDL_Surface* surface, RGBA_t array[height][width]) {
		// convert the pixels to 32 bit
		uint32_t* pixels = (uint32_t*)surface->pixels;
		// set all pixels
		for (uint32_t y = 0; y < height; ++y) {
			for (uint32_t x = 0; x < width; ++x) {
				pixels[(y*surface->w) + x] = (uint32_t)array[y][x];
			}
		}
	}
};