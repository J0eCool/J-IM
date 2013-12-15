#pragma once

#include "image.h"

#include <fstream>
#include <iostream>

namespace Jil
{
	class BmpFile
	{
	private:
		const char* _filename;

	public:
		BmpFile(const char* filename) : _filename(filename)
		{
			//_filename = filename;
		}

		void write(Image* img)
		{
			std::ofstream file;
			file.open(_filename, std::ios::binary);
			if (file.is_open())
			{
				short width = img->_width;
				short height = img->_height;

				int size = width * height;
				int imgStart = 0x1a;
				int dibHeadSize = 12;
				short colorPlanes = 1;
				short bitsPerPixel = 24;


				// BMP header
				file.write("BM", 2);
				file.write((char*)&size, 4);
				file.write("jimg", 4);
				file.write((char*)&imgStart, 4);

				// DIB header (BITMAPCOREHEADER)
				file.write((char*)&dibHeadSize, 4);
				file.write((char*)&width, 2);
				file.write((char*)&height, 2);
				file.write((char*)&colorPlanes, 2);
				file.write((char*)&bitsPerPixel, 2);

				// Pixel Data
				// int pad = 0;
				// for (int i = 0; i < size; ++i)
				// {
				// 	file.write((char*)&(img->_pixels[i]), 3);

				// 	pad += 3;
				// 	if ((i + i) % height == 0)
				// 	{
				// 		if (pad % 4 != 0)
				// 		{
				// 			file.write("\0\0\0\0", 4 - (pad % 4));
				// 		}
				// 		pad = 0;
				// 	}
				// }

				int rowSize = 4 * ((bitsPerPixel * width + 31) / 32);
				char* rowStr = new char[rowSize];
				for (int row = 0; row < height; ++row)
				{
					for (int i = 0; i < width; i++)
					{
						char* pix = (char*)&(img->_pixels[i + row * width]);
						for (int s = 0; s < 3; s++)
						{
							rowStr[3 * i + s] = pix[s];
						}
					}
					file.write(rowStr, rowSize);
				}
				delete rowStr;

				file.close();
			}
		}
	};
}