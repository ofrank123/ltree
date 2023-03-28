#include "LSystem.h"

LSystem::LSystem() {
	_grammar = std::make_unique<Grammar>();
	readFile("plant.txt");
}

void nextLine(std::ifstream& fileStream, std::string& line) {
	if (!std::getline(fileStream, line) || line.length() == 0) {
		throw std::runtime_error{ "Failed to parse file" };
	}
}

void LSystem::readFile(const char *fileName) {
	std::string line;
	std::ifstream fileStream(fileName);
	if (!fileStream) {
		throw std::runtime_error{ "File not found" };
	}

	std::unique_ptr<Grammar> newGrammar = std::make_unique<Grammar>();
	std::set<char> newDrawVars;

	// Check if random
	nextLine(fileStream, line);
	if (line == "r") {
		newGrammar->useRandom = true;
		nextLine(fileStream, line);
	}

	// Read angle
	f32 angle;
	try {
		angle = PI / 180.0 * std::stof(line);
	} catch (std::exception &err) {
		throw std::runtime_error{ "Failed to parse file" };
	}

	// Read draw vars
	nextLine(fileStream, line);
	for (char c : line) {
		newDrawVars.insert(c);
	}

	// Read starting word
	nextLine(fileStream, line);
	newGrammar->word = line;

	// Read rules
	while (std::getline(fileStream, line)) {
		if (line.length() >= 4) {
			newGrammar->addVar(line[0], line.substr(3).c_str());
		}
	}

	// Set vars
	_grammar = std::move(newGrammar);
	_drawVars = newDrawVars;
	_turnAngle = angle;
}

u32 LSystem::wordLength() {
	return _grammar->word.size();
}

void LSystem::parseGrammar(const std::function <void (f32, f32, f32, f32, f32)> &f) {
	std::vector<PenState> states;
	PenState state = {
		0.0,
		0.5,
		0.5,
	};
	for (char& c : _grammar->word) {
		switch (c) {
			case '+': // Turn right
				state.angle += _turnAngle;
				break;
			case '-': // Turn left
				state.angle -= _turnAngle;
				break;
			case '[': // Push stack
				states.push_back(state);
				break;
			case ']':
				if (states.empty()) {
					throw std::runtime_error { "Stack empty! "};
				}

				state = states.back();
				states.pop_back();
				break;
			default:
				auto search = _drawVars.find(c);
				if (search != _drawVars.end()) {
					f32 inc_x = std::sin(state.angle) * _lineLength_x;
					f32 inc_y = std::cos(state.angle) * _lineLength_y;

					f(state.x,
					  state.y,
					  inc_x,
					  inc_y,
					  states.size());

					state.x += inc_x;
					state.y += inc_y;
				}
		}
	}
}

void LSystem::iterate() {
	if (_grammar->word.length() < MAX_GRAMMAR) {
		_grammar->reduce();

		f32 maxLength_x = 0.000001;
		f32 maxLength_y = 0.000001;

		f32 max_x = 10000000000;
		f32 min_x = 10000000000;
		f32 max_y = 10000000000;
		f32 min_y = 10000000000;

		parseGrammar([&](f32 x, f32 y, f32 inc_x, f32 inc_y, f32 depth) {
			f32 cMaxLength_y = std::max<f32>(std::abs(0.5 - y), std::abs(0.5 - (y + inc_y)));
			if (cMaxLength_y > maxLength_y) {
				maxLength_y = cMaxLength_y;
			}

			f32 cMaxLength_x = std::max<f32>(std::abs(0.5 - x), std::abs(0.5 - (x + inc_x)));
			if (cMaxLength_x > maxLength_x) {
				maxLength_x = cMaxLength_x;
			}

			if (depth > _maxDepth) {
				_maxDepth = depth;
			}
		});

		_lineLength_x /= maxLength_x * 2;
		_lineLength_y /= maxLength_y * 2;
	}
}

void LSystem::draw(SDL_Renderer *renderer, u32 x1, u32 y1, u32 x2, u32 y2) {
	if (x1 >= x2 || y1 >= y2) {
		throw std::runtime_error{ "Invalid box to draw system in!" };
	}

	f32 size_x = x2 - x1 - (GUTTER * 2);
	f32 size_y = (y2 - y1 - (GUTTER * 2));

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	parseGrammar([=](f32 x, f32 y, f32 inc_x, f32 inc_y, f32 depth) {
		SDL_SetRenderDrawColor(renderer, (depth / _maxDepth) * 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer,
						   GUTTER + x1 + (x * size_x),
						   GUTTER + y1 + (size_y - (y * size_y)),
						   GUTTER + x1 + ((x + inc_x) * size_x ),
						   GUTTER + y1 + (size_y - ((y + inc_y) * size_y)));
	});
}

