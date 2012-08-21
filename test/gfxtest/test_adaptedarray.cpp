// Description: Testing for adapted array

#include "pastelgfxtest.h"

#include "pastel/sys/array.h"
#include "pastel/sys/views.h"
#include "pastel/sys/indexextenders.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/dsp/resample.h"
#include "pastel/gfx/image_tools.h"
#include "pastel/dsp/filters.h"

#include "pastel/gfx/coloradapter.h"

using namespace Pastel;

namespace
{

	void test()
	{
		Array<uint32, 2> image;

		loadPcx("lena.pcx", image);

		Array<uint32, 2> smallerImage(Vector2i(200, 200));

		savePcx(image, "output/adaptedview1.pcx");

		resample<Color>(
			constRgb888View(image), clampExtender(), lanczosFilter(2),
			rgb888View(smallerImage));

		savePcx(smallerImage, "output/adaptedview2.pcx");

		Array<uint32, 2> copySmallerImage(smallerImage.extent());

		copy(constArrayView(smallerImage),
			arrayView(copySmallerImage));

		savePcx(copySmallerImage, "output/adaptedview3.pcx");
	}

	void addTest()
	{
		testRunner().add("AdaptedView", test);
	}

	CallFunction run(addTest);

}
