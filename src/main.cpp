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

void circleTest(const char* filename)
{
	int imgDim = (int)pow(1.5f, randInt(10, 20));
	float imgAR = randInt(6, 20) / 10.0f;

	float rMid = imgDim / randInt(6, 18);
	int a = randFloat() < 0.8f ? 0x10 * randInt(0x2, 0x10) : 0xff;
	int nCirc = 2 * imgDim * 0xff / ((a / 2 + 0x80) * sqrt(rMid) * randFloat(0.5f, 3.5f));

	Image img(imgDim, (int)(imgDim / imgAR));

	Draw draw(&img);
	for (int i = 0; i < nCirc; i++)
	{
		int x = randInt(-rMid, img._width + rMid);
		int y = randInt(-rMid, img._height + rMid);
		int r = randInt(rMid / 2, 2 * rMid);

		Color c = Color::randomColor();
		c._a = a;
		draw.circle(x, y, r, c);
	}

	BmpFile file(filename);
	file.write(&img);
}

void rectTest(const char* filename)
{
	int imgDim = (int)pow(1.5f, randInt(10, 20));
	float imgAR = randInt(6, 20) / 10.0f;

	float rMid = imgDim / randInt(6, 18);
	int a = randFloat() < 0.8f ? 0x10 * randInt(0x2, 0x10) : 0xff;
	int nCirc = 2 * imgDim * 0xff / ((a / 2 + 0x80) * sqrt(rMid) * randFloat(0.5f, 3.5f));

	Image img(imgDim, (int)(imgDim / imgAR));

	Draw draw(&img);
	for (int i = 0; i < nCirc; i++)
	{
		int x = randInt(-rMid, img._width + rMid);
		int y = randInt(-rMid, img._height + rMid);
		int w = randInt(rMid / 2, 2 * rMid);
		int h = randInt(rMid / 2, 2 * rMid);

		Color c = Color::randomColor();
		c._a = a;
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