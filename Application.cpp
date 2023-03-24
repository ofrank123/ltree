#include "Application.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Application::Application() {
	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		throw std::runtime_error{ "Failed to initialize SDL!" };
	}
	_window = SDL_CreateWindow(
		"LTree",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
	if (!_window) {
		throw std::runtime_error{ "Failed to create window! "  };
	}
	_renderer = SDL_CreateRenderer(_window, -1, 0);
	if (!_renderer) {
		throw std::runtime_error{ "Failed to create renderer! "};
	}
}

void Application::run() {

	// Render loop
	SDL_Event e;
	bool quit = false;
	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		SDL_RenderClear(_renderer);

		system.draw(_renderer, 0, 0, 500, 500);

		SDL_RenderPresent(_renderer);
	}
}

Application::~Application() {
	SDL_DestroyRenderer( _renderer );
	SDL_DestroyWindow( _window );
	SDL_Quit();
}
