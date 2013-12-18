#include "image.h"
#include "vector.h"
#include "tests.h"

#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <dirent.h>

using namespace Jil;

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

	const int kNumTests = 6;
	TestData validTests[kNumTests] =
		{
			{"circle", circleTest, 25},
			{"rect", rectTest, 45},
			{"line", lineTest, 25},
			{"mixed", mixedTest, 50},
			{"poly", polyTest, 25},
			{"generate", generateTest, 25},
		};

	std::string testType = (argc > 1 ? argv[1] : "mixed");

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