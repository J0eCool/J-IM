#pragma once

#include "vector.h"

#include <vector>

namespace Jil
{
	class Polygon
	{
	public:
		std::vector<Vec2> verts;

		static Polygon regular(int sides, Vec2 const& center, float radius, float baseAngle = 0.0f)
		{
			Polygon shape;
			for (int i = 0; i < sides; i++)
			{
				float ang = 2 * PI * i / sides + baseAngle;
				Vec2 r = Vec2::fromAngle(ang, radius);
				shape.verts.push_back(center + r);
			}
			return shape;
		}
	};
}