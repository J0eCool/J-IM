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

		static Polygon random(int sides, Vec2 const& center, float radius)
		{
			Polygon shape;
			for (int i = 0; i < sides; i++)
			{
				Vec2 r = radius * Vec2(randFloat(-1, 1), randFloat(-1, 1));
				shape.verts.push_back(center + r);
			}
			return shape;
		}
	};

	struct PolygonSide
	{
		float xLo;
		float xHi;

		// y = mx + b;
		float m;
		float b;

		PolygonSide(Vec2 const& cur, Vec2 const& next)
		{
			Vec2 delta = next - cur;
			float dx = delta.x == 0.0f ? 0.000001f : delta.x;
			m = delta.y / dx;
			b = cur.y - m * cur.x;

			xLo = min(cur.x, next.x);
			xHi = max(cur.x, next.x);
		}
	};
}