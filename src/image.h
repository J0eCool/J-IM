#pragma once

namespace Jil
{
	struct Color
	{
	public:
		char _b;
		char _g;
		char _r;

		Color(char r, char g, char b) :
			_r(r), _g(g), _b(b) {}
		Color(char c) :
			_r(c), _g(c), _b(c) {}
		Color() :
			_r(0), _g(0), _b(0) {}
	};

	class Image
	{
	public:
		Color* _pixels;
		short _width;
		short _height;

		Image(short width, short height) : _width(width), _height(height)
		{
			_pixels = new Color[_width * _height];
		}

		~Image()
		{
			delete _pixels;
		}
	};
}