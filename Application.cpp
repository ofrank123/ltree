#include "Application.h"

Application::Application() {
	_system = std::make_unique<LSystem>();

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

	if ( TTF_Init() < 0 ) {
		throw std::runtime_error{ "Failed to initialize TTF! "};
	}

	_font = TTF_OpenFont("arial.ttf", 18);
	if ( !_font ) {
		throw std::runtime_error{ std::format("Failed to load font! SDL: {}", SDL_GetError()) };
	}
}

void Application::drawText(u32 x, u32 y, SDL_Color color, const char* str) {
	SDL_Rect dest;
	SDL_Surface *text_surf = TTF_RenderText_Blended(_font, str, color);
	SDL_Texture *text = SDL_CreateTextureFromSurface(_renderer, text_surf);

	dest.x = x;
	dest.y = y;
	dest.w = text_surf->w;
	dest.h = text_surf->h;
	SDL_RenderCopy(_renderer, text, NULL, &dest);

	SDL_DestroyTexture(text);
	SDL_FreeSurface(text_surf);
}

SDL_Rect clip_UI {
	0,
	0,
	(u32) SCREEN_WIDTH,
	(u32) 100
};

void Application::drawUI() {
	SDL_RenderSetClipRect(_renderer, &clip_UI);

	SDL_SetRenderDrawColor(_renderer, CLEAR_COLOR, 255);
	SDL_RenderFillRect(_renderer, &clip_UI);

	u32 y = 10;
	drawText(10, y, {0, 0, 0}, std::format("FPS: {:.0f}    Current Word Length: {}", _fps, _system->wordLength()).c_str());
	drawText(500, y, {0, 0, 0}, "Controls:");
	y += 20;
	drawText(500, y, {0, 0, 0}, "SPACE: Iterate        ENTER: Reload file (type to change filename)");
	drawText(10, y, {0, 0, 0}, std::format("Current File: {}", _fileText).c_str());
	y += 20;
	if (_errorText.length() > 0) {
		drawText(10, y, {227, 71, 43}, _errorText.c_str());
		y += 20;
	}
}

SDL_Rect clip_System {
	0,
	100,
	(u32) SCREEN_WIDTH,
	(u32) SCREEN_HEIGHT - 100
};


void Application::drawSystem() {
	SDL_RenderSetClipRect(_renderer, &clip_System);

	SDL_SetRenderDrawColor(_renderer, CLEAR_COLOR, 255);
	SDL_RenderFillRect(_renderer, &clip_System);

	_system->draw(_renderer, clip_System.x, clip_System.y, clip_System.w + clip_System.x, clip_System.h + clip_System.y);
}

u64 timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void Application::run() {

	// Render loop
	SDL_Event e;
	bool quit = false;
	bool regen = false;
	u64 lastFrameTime = timeSinceEpochMillisec();

	SDL_SetRenderDrawColor(_renderer, CLEAR_COLOR, 255);
	SDL_RenderClear(_renderer);

	SDL_SetWindowTitle(_window, "LTree");
	drawSystem();
	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym) {
						case SDLK_BACKSPACE:
							if (_fileText.length() > 0) _fileText.pop_back();
							break;
						case SDLK_SPACE:
							regen = true;
							break;
						case SDLK_RETURN:
							try {
								_system->readFile(_fileText.c_str());
								_errorText = "";
								drawSystem();
							} catch (const std::runtime_error& e) {
								_errorText = e.what();
							}
							break;
					}
					break;
				case SDL_TEXTINPUT:
					if (strcmp(e.text.text, " ") != 0) {
						_fileText += e.text.text;
					}
					break;
			}
		}

		u64 currentTime = timeSinceEpochMillisec();
		if (currentTime - lastFrameTime > 16) {
			_fps = 1000.0 / (currentTime - lastFrameTime);
			lastFrameTime = currentTime;

			drawUI();
			SDL_RenderPresent(_renderer);
		}

		if (regen) {
			_system->iterate();
			regen = false;
			drawSystem();
		}
	}
}

Application::~Application() {
	TTF_CloseFont( _font );

	SDL_DestroyRenderer( _renderer );
	SDL_DestroyWindow( _window );
	SDL_Quit();
}
