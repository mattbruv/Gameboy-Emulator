#include "display.h"

void Display::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window;

	window = SDL_CreateWindow(
		"Gameboy Emulator",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		400,
		200,
		SDL_WINDOW_RESIZABLE
	);

	if (window == NULL)
	{
		std::cout << "There was an error initializing the window" << std::endl << SDL_GetError();
	}

	SDL_Event event;
	bool running = true;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
				break;
			}
		}
	}
}