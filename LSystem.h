#pragma once

#include "ltree.h"

const u32 MAX_ITERS = 5;

struct Segment {
	f32 x;
	f32 y;
	f32 length;
	f32 angle;
};

class LSystem {
	std::vector<std::unique_ptr<Segment>> _segments;

	u32 iterations;
public:
	LSystem();

	void divideSegments();

	void draw(SDL_Renderer *renderer, u32 x1, u32 y1, u32 x2, u32 y2);
};
