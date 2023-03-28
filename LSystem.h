#pragma once

#include "ltree.h"
#include "Grammar.h"

const u32 MAX_GRAMMAR = 1000000;
const f32 GUTTER = 10;

struct PenState {
	f32 angle;
	f32 x;
	f32 y;
};

class LSystem {
	std::unique_ptr<Grammar> _grammar;
	std::set<char> _drawVars;
	f32 _lineLength_x = 0.5;
	f32 _lineLength_y = 0.5;
	f32 _turnAngle;
	f32 _maxDepth = 1;

	void parseGrammar(const std::function <void (f32, f32, f32, f32, f32)> &f);
public:
	LSystem();

	void readFile(const char *fileName);

	u32 wordLength();
	void iterate();
	void draw(SDL_Renderer *renderer, u32 x1, u32 y1, u32 x2, u32 y2);
};
