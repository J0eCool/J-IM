#include "util.h"
#include "image.h"
#include "vector.h"

#include <iostream>

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

		void circle(int x, int y, int r, Color c, float lineWeight = 0.0f)
		{
			lineWeight = max(lineWeight, 0.0f);
			float rad2 = lineWeight * lineWeight / 4;
			float r2 = r * r;
			int xLo = max(x - r - (int)(lineWeight + 1), 0);
			int xHi = min(x + r + (int)(lineWeight + 1), _img->_width);
			int yLo = max(y - r - (int)(lineWeight + 1), 0);
			int yHi = min(y + r + (int)(lineWeight + 1), _img->_height);
			for (int i = xLo; i < xHi; ++i)
			{
				for (int j = yLo; j < yHi; ++j)
				{
					if (lineWeight <= 0.0f)
					{
						float dist2 = distance2(x, y, i, j);
						if (dist2 <= r2)
						{
							_img->setPixel(i, j, c);
						}
						else if (dist2 <= (r + 1) * (r + 1))
						{
							float dist = sqrt(dist2);
							Color blend = c;
							blend._a = (int)(c._a * (1.0f - (dist - r)));
							_img->setPixel(i, j, blend);
						}
					}
					else
					{
						float dist = distance(x, y, i, j);
						float d = fabs(dist - r);
						if (d <= lineWeight / 2)
						{
							Color blend = c;
							blend._a = lerp(d / lineWeight, (int)c._a, 0);
							//blend._a = 0xff;
							_img->setPixel(i, j, blend);
						}
					}
				}
			}
		}

		void line(Vec2 const& start, Vec2 const& end, Color color, float size = 2.0f)
		{
			float rad2 = size * size / 4;
			int xLo = max((int)(min(start.x, end.x) - size), 0);
			int xHi = min((int)(max(start.x, end.x) + size), _img->_width);
			int yLo = max((int)(min(start.y, end.y) - size), 0);
			int yHi = min((int)(max(start.y, end.y) + size), _img->_height);

			Vec2 delta = end - start;
			float deltaDist = delta.length();
			Vec2 dir = delta.unit();

			int xStep = forceSign(delta.x);
			int xStart = xStep != -1 ? xLo : xHi;
			int xEnd = xStep != -1 ? xHi : xLo;

			int yStep = forceSign(delta.y);
			int yStart = yStep != -1 ? yLo : yHi;
			int yEnd = yStep != -1 ? yHi : yLo;

			int lastY = yStart;
			int prevLastY = yStart;
			for (int i = xStart; i != xEnd; i += xStep)
			{
				int drawStartY = -1;
				bool hasDrawn = false;
				for (int j = lastY; j != yEnd; j += yStep)
				{
					Vec2 C(i, j);
					Vec2 D = lerp((C - start).dot(dir) / deltaDist, start, end);
					float dist2 = (C - D).length2();

					if (dist2 <= rad2)
					{
						Color c = color;
						c._a = lerp(dist2 / rad2, (int)color._a, 0);
						_img->setPixel(i, j, c);
						hasDrawn = true;

						if (drawStartY == -1)
						{
							if (!hasDrawn)
							{
								j -= yStep * (size * 2 + 1);
							}
							else
							{
								drawStartY = j;
							}
						}
					}
					else if (drawStartY != -1)
					{
						lastY = prevLastY;
						prevLastY = drawStartY;
						break;
					}
				}
			}
		}
	};
}