// Description: MipMap example
// DocumentationOf: mipmap.h

#include "pastel_example.h"

#include "pastel/sys/string_algorithms.h"
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
		Array<real, 1> sound;

		sound.setExtent(10);
		
		MipMap<real, 1> mipMap(arrayView(sound));
		
		sound(4) = 2;
	}

	void testResample()
	{
		Array<Color, 2> texture;
		loadPcx("lena.pcx", texture);

		MipMap<Color, 2> mipMap(arrayView(texture));
		transform(mipMap, fitColor);

		Vector<integer, 2> position;

		Array<Color, 2> outputImage(texture.extent() * 2, Color(1));

		const integer images = mipMap.levels();
		for (integer i = 0;i < images;++i)
		{
			drawView(constArrayView(mipMap.view(i)),
				position, arrayView(outputImage));

			position += mipMap.view(i).extent();
		}

		savePcx(outputImage, "mipmap.pcx");
	}

	void test()
	{
		testResample();
	}

	void addTest()
	{
		testRunner().add("MipMap", test);
	}

	CallFunction run(addTest);

}
