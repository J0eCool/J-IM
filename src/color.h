#pragma once

#include "util.h"

#include <iostream>

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

		Color(char r, char g, char b, char a = kMax)
		{
			construct(r, g, b, a);
		}
		Color(char c)
		{
			construct(c, c, c, kMax);
		}
		Color()
		{
			construct(0, 0, 0, 0);
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

		static Color floatColor(float r, float g, float b, float a = 1.0f)
		{
			Color c;
			c.construct((int)kMax * r, (int)kMax * g, (int)kMax * b, (int)kMax * a);
			return c;
		}

		static Color HSV(float hue, float sat = 1.0f, float val = 1.0f)
		{
			hue = fmod(hue + 360.0f, 360.0f);

			Color color;
			float c = val * sat;
			float x = c * (1.0f - fabs(fmod(hue / 60.0f, 2.0f) - 1.0f));
			float m = val - c;

			if (hue < 60.0f) { color = floatColor(c, x, 0); }
			else if (hue < 120.0f) { color = floatColor(x, c, 0); }
			else if (hue < 180.0f) { color = floatColor(0, c, x); }
			else if (hue < 240.0f) { color = floatColor(0, x, c); }
			else if (hue < 300.0f) { color = floatColor(x, 0, c); }
			else { color = floatColor(c, 0, x); }

			color._r += m * kMax;
			color._g += m * kMax;
			color._b += m * kMax;

			return color;
		}
	};
}