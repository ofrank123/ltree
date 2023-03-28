#include "Grammar.h"

Grammar::Grammar() {
	// Reseed
	_rng = std::mt19937{ _rd() };
}

void Grammar::addVar(char var, std::string sub) {
	_vars[var] = sub;
}


boolean Grammar::roll() {
	std::uniform_int_distribution<i32> dist(0, 99);
	return dist(_rng) > 33;
}

void Grammar::reduce() {
	std::stringstream newWordStream;

	for (char& c : word) {
		auto search = _vars.find(c);
		if (search != _vars.end() && (!useRandom || roll())) {
			newWordStream << search->second;
		} else {
			newWordStream << c;
		}
	}

	word = newWordStream.str();
}
