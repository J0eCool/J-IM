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
		void blendPixel(int x, int y, Color const& c)
		{
			Color* pix = _pixels + (x + y * _width);
			*pix = pix->blend(c);
		}

		bool inBounds(int x, int y) const
		{
			return x >= 0 && x < _width && y >= 0 && y < _height;
		}
	};
}