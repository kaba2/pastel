#include "pastelgfxtest.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/pcx.h"
#include "pastel/gfx/colormixers.h"

#include "pastel/sys/view_all.h"

#include <iostream>
using namespace std;

using namespace Pastel;

namespace
{

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
				xMin + ((real)i / Width) * dx;

			const integer y = 
				yMin + (integer)std::floor(((x - xMin) / dx) * dy);

			const real yScreen = 
				((real)(y - yMin) / (yMax - yMin)) * Height;

			drawBox(AlignedBox2(i, 0, i + 1, yScreen), 
				Color(0), arrayView(image));
		}

		for (integer y = yMin;y <= yMax;++y)
		{
			const real x = 
				xMin + ((real)(y - yMin) / dy) * dx;

			const real xScreen = 
				x * Width;

			drawBox(AlignedBox2((xScreen + 0.5) - 1.5, 0, (xScreen + 0.5) + 1.5, Height), 
				Color(1, 0, 0), arrayView(image),
				transparentColorMixer<Color>(0.75));
		}

		savePcx(image, "testrange_usual.pcx");

		clear(Color(0.8), arrayView(image));

		for (integer i = 0;i < Width;++i)
		{
			const real x = 
				xMin + ((real)i / Width) * dx;

			const integer y = realToInteger(
				x,
				yMin, yMax,
				xMin, xMax);

			/*
			const integer y = 
				yMin + std::min((integer)std::floor(((x - xMin) / dx) * (dy + 1)), dy);
			*/

			const real yScreen = ((real)(y - yMin) / (yMax - yMin)) * Height;

			drawBox(AlignedBox2(i, 0, i + 1, yScreen), 
				Color(0), arrayView(image));
		}

		for (integer y = yMin;y <= yMax;++y)
		{
			const real x = 
				xMin + ((real)(y - yMin) / dy) * dx;

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
	}

	CallFunction run(testAdd);

}

