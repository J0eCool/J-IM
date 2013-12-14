#include "image.h"
#include "draw.h"
#include "bmpFile.h"

#include <stdlib.h>
#include <time.h>

using namespace Jil;

int main(int argc, char** argv)
{
	srand(time(0));

	Image img(100, 150);

	Draw draw(&img);
	for (int i = 0; i < 135; i++)
	{
		int x = randInt(-10, 90);
		int y = randInt(-10, 140);
		int w = randInt(10, 40);
		int h = randInt(10, 50);

		int r = randInt(0x80, 0xff);
		int g = randInt(0x80, 0xff);
		int b = randInt(0x80, 0xff);
		draw.rect(x, y, w, h, Color(r, g, b, 0x80));
	}

	BmpFile file("helloWorld.bmp");
	file.write(&img);
	return 0;
}