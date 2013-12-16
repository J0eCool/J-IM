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

		void rect(int x, int y, int w, int h, Color color, float lineWeight = 0.0f)
		{
			if (lineWeight > 0.0f)
			{
				Vec2 a(x, y);
				Vec2 b(x + w, y);
				Vec2 c(x + w, y + h);
				Vec2 d(x, y + h);

				line(a, b, color, lineWeight);
				line(b, c, color, lineWeight);
				line(c, d, color, lineWeight);
				line(d, a, color, lineWeight);
			}
			else
			{
				int xLo = max(x, 0);
				int xHi = min(x + w, _img->_width);
				int yLo = max(y, 0);
				int yHi = min(y + h, _img->_height);
				for (int i = xLo; i < xHi; ++i)
				{
					for (int j = yLo; j < yHi; ++j)
					{
						_img->blendPixel(i, j, color);
					}
				}
			}
		}

		void circle(int x, int y, int r, Color color, float lineWeight = 0.0f, float blur = 1.0f)
		{
			lineWeight = max(lineWeight, 0.0f);
			blur = max(blur, 0.0f);
			float rad2 = lineWeight * lineWeight / 4;
			float tot = r + lineWeight / 2 + blur + 1;
			float r2 = tot * tot;
			int xLo = max(x - (int)tot, 0);
			int xHi = min(x + (int)tot, _img->_width);
			int yLo = max(y - (int)tot, 0);
			int yHi = min(y + (int)tot, _img->_height);
			for (int i = xLo; i < xHi; ++i)
			{
				for (int j = yLo; j < yHi; ++j)
				{
					float d;
					bool draw = false;
					if (lineWeight <= 0.0f)
					{
						float dist2 = distance2(x, y, i, j);
						if (dist2 <= r2)
						{
							d = sqrt(dist2) - r;
							draw = true;
						}
					}
					else
					{
						float dist = fabs(distance(x, y, i, j) - r);
						if (dist <= lineWeight / 2 + blur)
						{
							d = dist - lineWeight / 2;
							draw = true;
						}
					}

					if (draw)
					{
						Color blend = color;
						blend._a = lerp(d / blur, (int)color._a, 0);
						_img->blendPixel(i, j, blend);
					}
				}
			}
		}

		void line(Vec2 const& start, Vec2 const& end, Color color, float size = 2.0f, float blur = 1.0f)
		{
			float rad = size / 2 + blur + 1;
			int xLo = max((int)(min(start.x, end.x) - rad), 0);
			int xHi = min((int)(max(start.x, end.x) + rad), _img->_width);
			int yLo = max((int)(min(start.y, end.y) - rad), 0);
			int yHi = min((int)(max(start.y, end.y) + rad), _img->_height);

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
			for (int i = xStart; (xEnd - i) * xStep >= 0; i += xStep)
			{
				int drawStartY = -1;
				bool hasDrawn = false;
				for (int j = lastY; (yEnd - j) * yStep >= 0; j += yStep)
				{
					if (!_img->inBounds(i, j))
					{
						break;
					}

					Vec2 C(i, j);
					Vec2 D = lerp((C - start).dot(dir) / deltaDist, start, end);
					float dist = (C - D).length();

					if (dist <= rad)
					{
						Color c = color;
						c._a = lerp((dist - size / 2) / blur, (int)color._a, 0);
						_img->blendPixel(i, j, c);
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