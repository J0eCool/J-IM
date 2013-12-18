#pragma once

#include "testParams.h"

namespace Jil
{
	class Generator
	{
	};

	void generate(Draw& draw, TestParams& params)
	{
		params.count = 1;

		int w = params.width;
		int h = params.height;
		int mid = w / 2;

		std::vector<Vec2> left;
		std::vector<Vec2> right;

		int nVerts = 5;
		for (int i = 0; i < nVerts; i++)
		{
			float r = randFloat(0.05f, 0.15f) * w;
			float y = h * lerp((float)i / nVerts, 0.2f, 0.75f);
			left.push_back(Vec2(mid - r, y));
			right.push_back(Vec2(mid + r, y));
		}

		Polygon shape;
		for (int i = 0; i < left.size(); i++)
		{
			shape.verts.push_back(left[i]);
		}
		shape.verts.push_back(Vec2(mid, 0.9f * h));
		for (int i = right.size() - 1; i >= 0; i--)
		{
			shape.verts.push_back(right[i]);
		}
		draw.poly(shape, Color(0xff));
	}
}