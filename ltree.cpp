#include "ltree.h"

#include "LSystem.cpp"
#include "Application.cpp"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE pPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	try {
		Application *app = new Application();
		app->run();
		delete app;
	} catch (const std::runtime_error& e) {
		printf("Exception: %s\n", e.what());
	}

	return 0;
}
