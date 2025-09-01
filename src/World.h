#pragma once

#include <memory>
#include <random>

#include "Screen.h"

class Player;

enum Tile
{
	AIR = 0,
	STONE = 1,
	ORE = 2,
	FOOD = 3,
};
wchar_t getDisplayableTile(Tile _tile);


class World
{
public:
	World(int _width, int _height);
	~World();

	int getWidth() const;
	int getHeight() const;

	bool isInside(int _x, int _y) const;

	Tile getTile(int _x, int _y) const;
	void setTile(int _x, int _y, Tile _tile);

	void generate();
	void findSpawnpoint(std::unique_ptr<Player>& _player);

	int getTotalOreAmount() const;

	int countTiles(Tile _tile) const;

	void render(std::unique_ptr<Screen>& _screen) const;

private:
	int mWidth, mHeight;
	Tile* mMap;
	std::mt19937 mRand;
	int mTotalOres;
};

