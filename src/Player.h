#pragma once

#include <memory>

#include "Screen.h"

class World;

class Player
{
public:
	Player(std::shared_ptr<World>& _world);

	bool dead() const;
	bool won() const;

	int getX() const;
	int getY() const;
	void setX(int _x);
	void setY(int _y);

	int getMoney() const;

	void move(int _dx, int _dy);

	void stepX(int _dx);
	void stepY(int _dx);

	void update();
	void render(std::unique_ptr<Screen>& _screen);

private:
	int mX, mY;
	int mXAcc, mYAcc;
	int mMoney;
	int mEnergy;
	std::shared_ptr<World> mWorld;
};

