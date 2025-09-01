#include "Screen.h"

#include <cstring>
#include <Windows.h>

DWORD __trash;

Screen::Screen(int _width, int _height) : mWidth{ _width }, mHeight{ _height }
{
	mBuffer = (wchar_t*)malloc(mWidth * mHeight * sizeof(wchar_t));
	mConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}
Screen::~Screen()
{
	free(mBuffer);
	CloseHandle(mConsole);
}

int Screen::getWidth() const
{
	return mWidth;
}
int Screen::getHeight() const
{
	return mHeight;
}

void Screen::clear()
{
	memset(mBuffer, 0, mWidth * mHeight * sizeof(wchar_t));
}
void Screen::display() const
{
	WriteConsoleOutputCharacter(mConsole, mBuffer, mWidth * mHeight, { 0, 0 }, &__trash);
}
void Screen::pixel(int _x, int _y, wchar_t _c)
{
	mBuffer[_y * mWidth + _x] = _c;
}
void Screen::write(int _x, int _y, const char* _text, int _len)
{
	int oy = 0, ox = 0;
	for (int i = 0; i < _len; i++)
	{
		wchar_t c = static_cast<wchar_t>(_text[i]);
		if (c == '\n') { oy++; ox = 0; }
		else
		{
			pixel(_x + ox, _y + oy, c);
			ox++;
		}
	}
}
