#pragma once

#include<SDL/SDL.h>
#include "LSystem.h"

#define CLEAR_COLOR 255, 246, 212


const f32 SCREEN_WIDTH = 1280;
const f32 SCREEN_HEIGHT = 720;

class Application {
	SDL_Window *_window;
	SDL_Renderer *_renderer;
    TTF_Font *_font;
	f32 _fps;
	std::string _fileText = "plant.txt";
	std::string _errorText = "";

	std::unique_ptr<LSystem> _system;

	void drawText(u32 x, u32 y, SDL_Color color, const char *str);
	void drawSystem();
	void drawUI();
public:
	Application();
	~Application();

	void run();
};
