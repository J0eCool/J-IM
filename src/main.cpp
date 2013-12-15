#include "image.h"
#include "draw.h"
#include "bmpFile.h"

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

		Color c = Color::HSV(randFloat(0.0f, 360.0f), randFloat());
		c._a = a;
		draw.circle(x, y, r, c);
	}

	BmpFile file(filename);
	file.write(&img);
}

int main(int argc, char** argv)
{
	clock_t startClock = clock();
	srand(time(0));

	const char* testType = (argc > 1 ? argv[1] : "circle");
	int nFiles = (argc > 2 ? atoi(argv[2]) : 35);

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
		circleTest(filename);
		std::cout << " Done" << std::endl;
	}

	double seconds = (float)(clock() - startClock) / CLOCKS_PER_SEC;
	std::cout << "Finished in " << seconds << "s" << std::endl;

	return 0;
}