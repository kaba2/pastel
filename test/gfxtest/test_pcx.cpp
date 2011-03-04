// Description: Testing for PCX files
// DocumentationOf: loadpcx.h

#include "pastelgfxtest.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

namespace
{

	void testPcx()
	{
		Array<Color, 2> image;
		std::vector<Color> palette;

		loadPcx("testpcx_rgb.pcx", image);
		savePcx(image, "output/pcx_rgb_output_rgb.pcx");

		loadPcx("testpcx_8bit.pcx", image);
		savePcx(image, "output/pcx_8bit_output_rgb.pcx");

		loadPcx("testpcx_4bit.pcx", image);
		savePcx(image, "output/pcx_4bit_output_rgb.pcx");

		//loadPcx("pcx_2bit.pcx", image);
		//savePcx(image, "output/pcx_2bit_output_rgb.pcx");

		loadPcx("testpcx_1bit.pcx", image);
		savePcx(image, "output/pcx_1bit_output_rgb.pcx");

		Array<uint8, 2> indexedImage;

		loadIndexedPcx("testpcx_8bit.pcx", indexedImage, &palette);
		saveIndexedPcx(indexedImage, palette, "output/pcx_8bit_output_8bit.pcx");

		loadIndexedPcx("testpcx_4bit.pcx", indexedImage, &palette);
		saveIndexedPcx(indexedImage, palette, "output/pcx_4bit_output_8bit.pcx");

		//loadIndexedPcx("testpcx_2bit.pcx", indexedImage);
		//saveIndexedPcx(indexedImage, "output/pcx_2bit_output.pcx");

		loadIndexedPcx("testpcx_1bit.pcx", indexedImage, &palette);
		saveIndexedPcx(indexedImage, palette, "output/pcx_1bit_output_8bit.pcx");

		/*
		for (integer y = indexedImage.height() - 1;y >= 0;--y)
		{
			for (integer x = 0;x < indexedImage.width();++x)
			{
				if (indexedImage(x, y))
				{
					cout << "1";
				}
				else
				{
					cout << "0";
				}
			}
			cout << endl;
		}
		*/

		Array<bool, 2> binaryImage(indexedImage.width(), indexedImage.height());

		//copy(constArrayView(indexedImage), arrayView(binaryImage));

		loadIndexedPcx("testpcx_1bit.pcx", binaryImage);

		/*
		for (integer y = binaryImage.height() - 1;y >= 0;--y)
		{
			for (integer x = 0;x < binaryImage.width();++x)
			{
				if (binaryImage(x, y))
				{
					cout << "1";
				}
				else
				{
					cout << "0";
				}
			}
			cout << endl;
		}
		*/

		saveBinaryPcx(binaryImage, "output/pcx_1bit_output_1bit.pcx");
	}

	void test()
	{
		testPcx();
	}

	void addTest()
	{
		gfxTestList().add("Pcx", test);
	}

	CallFunction run(addTest);

}
