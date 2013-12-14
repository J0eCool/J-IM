#pragma once

#include <stdlib.h>

namespace Jil
{
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
		//return randFloat() * (hi - lo) + lo;
		return lerp(randFloat(), lo, hi);
	}

	int randInt(int lo, int hi)
	{
		if (hi <= lo)
		{
			return lo;
		}
		return rand() % (hi - lo) + lo;
	}
}