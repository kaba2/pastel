// Description: Testing for PCX files
// DocumentationOf: loadpcx.h

#include "test/test_init.h"

#include "pastel/gfx/image_file/pcx.h"

#include "pastel/sys/view.h"

#include <iostream>
#include <string>

TEST_CASE("Pcx (Pcx)")
{
	Array<Color, 2> image;
	std::vector<Color> palette;

	loadPcx("testpcx_rgb.pcx", image);
	savePcx(image, "pcx_rgb_output_rgb.pcx");

	loadPcx("testpcx_8bit.pcx", image);
	savePcx(image, "pcx_8bit_output_rgb.pcx");

	loadPcx("testpcx_4bit.pcx", image);
	savePcx(image, "pcx_4bit_output_rgb.pcx");

	//loadPcx("pcx_2bit.pcx", image);
	//savePcx(image, "pcx_2bit_output_rgb.pcx");

	loadPcx("testpcx_1bit.pcx", image);
	savePcx(image, "pcx_1bit_output_rgb.pcx");

	Array<uint8, 2> indexedImage;

	loadIndexedPcx("testpcx_8bit.pcx", indexedImage, &palette);
	saveIndexedPcx(indexedImage, palette, "pcx_8bit_output_8bit.pcx");

	loadIndexedPcx("testpcx_4bit.pcx", indexedImage, &palette);
	saveIndexedPcx(indexedImage, palette, "pcx_4bit_output_8bit.pcx");

	//loadIndexedPcx("testpcx_2bit.pcx", indexedImage);
	//saveIndexedPcx(indexedImage, "pcx_2bit_output.pcx");

	loadIndexedPcx("testpcx_1bit.pcx", indexedImage, &palette);
	saveIndexedPcx(indexedImage, palette, "pcx_1bit_output_8bit.pcx");

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

	Array<bool, 2> binaryImage(
		Vector2i(indexedImage.width(), indexedImage.height()));

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

	saveBinaryPcx(binaryImage, "pcx_1bit_output_1bit.pcx");
}

