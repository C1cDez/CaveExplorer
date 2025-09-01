#include <iostream>
#include <signal.h>

#include "Game.h"

void handle(int sig)
{
	Game::get().finish();
	exit(0);
}

int main()
{
	signal(SIGINT, handle);

	Game::get().init();
	Game::get().run();
	Game::get().finish();
}
