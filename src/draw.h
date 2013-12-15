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
					_img->setPixel(i, j, c);
				}
			}
		}

		void circle(int x, int y, int r, Color c)
		{
			int xLo = max(x - r - 1, 0);
			int xHi = min(x + r + 1, _img->_width);
			int yLo = max(y - r - 1, 0);
			int yHi = min(y + r + 1, _img->_height);
			for (int i = xLo; i < xHi; ++i)
			{
				for (int j = yLo; j < yHi; ++j)
				{
					float dist = distance(x, y, i, j);
					if (dist <= r)
					{
						_img->setPixel(i, j, c);
					}
					else if (dist <= r + 1)
					{
						Color blend = c;
						blend._a = (int)(c._a * (1.0f - (dist - r)));
						_img->setPixel(i, j, blend);
					}
				}
			}
		}
	};
}