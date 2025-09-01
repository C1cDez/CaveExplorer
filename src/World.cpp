#include "World.h"

#include <random>

#include "Player.h"

wchar_t getDisplayableTile(Tile _tile)
{
	switch (_tile)
	{
	case Tile::AIR: return ' ';
	case Tile::STONE: return '#';
	case Tile::ORE: return 'O';
	case Tile::FOOD: return '%';
	default: return '?';
	}
}


World::World(int _width, int _height) : mWidth{ _width }, mHeight{ _height }
{
	mMap = (Tile*)malloc(mWidth * mHeight * sizeof(Tile));
	mRand = std::mt19937{ static_cast<std::mt19937::result_type>(time(NULL)) };
	mTotalOres = 0;
}
World::~World()
{
	free(mMap);
}
int World::getWidth() const
{
	return mWidth;
}
int World::getHeight() const
{
	return mHeight;
}
bool World::isInside(int _x, int _y) const
{
	return (_x >= 0 && _x < mWidth) && (_y >= 0 && _y < mHeight);
}
Tile World::getTile(int _x, int _y) const
{
	return isInside(_x, _y) ? mMap[_y * mWidth + _x] : Tile::AIR;
}
void World::setTile(int _x, int _y, Tile _tile)
{
	if (isInside(_x, _y)) mMap[_y * mWidth + _x] = _tile;
}
int World::getTotalOreAmount() const
{
	return mTotalOres;
}
int World::countTiles(Tile _tile) const
{
	int amount = 0;
	for (int i = 0; i < mWidth * mHeight; i++)
		if (mMap[i] == _tile) amount++;
	return amount;
}

#define RANDOM_UNIFORM(min_, max_, eng) std::uniform_int_distribution<int>{ min_, max_ }(eng)

constexpr int MIN_ORE_NESTS = 40, MAX_ORE_NESTS = 100;
constexpr int MIN_FOOD_NESTS = 1, MAX_FOOD_NESTS = 4;
constexpr int MIN_FOOD_RADIUS = 1, MAX_FOOD_RADIUS = 3;
constexpr int MIN_CAVE_NESTS = 6, MAX_CAVE_NESTS = 26;
constexpr int MIN_CAVE_STEPS = 500, MAX_CAVE_STEPS = 1000;
constexpr int SPREADING_COFF = 20;

void World::generate()
{
	for (int y = 0; y < mHeight; y++)
		for (int x = 0; x < mWidth; x++)
			setTile(x, y, Tile::STONE);

	std::uniform_int_distribution<int> positionDist{ 0, mWidth * mHeight };

	std::uniform_int_distribution<int> oreFormDist{ 0, 1 << 8 };
	for (int i = 0; i < RANDOM_UNIFORM(MIN_ORE_NESTS, MAX_ORE_NESTS, mRand); i++)
	{
		int pi = positionDist(mRand);
		int ox = pi % mWidth, oy = pi / mWidth;
		int form = oreFormDist(mRand);
		setTile(ox, oy, Tile::ORE);
		if (form & 0b00000001) setTile(ox - 1, oy - 1, Tile::ORE);
		if (form & 0b00000010) setTile(ox,     oy - 1, Tile::ORE);
		if (form & 0b00000100) setTile(ox + 1, oy - 1, Tile::ORE);
		if (form & 0b00001000) setTile(ox - 1, oy,     Tile::ORE);
		if (form & 0b00010000) setTile(ox + 1, oy,     Tile::ORE);
		if (form & 0b00100000) setTile(ox - 1, oy + 1, Tile::ORE);
		if (form & 0b01000000) setTile(ox,     oy + 1, Tile::ORE);
		if (form & 0b10000000) setTile(ox + 1, oy + 1, Tile::ORE);
	}

	std::uniform_int_distribution<int> dirDist{ 0, 2 * SPREADING_COFF };
	for (int i = 0; i < RANDOM_UNIFORM(MIN_CAVE_NESTS, MAX_CAVE_NESTS, mRand); i++)
	{
		int pi = positionDist(mRand);
		int sx = pi % mWidth, sy = pi / mWidth;
		for (int s = 0; s < RANDOM_UNIFORM(MIN_CAVE_STEPS, MAX_CAVE_STEPS, mRand); s++)
		{
			setTile(sx, sy, Tile::AIR);
			int dir = dirDist(mRand);
			if (dir % SPREADING_COFF)
				sx += (dir < SPREADING_COFF) ? 1 : -1;
			else
				sy += dir ? 1 : -1;
		}
	}

	std::uniform_int_distribution<int> foodRadDist{ MIN_FOOD_RADIUS, MAX_FOOD_RADIUS };
	for (int i = 0; i < RANDOM_UNIFORM(MIN_FOOD_NESTS, MAX_FOOD_NESTS, mRand); i++)
	{
		int pi = positionDist(mRand);
		int sx = pi % mWidth, sy = pi / mWidth;
		int rad = foodRadDist(mRand);
		for (int dy = -rad; dy <= rad; dy++)
		{
			for (int dx = -rad; dx <= rad; dx++)
			{
				if (dx * dx + dy * dy <= rad * rad) setTile(sx + dx, sy + dy, Tile::FOOD);
			}
		}
	}

	//Info plate
	for (int x = 0; x < 17; x++) setTile(x, 0, Tile::AIR);
	for (int x = 0; x < 17; x++) setTile(x, 1, Tile::AIR);

	mTotalOres = countTiles(Tile::ORE);
}

void World::findSpawnpoint(std::unique_ptr<Player>& _player)
{
	int x = 0, y = 0;
	std::uniform_int_distribution<int> posDist{ 0, mWidth * mHeight };
	do
	{
		int pi = posDist(mRand);
		x = pi % mWidth;
		y = pi / mWidth;
	} while (getTile(x, y) != Tile::AIR);

	_player->setX(x);
	_player->setY(y);
}

void World::render(std::unique_ptr<Screen>& _screen) const
{
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			Tile tile = getTile(x, y);
			wchar_t ch = getDisplayableTile(tile);
			_screen->pixel(x, y, ch);
		}
	}
}
