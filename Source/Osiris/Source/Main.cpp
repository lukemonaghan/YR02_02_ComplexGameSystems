#include "Game.hpp"

int main(int argc, char* argv[]) {

	Osiris::App *app = new Osiris::Game();

	if (app->Start(argc, argv) == true)
		app->Run();
	app->Quit();

	delete app;

	return 0;
}

