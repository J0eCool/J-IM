#pragma once

#include <stdlib.h>
#include <math.h>

namespace Jil
{
	template <typename T>
	void swap(T& a, T& b)
	{
		T t = a;
		a = b;
		b = t;
	}

	template <typename T>
	T max(T const& a, T const& b)
	{
		if (a < b)
		{
			return b;
		}
		return a;
	}
	template <typename T>
	T max(T const& a, T const& b, T const& c)
	{
		return max(max(a, b), c);
	}

	template <typename T>
	T min(T const& a, T const& b)
	{
		if (b < a)
		{
			return b;
		}
		return a;
	}

	template <typename T>
	T clamp(T const& v, T const& lo, T const& hi)
	{
		return min(max(v, lo), hi);
	}

	template <typename T>
	T lerp(float t, T const& lo, T const& hi)
	{
		t = clamp(t, 0.0f, 1.0f);
		return (T)(t * (hi - lo) + lo);
	}

	float randFloat()
	{
		return (float)rand() / RAND_MAX;
	}
	float randFloat(float lo, float hi)
	{
		return lerp(randFloat(), lo, hi);
	}
	float randBool(float trueProb)
	{
		return randFloat() <= trueProb;
	}

	int randInt(int lo, int hi)
	{
		if (hi <= lo)
		{
			return lo;
		}
		return rand() % (hi - lo) + lo;
	}

	float distance2(float x1, float y1, float x2, float y2)
	{
		float dx = x2 - x1;
		float dy = y2 - y1;
		return dx * dx + dy * dy;
	}
	float distance(float x1, float y1, float x2, float y2)
	{
		return sqrt(distance2(x1, y1, x2, y2));
	}

	int sign(float n)
	{
		if (n < 0.0f)
		{
			return -1;
		}
		if (n > 0.0f)
		{
			return 1;
		}
		return 0;
	}

	int forceSign(float n)
	{
		int s = sign(n);
		if (s == 0)
		{
			s = 1;
		}
		return s;
	}
}