#include "pastelgfxtest.h"

#include "pastel/sys/string_tools.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/drawing.h"

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

		Array<2, Color> texture;
		loadPcx("lena.pcx", texture);

		RipMap<2, Color> ripMap(arrayView(texture));

		transform(ripMap, fitColor);

		Point<2, integer> position;

		Array<2, Color> outputImage(texture.extent() * 2);

		integer yPosition = 0;

		const Vector<2, integer> textures = ripMap.levels();
		for (integer y = 0;y < textures.y();++y)
		{
			Point<2, integer> position(0, yPosition);

			for (integer x = 0;x < textures.x();++x)
			{
				Array<2, Color>& image = ripMap(Point<2, integer>(x, y));

				drawView(constArrayView(image),
					position, arrayView(outputImage));

				position.x() += image.extent().x();
			}

			yPosition += ripMap(Point<2, integer>(0, y)).extent().y();
		}

		savePcx(outputImage, "output/ripmap.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("RipMap", testRipMap);
	}

	CallFunction run(testAdd);

}
