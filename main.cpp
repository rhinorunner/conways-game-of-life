/*
* Created by Zaine Rehman
* 3/24/2024 - 4/1/2024
* 
* the game of life!
* 
* 
* CONTROLS (buttons are on keypad):
* 
* [left mouse] - place/remove pixel
* [right mouse] - drag place/remove pixel
* 
* [/] - increase frame delay
* [*] - decrease frame delay
* [.] - set frame delay to 0
* 
* [-] - fill screen with random pixels, 50% filled
* [+] - fill screen with random pixels, 25% filled
* 
* [1] - small pixel size
* [2] - medium pixel size
* [3] - large pixel size
* 
* [4] - standard game logic
* [5] - game logic tweaked for more growth
* [6] - game logic tweaked for less growth
* 
* [7] - black and white colors
* [8] - color scheme 1
* [9] - color scheme 2
* 
* [0] - clear screen
* 
* [enter] - pause
* 
* 
* 
* yes, its supposed to have threading. 
* no, there is not threading. 
*/

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>

#include <SDL.h>
#undef main

#include "render.hpp"

#define loop(x) for (uint32_t i = 0; i < x; ++i)

constexpr uint32_t SCREENWIDTH = 1000;
constexpr uint32_t SCREENHEIGHT = 1000;
uint16_t SCALEFACTOR = 1; // pixels per cell = scaleFactor^2, width and height must be divisible by this
int32_t DELAY = 100; // ms
uint16_t THREADS = 1;
uint8_t COLORMODE = 1;
uint8_t GAMELOGIC = 0; // 0 = cgol

std::vector<std::vector<bool>> PIXELS (SCREENHEIGHT/SCALEFACTOR,std::vector<bool>(SCREENWIDTH/SCALEFACTOR,false));

bool QUIT = false;
bool EDITING = true;
SDL_Surface* SURFACE;
SDL_Window* WINDOW;
SDL_Event event;
Renderer renderer;
RGBA_t display[SCREENHEIGHT][SCREENWIDTH];
int mouseX, mouseY;
uint8_t rightMode = 0;

/*
	1 - 3rd color mode
	2 - fill screen with random pixels
	3 - alternate game logic
4 - make mouse work in non edit mode??
*/

void getNewPixels(
	const std::vector<std::vector<bool>>& oldPixels, 
	std::vector<std::vector<bool>>& returnTo, 
	uint32_t from, 
	uint32_t to
) {
	std::vector<std::vector<bool>> newPixels (SCREENHEIGHT/SCALEFACTOR,std::vector<bool>(SCREENWIDTH/SCALEFACTOR,false));
	//std::cout << "going vertically from " << from << " to " << to << '\n';

	for (uint32_t i = from; i < to; ++i) {
		for (uint32_t x = 0; x < SCREENWIDTH/SCALEFACTOR; ++x) {

			switch (GAMELOGIC) {
			// traditional cgol
			case 0: {
				uint8_t neighborsAlive = 0;
				if (i > 0) {
					if ((x > 0) && (oldPixels[i-1][x-1])) neighborsAlive++;
					if (oldPixels[i-1][x]) neighborsAlive++;
					if ((x < SCREENWIDTH/SCALEFACTOR-1) && (oldPixels[i-1][x+1])) neighborsAlive++;
				}
				if (i < SCREENHEIGHT/SCALEFACTOR-1) {
					if ((x > 0) && (oldPixels[i+1][x-1])) neighborsAlive++;
					if (oldPixels[i+1][x]) neighborsAlive++;
					if ((x < SCREENWIDTH/SCALEFACTOR-1) && (oldPixels[i+1][x+1])) neighborsAlive++;
				}
				if ((x > 0) && (oldPixels[i][x-1])) neighborsAlive++;
				if ((x < SCREENWIDTH/SCALEFACTOR-1) && (oldPixels[i][x+1])) neighborsAlive++;

				newPixels[i][x] = true;
				if (oldPixels[i][x] && !((neighborsAlive == 2) || (neighborsAlive == 3))) newPixels[i][x] = false;
				else if (!oldPixels[i][x] && !(neighborsAlive == 3)) newPixels[i][x] = false;
				break;
			}
			// pixels become alive from dead at 2 or 3 neighbors
			// but alive pixels only survive at 2
			case 1: {
				uint8_t neighborsAlive = 0;
				if (i > 0) {
					if ((x > 0) && (oldPixels[i-1][x-1])) neighborsAlive++;
					if (oldPixels[i-1][x]) neighborsAlive++;
					if ((x < SCREENWIDTH/SCALEFACTOR-1) && (oldPixels[i-1][x+1])) neighborsAlive++;
				}
				if (i < SCREENHEIGHT/SCALEFACTOR-1) {
					if ((x > 0) && (oldPixels[i+1][x-1])) neighborsAlive++;
					if (oldPixels[i+1][x]) neighborsAlive++;
					if ((x < SCREENWIDTH/SCALEFACTOR-1) && (oldPixels[i+1][x+1])) neighborsAlive++;
				}
				if ((x > 0) && (oldPixels[i][x-1])) neighborsAlive++;
				if ((x < SCREENWIDTH/SCALEFACTOR-1) && (oldPixels[i][x+1])) neighborsAlive++;

				newPixels[i][x] = true;
				if (oldPixels[i][x] && !(neighborsAlive == 3)) newPixels[i][x] = false;
				else if (!oldPixels[i][x] && !((neighborsAlive == 2) || (neighborsAlive == 3))) newPixels[i][x] = false;
				break;
			}
			// only stay alive at 3 pixels
			case 2: {
				uint8_t neighborsAlive = 0;
				if (i > 0) {
					if ((x > 0) && (oldPixels[i-1][x-1])) neighborsAlive++;
					if (oldPixels[i-1][x]) neighborsAlive++;
					if ((x < SCREENWIDTH/SCALEFACTOR-1) && (oldPixels[i-1][x+1])) neighborsAlive++;
				}
				if (i < SCREENHEIGHT/SCALEFACTOR-1) {
					if ((x > 0) && (oldPixels[i+1][x-1])) neighborsAlive++;
					if (oldPixels[i+1][x]) neighborsAlive++;
					if ((x < SCREENWIDTH/SCALEFACTOR-1) && (oldPixels[i+1][x+1])) neighborsAlive++;
				}
				if ((x > 0) && (oldPixels[i][x-1])) neighborsAlive++;
				if ((x < SCREENWIDTH/SCALEFACTOR-1) && (oldPixels[i][x+1])) neighborsAlive++;

				newPixels[i][x] = true;
				if (oldPixels[i][x] && !(neighborsAlive == 3)) newPixels[i][x] = false;
				else if (!oldPixels[i][x] && !(neighborsAlive == 3)) newPixels[i][x] = false;
				break;
			}
			}

			for (uint16_t y = 0; y < SCALEFACTOR; ++y) {
				for (uint16_t z = 0; z < SCALEFACTOR; ++z) {
					RGBA_t yes, no;
					switch (COLORMODE) {
					case 1:
						yes = RGBA_t{
							uint8_t(i*255/(SCREENHEIGHT/SCALEFACTOR)),
							200,
							uint8_t(x*255/(SCREENWIDTH/SCALEFACTOR)),
							255
						};
						no = Colors::black;
						break;
					case 2:
						yes = RGBA_t{
							160,
							uint8_t(x*255/(SCREENWIDTH/SCALEFACTOR)),
							uint8_t(i*255/(SCREENHEIGHT/SCALEFACTOR)),
							255
						};
						no = Colors::black;
						break;
					default:
						yes = Colors::white;
						no = Colors::black;
						break;
					}
					display[i*SCALEFACTOR+y][x*SCALEFACTOR+z] = newPixels[i][x] ? yes : no;
				}
			}
		}
	}
	returnTo = newPixels;
}

int main() {
	srand(time(0));
	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		// honestly no idea why you would even get here
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}
	WINDOW = SDL_CreateWindow("the game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, NULL);
	if (WINDOW == NULL) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}
	SURFACE = SDL_GetWindowSurface(WINDOW);

	while (!QUIT) {
		SDL_PollEvent(&event);
		
		switch (event.type) {
		case SDL_QUIT:
			QUIT = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				QUIT = true;
				break;

			case SDLK_KP_MULTIPLY:
				DELAY -= 10;
				if (DELAY < 0) DELAY = 0;
				break;
			case SDLK_KP_DIVIDE:
				DELAY += 10;
				break;
			case SDLK_KP_ENTER:
				EDITING = !EDITING;
				break;
			case SDLK_KP_PERIOD:
				DELAY = 0;
				break;

			case SDLK_KP_7:
				COLORMODE = 0;
				break;
			case SDLK_KP_8:
				COLORMODE = 1;
				break;
			case SDLK_KP_9:
				COLORMODE = 2;
				break;

			case SDLK_KP_4:
				GAMELOGIC = 0;
				break;
			case SDLK_KP_5:
				GAMELOGIC = 1;
				break;
			case SDLK_KP_6:
				GAMELOGIC = 2;
				break;

			case SDLK_KP_MINUS: // random 50/50
				for (uint32_t i = 0; i < SCREENHEIGHT/SCALEFACTOR; ++i) 
					for (uint32_t x = 0; x < SCREENWIDTH/SCALEFACTOR; ++x)
						PIXELS[i][x] = rand() % 2;
				break;
			case SDLK_KP_PLUS: // random 25/75
				for (uint32_t i = 0; i < SCREENHEIGHT/SCALEFACTOR; ++i) 
					for (uint32_t x = 0; x < SCREENWIDTH/SCALEFACTOR; ++x)
						PIXELS[i][x] = rand() % 4 == 0 ? 1 : 0;
				break;

			case SDLK_KP_0: {
				std::vector<std::vector<bool>> gone (SCREENHEIGHT/SCALEFACTOR,std::vector<bool>(SCREENWIDTH/SCALEFACTOR,false));
				PIXELS = gone;
				break;
			}
			case SDLK_KP_1: {
				SCALEFACTOR = 1;
				std::vector<std::vector<bool>> sf1 (SCREENHEIGHT/SCALEFACTOR,std::vector<bool>(SCREENWIDTH/SCALEFACTOR,false));
				PIXELS = sf1;
				break;
			}
			case SDLK_KP_2: {
				SCALEFACTOR = 4;
				std::vector<std::vector<bool>> sf2 (SCREENHEIGHT/SCALEFACTOR,std::vector<bool>(SCREENWIDTH/SCALEFACTOR,false));
				PIXELS = sf2;
				break;
			}
			case SDLK_KP_3: {
				SCALEFACTOR = 8;
				std::vector<std::vector<bool>> sf3 (SCREENHEIGHT/SCALEFACTOR,std::vector<bool>(SCREENWIDTH/SCALEFACTOR,false));
				PIXELS = sf3;
				break;
			}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&mouseX,&mouseY);
			if (((mouseX > 0) && (mouseX < SCREENWIDTH)) && ((mouseY > 0) && (mouseY < SCREENHEIGHT))) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					//SDL_GetGlobalMouseState(&mouseX,&mouseY); // interesting
					PIXELS[mouseY/SCALEFACTOR][mouseX/SCALEFACTOR] = !PIXELS[mouseY/SCALEFACTOR][mouseX/SCALEFACTOR];
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					if (PIXELS[mouseY/SCALEFACTOR][mouseX/SCALEFACTOR]) rightMode = 2; // removing
					else rightMode = 1; // adding
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_RIGHT) {
				rightMode = 0;
			}
			break;
		}
		if (rightMode == 1) {
			SDL_GetMouseState(&mouseX,&mouseY);
			if (((mouseX > 0) && (mouseX < SCREENWIDTH)) && ((mouseY > 0) && (mouseY < SCREENHEIGHT))) {
				PIXELS[mouseY/SCALEFACTOR][mouseX/SCALEFACTOR] = true;
			}
		}
		else if (rightMode == 2) {
			SDL_GetMouseState(&mouseX,&mouseY);
			if (((mouseX > 0) && (mouseX < SCREENWIDTH)) && ((mouseY > 0) && (mouseY < SCREENHEIGHT))) {
				PIXELS[mouseY/SCALEFACTOR][mouseX/SCALEFACTOR] = false;
			}
		}

		if (SDL_MUSTLOCK(SURFACE)) SDL_LockSurface(SURFACE);

		std::vector<std::vector<bool>> newPixels (SCREENHEIGHT/SCALEFACTOR,std::vector<bool>(SCREENWIDTH/SCALEFACTOR,false));
		std::vector<std::thread> threads {};
		
		if (!EDITING) {
			uint16_t overflow = 0;
			uint32_t blockSize = (SCREENHEIGHT/SCALEFACTOR)/THREADS;
			overflow = SCREENHEIGHT/SCALEFACTOR - blockSize*THREADS;
			loop(THREADS) {
				threads.push_back(std::thread(
					getNewPixels,
					PIXELS,
					std::ref(newPixels),
					blockSize*i,
					(i == THREADS-1 ? blockSize*(i+1)+overflow : blockSize*(i+1))
				));
			}
			loop(THREADS) threads[i].join();
			PIXELS = newPixels;
		}
		else {
			for (uint32_t i = 0; i < SCREENHEIGHT/SCALEFACTOR; ++i) {
				for (uint32_t x = 0; x < SCREENWIDTH/SCALEFACTOR; ++x) {
					for (uint16_t y = 0; y < SCALEFACTOR; ++y) {
						for (uint16_t z = 0; z < SCALEFACTOR; ++z) {
							RGBA_t yes, no;
							switch (COLORMODE) {
							case 1:
								yes = RGBA_t{
									uint8_t(i*255/(SCREENHEIGHT/SCALEFACTOR)),
									200,
									uint8_t(x*255/(SCREENWIDTH/SCALEFACTOR)),
									255
								};
								no = Colors::black;
								break;
							case 2:
								yes = RGBA_t{
									160,
									uint8_t(x*255/(SCREENWIDTH/SCALEFACTOR)),
									uint8_t(i*255/(SCREENHEIGHT/SCALEFACTOR)),
									255
								};
								no = Colors::black;
								break;
							default:
								yes = Colors::white;
								no = Colors::black;
								break;
							}
							display[i*SCALEFACTOR+y][x*SCALEFACTOR+z] = PIXELS[i][x] ? yes : no;
						}
					}
				}
			}
		}

		//auto start = std::chrono::high_resolution_clock::now();
		//auto stop = std::chrono::high_resolution_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::duration<double>> (stop-start).count() << '\n';

		renderer.putPixelBlock<SCREENWIDTH,SCREENHEIGHT>(SURFACE, display);
		
		if (SDL_MUSTLOCK(SURFACE)) SDL_UnlockSurface(SURFACE);
		
		SDL_UpdateWindowSurface(WINDOW);
		if (!EDITING) std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
	}

	SDL_DestroyWindow(WINDOW);
	SDL_Quit();
	
	return 0;
}