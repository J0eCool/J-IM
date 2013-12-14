#include "util.h"
#include "image.h"

namespace Jil
{
	class Draw
	{
	private:
		Image* _img;

	public:
		Draw(Image* img) : _img(img) {}

		void rect(int x, int y, int w, int h, Color c)
		{
			int xLo = max(x, 0);
			int xHi = min(x + w, _img->_width);
			int yLo = max(y, 0);
			int yHi = min(y + h, _img->_height);
			for (int i = xLo; i < xHi; ++i)
			{
				for (int j = yLo; j < yHi; ++j)
				{
					_img->setPixel(i, j, _img->getPixel(i, j).blend(c));
				}
			}
		}
	};
}