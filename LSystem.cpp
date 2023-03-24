#include "LSystem.h"

LSystem::LSystem() {
	std::unique_ptr<Segment> initial_segment(new Segment);
	initial_segment->x = 0.5;
	initial_segment->y = 0.0;
	initial_segment->length = 0.1;
	initial_segment->angle = 0.25 * PI;

	_segments.push_back(std::move(initial_segment));
}

void LSystem::divideSegments() {
	std::vector<std::unique_ptr<Segment>> newSegments;

	newSegments.resize(_segments.size());

}

void LSystem::draw(SDL_Renderer *renderer, u32 x1, u32 y1, u32 x2, u32 y2) {
	if (x1 >= x2 || y1 >= y2) {
		throw std::runtime_error{ "Invalid box to draw system in!" };
	}
	f32 xsize = x2 - x1;
	f32 ysize = y2 - y1;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (auto& segment : _segments) {
		f32 seg_x1 = x1 + xsize * segment->x;
		f32 seg_y1 = y1 + ysize * segment->y;
		SDL_RenderDrawLine(renderer,
						   seg_x1,
						   seg_y1,
						   seg_x1 + segment->length * std::cos(segment->angle) * xsize,
						   seg_y1 + segment->length * std::sin(segment->angle) * ysize);
	}
}
