#include "pastelgfxtest.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/pcx.h"
#include "pastel/gfx/colormixers.h"
#include "pastel/math/lowdiscrepancy.h"

#include "pastel/sys/view_all.h"

#include <iostream>
using namespace std;

using namespace Pastel;

namespace
{

	void testRandom()
	{
		const integer Width = 500;
		const integer Height = 500;
		Array<2, Color> image(Width, Height);

		const integer BlocksX = 10;
		const integer BlocksY = 10;

		const real blockWidth = (real)Width / BlocksX;
		const real blockHeight = (real)Height / BlocksY;

		for (integer x = 0;x < BlocksX;++x)
		{
			for (integer y = 0;y < BlocksY;++y)
			{
				const real value = randomReal();
				const integer quantizedValue = 
					quantize(value, 0, 1, 0, 3);
				drawBox(AlignedBox2(blockWidth * x, blockWidth * y, 
					blockWidth * (x + 1), blockHeight * (y + 1)),
					Color(0, 0, dequantize(quantizedValue, 0, 3, 0, 1)), arrayView(image));
			}
		}

		savePcx(image, "test_range_random_uniform.pcx");

		for (integer x = 0;x < BlocksX;++x)
		{
			for (integer y = 0;y < BlocksY;++y)
			{
				const real value = randomReal();
				const integer quantizedValue = std::floor(value * 3 + 0.5);
				drawBox(AlignedBox2(blockWidth * x, blockWidth * y, 
					blockWidth * (x + 1), blockHeight * (y + 1)),
					Color(0, 0, dequantize(quantizedValue, 0, 3, 0, 1)), arrayView(image));
			}
		}

		savePcx(image, "test_range_random_round.pcx");
	}

	void testBegin()
	{
		const integer Width = 500;
		const integer Height = 100;
		Array<2, Color> image(Width, Height);

		const real xMin = 0;
		const real xMax = 1;
		const integer yMin = 0;
		const integer yMax = 7;

		const real dx = xMax - xMin;
		const integer dy = yMax - yMin;

		clear(Color(0.8), arrayView(image));

		for (integer i = 0;i < Width;++i)
		{
			const real x = 
				dequantize(
				i, 0, Width - 1,
				xMin, xMax);

			const integer y = 
				quantize(
				x, xMin, xMax,
				yMin, yMax);

			const real yScreen =
				dequantize(
				y, yMin, yMax, 
				0, Height);

			drawBox(AlignedBox2(i, 0, i + 1, yScreen), 
				Color(0), arrayView(image));
		}

		for (integer y = yMin;y <= yMax;++y)
		{
			const real x = ditheredQuantize(
				y, yMin, yMax,
				0, Width);

			drawBox(AlignedBox2((x + 0.5) - 1.5, 0, (x + 0.5) + 1.5, Height), 
				Color(1, 0, 0), arrayView(image),
				transparentColorMixer<Color>(0.75));
		}

		savePcx(image, "testrange_usual.pcx");

		clear(Color(0.8), arrayView(image));

		for (integer i = 0;i < Width;++i)
		{
			const real x = 
				xMin + ((real)i / Width) * dx;

			const integer y = quantize(
				x, xMin, xMax,
				yMin, yMax);

			const real yScreen = ((real)(y - yMin) / (yMax - yMin)) * Height;

			drawBox(AlignedBox2(i, 0, i + 1, yScreen), 
				Color(0), arrayView(image));
		}

		for (integer y = yMin;y <= yMax;++y)
		{
			const real x = 
				xMin + ((real)(y - yMin) / (dy + 1)) * dx;

			const real xScreen = 
				x * Width;

			drawBox(AlignedBox2((xScreen + 0.5) - 1.5, 0, (xScreen + 0.5) + 1.5, Height), 
				Color(1, 0, 0), arrayView(image),
				transparentColorMixer<Color>(0.75));
		}

		savePcx(image, "testrange_correct.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("Range", testBegin);
		gfxTestList().add("Range.Random", testRandom);
	}

	CallFunction run(testAdd);

}

