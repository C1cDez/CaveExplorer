#pragma once

#include <Windows.h>

class Screen
{
public:
	Screen(int _width, int _height);
	~Screen();

	int getWidth() const;
	int getHeight() const;

	void clear();
	void display() const;

	void pixel(int _x, int _y, wchar_t _c);
	void write(int _x, int _y, const char* _text, int _len);

private:
	int mWidth, mHeight;
	wchar_t* mBuffer;
	HANDLE mConsole;
};
