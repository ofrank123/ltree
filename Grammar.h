#pragma once

#include "ltree.h"

class Grammar {
	std::unordered_map<char, std::string> _vars;

	// RNG
	std::random_device _rd;
	std::mt19937 _rng;

	boolean roll();
public:
	boolean useRandom = false;
	std::string word;

	Grammar();

	void addVar(char var, std::string sub);
	void reduce();
};
