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

	static TestParams getRandom()
	{
		TestParams params;

		float imgAR = randBool(0.5f) ? randInt(6, 20) / 10.0f : 1;
		params.width = (int)pow(2.0f, randInt(5, 12));
		params.height = (int)(params.width / imgAR);

		params.midSize = params.width / randInt(6, 18);
		params.alpha = randFloat() < 0.8f ? 0x10 * randInt(0x2, 0x10) : 0xff;
		params.count = 2 * params.width * 0xff
			/ ((params.alpha / 2 + 0x80) * sqrt(params.midSize) * randFloat(0.5f, 3.5f));
		params.midWeight = randInt(-8, 9);

		return params;
	}
};


void circleTest(const char* filename)
{
	TestParams params = TestParams::getRandom();
	Image img(params.width, params.height);
	Draw draw(&img);

	for (int i = 0; i < params.count; i++)
	{
		int x = randInt(-params.midSize, img._width + params.midSize);
		int y = randInt(-params.midSize, img._height + params.midSize);
		int r = randInt(params.midSize / 2, 2 * params.midSize);
		float l = randFloat(params.midWeight / 2, 2 * params.midWeight);

		Color c = Color::randomColor();
		c._a = params.alpha;
		draw.circle(x, y, r, c, l);
	}

	BmpFile file(filename);
	file.write(&img);
}

void rectTest(const char* filename)
{
	TestParams params = TestParams::getRandom();
	Image img(params.width, params.height);
	Draw draw(&img);

	for (int i = 0; i < params.count; i++)
	{
		int x = randInt(-params.midSize, img._width + params.midSize);
		int y = randInt(-params.midSize, img._height + params.midSize);
		int w = randInt(params.midSize / 2, 2 * params.midSize);
		int h = randInt(params.midSize / 2, 2 * params.midSize);

		Color c = Color::randomColor();
		c._a = params.alpha;
		draw.rect(x, y, w, h, c);
	}

	BmpFile file(filename);
	file.write(&img);
}

void lineTest(const char* filename)
{
	Image img(500, 500);

	Draw draw(&img);

	for (int i = 0; i < 150; i++)
	{
		Vec2 A(randFloat(50, 450), randFloat(50, 450));
		Vec2 B(randFloat(50, 450), randFloat(50, 450));
		Color c = Color::randomColor();
		float r = randFloat(1.0f, 5.0f);
		draw.line(A, B, c, r);
	}

	BmpFile file(filename);
	file.write(&img);
}

struct Test
{
	const char* name;
	void (*function)(const char*);
	int defaultCount;
};

int main(int argc, char** argv)
{
	clock_t startClock = clock();
	srand(time(0));

	const int kNumTests = 3;
	Test validTests[kNumTests] =
		{
			{"circle", circleTest, 25},
			{"rect", rectTest, 45},
			{"line", lineTest, 25},
		};

	std::string testType = (argc > 1 ? argv[1] : "line");

	Test* test = 0;
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