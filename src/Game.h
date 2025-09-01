#pragma once

#include <memory>

#include "Screen.h"
#include "World.h"
#include "Player.h"

class Game
{
public:
	static Game& get();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void init();
	void run();
	void finish();

	void showDeathScreen();

	void update();
	void render();
private:
	int mWidth, mHeight;
	std::unique_ptr<Screen> mScreen;
	std::shared_ptr<World> mWorld;
	std::unique_ptr<Player> mPlayer;

	Game();
};

