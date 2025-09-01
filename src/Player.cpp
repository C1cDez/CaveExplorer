#include "Player.h"

#include "World.h"

Player::Player(std::shared_ptr<World>& _world)
{
	mX = mY = 0;
	mXAcc = mYAcc = 0;
	mEnergy = 100;
	mMoney = 0;
	mWorld = _world;
}
bool Player::dead() const
{
	return mEnergy <= 0;
}
bool Player::won() const
{
	return mMoney >= mWorld->getTotalOreAmount();
}
int Player::getX() const
{
	return mX;
}
int Player::getY() const
{
	return mY;
}
void Player::setX(int _x)
{
	mX = _x;
}
void Player::setY(int _y)
{
	mY = _y;
}
void Player::move(int _dx, int _dy)
{
	mX += _dx;
	mY += _dy;
}
int Player::getMoney() const
{
	return mMoney;
}
void Player::stepX(int _dx)
{
	if (mXAcc > 2) { move(_dx, 0); mXAcc = 0; }
	else mXAcc++;
}
void Player::stepY(int _dy)
{
	if (mYAcc > 4) { move(0, _dy); mYAcc = 0; }
	else mYAcc++;
}
void Player::render(std::unique_ptr<Screen>& _screen)
{
	_screen->pixel(mX, mY, '^');

	_screen->write(0, 0, "Energy: ", 8);
	char energyNumBuffer[4 + 1];
	sprintf_s(energyNumBuffer, 5, "%4d", mEnergy);
	_screen->write(8, 0, energyNumBuffer, 4);

	_screen->write(0, 1, "Money:   ", 9);
	char moneyNumBuffer[3 + 1 + 3 + 1];
	sprintf_s(moneyNumBuffer, 8, "%3d/%3d", mMoney, mWorld->getTotalOreAmount());
	_screen->write(9, 1, moneyNumBuffer, 7);
}

void Player::update()
{
	if (GetAsyncKeyState('W') & 0x8000)
		stepY(-1);
	if (GetAsyncKeyState('S') & 0x8000)
		stepY(1);
	if (GetAsyncKeyState('D') & 0x8000)
		stepX(1);
	if (GetAsyncKeyState('A') & 0x8000)
		stepX(-1);

	if (mX < 0) mX = 0;
	if (mY < 0) mY = 0;
	if (mX >= mWorld->getWidth()) mX = mWorld->getWidth() - 1;
	if (mY >= mWorld->getHeight()) mY = mWorld->getHeight() - 1;

	Tile tile = mWorld->getTile(mX, mY);
	if (tile == Tile::STONE) mEnergy--;
	else if (tile == Tile::ORE) mMoney++;
	else if (tile == Tile::FOOD) mEnergy += 8;
	mWorld->setTile(mX, mY, Tile::AIR);
}
