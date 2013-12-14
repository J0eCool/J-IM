#pragma once

#include "color.h"

namespace Jil
{
	class Image
	{
	public:
		Color* _pixels;
		int _width;
		int _height;

		Image(int width, int height) : _width(width), _height(height)
		{
			_pixels = new Color[_width * _height];
		}

		~Image()
		{
			delete _pixels;
		}

		Color const& getPixel(int x, int y)
		{
			return _pixels[x + y * _width];
		}
		void setPixel(int x, int y, Color const& c)
		{
			_pixels[x + y * _width] = c;
		}
	};
}