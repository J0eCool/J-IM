#include "image.h"
#include "bmpFile.h"

using namespace Jil;

int main(int argc, char** argv)
{
	Image img(70, 150);
	int w(img._width), h(img._height), siz(w * h);
	for (int i = 0; i < siz; i++)
	{
		int x = i % w;
		int y = i / w;
		img._pixels[i] = Color(x * 0xff / w, y * 0xff / h, 0xff);
	}

	Jil::BmpFile file("helloWorld.bmp");
	file.write(&img);
	return 0;
}