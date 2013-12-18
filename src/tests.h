#pragma once

#include "vector.h"
#include "bmpFile.h"
#include "draw.h"
#include "generator.h"
#include "testParams.h"

namespace Jil
{
	typedef void (*drawFuncType)(Draw&, TestParams&);

	void drawCircle(Draw& draw, TestParams& params)
	{
		int x = params.randSizeX();
		int y = params.randSizeY();
		int r = params.randSize();
		float l = params.randWeight();
		float b = params.randBlur();

		Color c = Color::randomColor();
		c._a = params.alpha;
		draw.circle(x, y, r, c, l, b);	
	}

	void drawRect(Draw& draw, TestParams& params)
	{
		int x = params.randSizeX();
		int y = params.randSizeY();
		int w = params.randSize();
		int h = params.randSize();
		float l = params.randWeight();

		Color c = Color::randomColor();
		c._a = params.alpha;
		draw.rect(x, y, w, h, c, l);
	}

	void drawLine(Draw& draw, TestParams& params)
	{
		Vec2 A(params.randWeightVec());
		Vec2 B(params.randWeightVec());
		float l = fabs(params.randWeight());
		float b = params.randBlur();

		Color c = Color::randomColor();
		c._a = params.alpha;
		draw.line(A, B, c, l, b);
	}

	void drawPoly(Draw& draw, TestParams& params)
	{
		Vec2 center = params.randSizeVec();
		float ang = randFloat(0.0f, 2 * PI);
		int sides = randInt(3, 15);
		float size = params.randSize();
		Polygon p;
		if (params.flag)
		{
			p = Polygon::random(sides, center, size);
		}
		else
		{
			p = Polygon::regular(sides, center, size, ang);
		}

		Color c = Color::randomColor();
		c._a = params.alpha;
		draw.poly(p, c, params.randWeight() * 0);
	}

	void runTest(const char* filename, drawFuncType drawFuncs[], int nFuncs = 1)
	{
		TestParams params = TestParams::getRandom();
		Image img(params.width, params.height);
		Draw draw(&img);

		for (int i = 0; i < params.count; i++)
		{
			drawFuncType func = drawFuncs[randInt(0, nFuncs)];
			func(draw, params);
		}

		BmpFile file(filename);
		file.write(&img);
	}

	void circleTest(const char* filename)
	{
		drawFuncType funcs[1] = { drawCircle };
		runTest(filename, funcs);
	}

	void rectTest(const char* filename)
	{
		drawFuncType funcs[1] = { drawRect };
		runTest(filename, funcs);

	}

	void lineTest(const char* filename)
	{
		drawFuncType funcs[1] = { drawLine };
		runTest(filename, funcs);
	}

	void polyTest(const char* filename)
	{
		drawFuncType funcs[1] = { drawPoly };
		runTest(filename, funcs);
	}

	void mixedTest(const char* filename)
	{
		const int kNumFuncs = 4;
		drawFuncType funcs[kNumFuncs] = { drawCircle, drawRect, drawLine, drawPoly };
		runTest(filename, funcs, kNumFuncs);
	}

	void generateTest(const char* filename)
	{
		drawFuncType funcs[1] = { generate };
		runTest(filename, funcs);
	}
}