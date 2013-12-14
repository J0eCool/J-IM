#pragma once

#include "util.h"

namespace Jil
{
	struct Color
	{
	private:
		void construct(char r, char g, char b, char a)
		{
			_r = r;
			_g = g;
			_b = b;
			_a = a;
		}

		static const int kMax = 0xff;

	public:
		unsigned char _b;
		unsigned char _g;
		unsigned char _r;
		unsigned char _a;

		Color(char r, char g, char b, char a)
		{
			construct(r, g, b, a);
		}
		Color(char r, char g, char b)
		{
			construct(r, g, b, kMax);
		}
		Color(char c)
		{
			construct(c, c, c, kMax);
		}
		Color()
		{
			construct(0, 0, 0, kMax);
		}

		Color blend(Color const& other) const
		{
			float amt = (float)other._a / kMax;
			float newA = min(_a + amt / 2.0f, 1.0f);
			int r = lerp(amt, _r, other._r);
			int g = lerp(amt, _g, other._g);
			int b = lerp(amt, _b, other._b);
			return Color(r, g, b, newA);
		}
	};
}