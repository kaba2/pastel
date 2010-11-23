// Description: Testing for RipMap
// DocumentationOf: ripmap.h

#include "pasteldsptest.h"

#include "pastel/sys/string_tools.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/draw.h"

#include "pastel/dsp/ripmap_tools.h"

using namespace Pastel;

namespace
{

	void testRipMap()
	{
		/*
		This test computes a ripmap from the
		input image. It then combines all the
		individual ripmaps to a single image.
		*/

		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		RipMap<Color, 2> ripMap(arrayView(texture));

		transform(ripMap, fitColor);

		Vector<integer, 2> position;

		Array<Color, 2> outputImage(texture.extent() * 2, Color(1));

		integer yPosition = 0;

		const Vector<integer, 2> textures = ripMap.levels();
		for (integer y = 0;y < textures.y();++y)
		{
			Vector<integer, 2> position(0, yPosition);

			for (integer x = 0;x < textures.x();++x)
			{
				Array<Color, 2>& image = ripMap(Vector<integer, 2>(x, y));

				drawView(constArrayView(image),
					position, arrayView(outputImage));

				position.x() += image.extent().x();
			}

			yPosition += ripMap(Vector<integer, 2>(0, y)).extent().y();
		}

		savePcx(outputImage, "output/ripmap.pcx");
	}

	void testAdd()
	{
		dspTestList().add("RipMap", testRipMap);
	}

	CallFunction run(testAdd);

}
