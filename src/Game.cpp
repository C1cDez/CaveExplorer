#include "Game.h"

Game& Game::get()
{
	static Game game{};
	return game;
}
Game::Game()
{
	mWidth = 120;
	mHeight = 30;
	mScreen = std::make_unique<Screen>(mWidth, mHeight);
	mWorld = std::make_shared<World>(mWidth, mHeight);
	mPlayer = std::make_unique<Player>(mWorld);
}
void Game::init()
{
	mWorld->generate();
	mWorld->findSpawnpoint(mPlayer);
}
void Game::run()
{
	while (1)
	{
		mScreen->clear();
		update();
		if (mPlayer->dead() || mPlayer->won()) break;
		render();
		mScreen->display();
		Sleep(16);
	}
	showDeathScreen();
}

void Game::showDeathScreen()
{
	mScreen->clear();

	mWorld->render(mScreen);

	int hy = mScreen->getHeight() / 2 - 1;
	for (int x = 0; x < mScreen->getWidth(); x++)
	{
		mScreen->pixel(x, hy, '-');
		mScreen->pixel(x, hy + 1, ' ');
		mScreen->pixel(x, hy + 2, ' ');
		mScreen->pixel(x, hy + 3, '-');
	}

	int cx = mScreen->getWidth() / 2;
	if (mPlayer->won())  //Win screen
	{
		char youCollected[24];
		sprintf_s(youCollected, 24, "You collected %3d coins", mPlayer->getMoney());
		mScreen->write(cx - 7 / 2, hy + 1, "YOU WON", 7);
		mScreen->write(cx - 23 / 2, hy + 2, youCollected, 23);
	}
	else
	{
		char gameInfo[43];
		sprintf_s(gameInfo, 43, "You collected %3d coins. And left %3d food", 
			mPlayer->getMoney(), mWorld->countTiles(Tile::FOOD));
		mScreen->write(cx - 8 / 2, hy + 1, "YOU DIED", 8);
		mScreen->write(cx - 42 / 2, hy + 2, gameInfo, 42);
	}

	mScreen->display();
	while ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000));
}
void Game::finish()
{

}

void Game::update()
{
	mPlayer->update();
}
void Game::render()
{
	mWorld->render(mScreen);
	mPlayer->render(mScreen);
}
