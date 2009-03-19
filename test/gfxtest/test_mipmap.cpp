#include "pastelgfxtest.h"

#include "pastel/sys/string_tools.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/drawing.h"

#include "pastel/dsp/mipmap_tools.h"

using namespace Pastel;

namespace
{

	void test1d()
	{
		Array<1, real> sound;

		sound.setExtent(10);
		
		MipMap<1, real> mipMap(arrayView(sound));
		
		sound(4) = 2;
	}

	void testResample()
	{
		Array<2, Color> texture;
		loadPcx("lena.pcx", texture);

		MipMap<2, Color> mipMap(arrayView(texture));
		transform(mipMap, fitColor);

		Point<2, integer> position;

		Array<2, Color> outputImage(texture.extent() * 2);

		const integer images = mipMap.levels();
		for (integer i = 0;i < images;++i)
		{
			drawView(constArrayView(mipMap.view(i)),
				position, arrayView(outputImage));

			position += mipMap.view(i).extent();
		}

		savePcx(outputImage, "output/mipmap.pcx");
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
