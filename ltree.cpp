#include "ltree.h"

#include "Grammar.cpp"
#include "LSystem.cpp"
#include "Application.cpp"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE pPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	try {
		Application app;
		app.run();
	}
	catch (const std::runtime_error& e) {
		printf("Exception: %s\n", e.what());
	}

	return 0;
}
