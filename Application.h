#pragma once

#include<SDL/SDL.h>
#include "LSystem.h"

class Application {
	SDL_Window *_window;
	SDL_Renderer *_renderer;

	LSystem system;
public:
	Application();
	~Application();

	void run();
};
