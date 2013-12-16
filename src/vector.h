#pragma once

#include "util.h"

namespace Jil
{
	class Vec2
	{
	public:
		float x;
		float y;

		Vec2() : x(0.0f), y(0.0f) {}
		Vec2(float _x, float _y) : x(_x), y(_y) {}

		float length() const
		{
			return distance(x, y, 0.0f, 0.0f);
		}
		float length2() const
		{
			return distance2(x, y, 0.0f, 0.0f);
		}

		Vec2 unit() const
		{
			float m = length();
			if (m == 0.0f)
			{
				return Vec2();
			}
			return Vec2(x / m, y / m);
		}

		Vec2 operator+(Vec2 const& other) const
		{
			return Vec2(x + other.x, y + other.y);
		}
		Vec2 operator-(Vec2 const& other) const
		{
			return Vec2(x - other.x, y - other.y);
		}
		Vec2 operator*(float scalar) const
		{
			return Vec2(x * scalar, y * scalar);
		}
		Vec2 operator/(float scalar) const
		{
			return Vec2(x / scalar, y / scalar);
		}

		float dot(Vec2 const& other) const
		{
			return x * other.x + y * other.y;
		}
	};

	Vec2 operator*(float scalar, Vec2 const& vec)
	{
		return vec * scalar;
	}

	class Point
	{
	public:
		int x;
		int y;

		Point() : x(0), y(0) {}
		Point(int _x, int _y) : x(_x), y(_y) {}
		Point(Vec2 const& vec) : x((int)vec.x), y((int)vec.y) {}
	};
}