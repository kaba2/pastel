#include "pastelgfxtest.h"

#include "pastel/sys/string_tools.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/mipmap_tools.h"

using namespace Pastel;

namespace
{

	void testResample()
	{
		LinearArray<2, Color> texture;
		loadPcx("lena.pcx", texture);

		MipMap<2, Color> mipMap(arrayView(texture));
		transform(mipMap, fitColor);

		Point<2, integer> position;

		LinearArray<2, Color> outputImage(texture.extent() * 2);

		const integer images = mipMap.levels();
		for (integer i = 0;i < images;++i)
		{
			LinearArray<2, Color>& image = mipMap(i);

			drawView(constArrayView(image),
				position, arrayView(outputImage));

			position += image.extent();
		}

		savePcx(outputImage, "test_mipmap.pcx");
	}

	void testBegin()
	{
		testResample();
	}

	void testAdd()
	{
		gfxTestList().add("MipMap", testBegin);
	}

	CallFunction run(testAdd);

}
