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

	class TestObject
	{
	public:
		virtual void operator()(Draw& draw, TestParams& params) const = 0;
	};

	class SingleTest : public TestObject
	{
	private:
		drawFuncType _drawFunc;

	public:
		SingleTest(drawFuncType func) : _drawFunc(func) {}

		void operator()(Draw& draw, TestParams& params) const
		{
			_drawFunc(draw, params);
		}
	};

	class BatchTest : public TestObject
	{
	private:
		drawFuncType* _drawFuncs;
		int _nFuncs;

	public:
		BatchTest(drawFuncType* funcs, int count = 1) : _drawFuncs(funcs), _nFuncs(count) {}

		void operator()(Draw& draw, TestParams& params) const
		{
			for (int i = 0; i < params.count; i++)
			{
				drawFuncType func = _drawFuncs[randInt(0, _nFuncs)];
				func(draw, params);
			}
		}
	};

	template <typename F>
	void runTest(const char* filename, F test)
	{
		TestParams params = TestParams::getRandom();
		Image img(params.width, params.height);
		Draw draw(&img);

		test(draw, params);

		BmpFile file(filename);
		file.write(&img);
	}

	void circleTest(const char* filename)
	{
		drawFuncType funcs[1] = { drawCircle };
		runTest(filename, BatchTest(funcs));
	}

	void rectTest(const char* filename)
	{
		drawFuncType funcs[1] = { drawRect };
		runTest(filename, BatchTest(funcs));

	}

	void lineTest(const char* filename)
	{
		drawFuncType funcs[1] = { drawLine };
		runTest(filename, BatchTest(funcs));
	}

	void polyTest(const char* filename)
	{
		drawFuncType funcs[1] = { drawPoly };
		runTest(filename, BatchTest(funcs));
	}

	void mixedTest(const char* filename)
	{
		const int kNumFuncs = 4;
		drawFuncType funcs[kNumFuncs] = { drawCircle, drawRect, drawLine, drawPoly };
		runTest(filename, BatchTest(funcs, kNumFuncs));
	}

	void generateTest(const char* filename)
	{
		runTest(filename, SingleTest(generate));
	}

	class BatchBlit : public TestObject
	{
	private:
		Image _img;

		static const int kImgSize = 32;

	public:
		BatchBlit() : _img(kImgSize, kImgSize)
		{
			const int kNumFuncs = 4;
			drawFuncType funcs[kNumFuncs] = { drawCircle, drawRect, drawLine, drawPoly };
			BatchTest mixBatch = BatchTest(funcs, kNumFuncs);
			
			Draw draw(&_img);
			TestParams params = TestParams::getRandom(kImgSize, kImgSize);
			for (int i = 0; i < 12; i++)
			{
				mixBatch(draw, params);
			}
		}

		void operator()(Draw& draw, TestParams& params) const
		{
			for (int i = 0; i < params.count; i++)
			{
				int x = randInt(-kImgSize, draw.getImage()->_width + kImgSize);
				int y = randInt(-kImgSize, draw.getImage()->_height + kImgSize);

				// int x = randInt(30, draw.getImage()->_width - 30);
				// int y = randInt(30, draw.getImage()->_height - 30);
				draw.blit(_img, x, y);
			}
		}
	};

	void blitTest(const char* filename)
	{
		runTest(filename, BatchBlit());
	}
}