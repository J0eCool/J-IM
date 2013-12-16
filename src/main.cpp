#include "image.h"
#include "draw.h"
#include "bmpFile.h"
#include "vector.h"

#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <dirent.h>

using namespace Jil;

struct TestParams
{
	int width;
	int height;
	float midSize;
	int alpha;
	int count;
	float midWeight;
	float midBlur;

	static TestParams getRandom()
	{
		TestParams params;

		float imgAR = randBool(0.5f) ? randInt(6, 20) / 10.0f : 1;
		params.width = (int)pow(2.0f, randInt(6, 12));
		params.height = (int)(params.width / imgAR);

		params.midSize = params.width / randInt(6, 18);
		params.alpha = randBool(0.75f) ? 0x10 * randInt(0x4, 0x10) : 0xff;
		params.count = 2 * params.width * 0xff
			/ ((params.alpha / 2 + 0x80) * sqrt(params.midSize) * randFloat(0.5f, 3.5f));
		params.midWeight = randInt(-8, 9);
		params.midBlur = randBool(0.7f) ? 1.0f : randInt(2, 9);

		return params;
	}

	int randSizeX() const { return randInt(-midSize, width + midSize); }
	int randSizeY() const { return randInt(-midSize, height + midSize); }
	int randWeightX() const { return randInt(-midWeight, width + midWeight); }
	int randWeightY() const { return randInt(-midWeight, height + midWeight); }

	int randSize() const { return randInt(midSize / 2, 2 * midSize); }
	int randWeight() const { return randInt(midWeight / 2, 2 * midWeight); }
	int randBlur() const { return midBlur == 1.0f ? 1.0f :
		randInt(midBlur / 2, 2 * midBlur); }
};

typedef void (*drawFuncType)(Draw&, TestParams const&);

void drawCircle(Draw& draw, TestParams const& params)
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

void drawRect(Draw& draw, TestParams const& params)
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

void drawLine(Draw& draw, TestParams const& params)
{
	Vec2 A(params.randWeightX(),
		params.randWeightY());
	Vec2 B(params.randWeightX(),
		params.randWeightY());
	float l = fabs(params.randWeight());
	float b = params.randBlur();

	Color c = Color::randomColor();
	c._a = params.alpha;
	draw.line(A, B, c, l, b);
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

void mixedTest(const char* filename)
{
	const int kNumFuncs = 3;
	drawFuncType funcs[3] = { drawCircle, drawRect, drawLine };
	runTest(filename, funcs, kNumFuncs);
}

struct TestData
{
	const char* name;
	void (*function)(const char*);
	int defaultCount;
};

int main(int argc, char** argv)
{
	clock_t startClock = clock();
	srand(time(0));

	const int kNumTests = 4;
	TestData validTests[kNumTests] =
		{
			{"circle", circleTest, 25},
			{"rect", rectTest, 45},
			{"line", lineTest, 25},
			{"mixed", mixedTest, 50},
		};

	std::string testType = (argc > 1 ? argv[1] : "line");

	TestData* test = 0;
	for (int i = 0; i < kNumTests; i++)
	{
		if (testType == validTests[i].name)
		{
			test = validTests + i;
			break;
		}
	}

	if (test)
	{
		int nFiles = (argc > 2 ? atoi(argv[2]) : test->defaultCount);

		DIR* outDir = opendir("./out");
		if (!outDir)
		{
			mkdir("./out");
		}

		std::string outSubDirPath = "./out/";
		outSubDirPath += testType;
		DIR* outSubDir = opendir(outSubDirPath.c_str());
		if (!outSubDir)
		{
			mkdir(outSubDirPath.c_str());
		}

		for (int i = 0; i < nFiles; i++)
		{
			char filename[64];
			sprintf(filename, "%s/%d.bmp", outSubDirPath.c_str(), i);
			std::cout << "Writing file " << filename << "...";
			test->function(filename);
			std::cout << " Done" << std::endl;
		}

		double seconds = (float)(clock() - startClock) / CLOCKS_PER_SEC;
		std::cout << "Finished in " << seconds << "s" << std::endl;
	}
	else
	{
		std::cout << testType << " is not a valid operation!\nValid tests:\n";
		for (int i = 0; i < kNumTests; i++)
		{
			std::cout << "  * " << validTests[i].name << std::endl;
		}
	}

	return 0;
}