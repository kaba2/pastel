#include "pastelgfxtest.h"

#include "pastel/sys/array.h"
#include "pastel/sys/view_all.h"
#include "pastel/sys/indexextender_all.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/dsp/resample.h"
#include "pastel/gfx/image_tools.h"
#include "pastel/dsp/filter_all.h"

#include "pastel/gfx/coloradapter.h"

using namespace Pastel;

namespace
{

	void testBegin()
	{
		Array<uint32, 2> image;

		loadPcx("lena.pcx", image);

		Array<uint32, 2> smallerImage(200, 200);

		savePcx(image, "output/adaptedview1.pcx");

		resample<Color>(constRgb888View(image), clampExtender(), lanczosFilter(2),
			rgb888View(smallerImage));

		savePcx(smallerImage, "output/adaptedview2.pcx");

		Array<uint32, 2> copySmallerImage(smallerImage.extent());

		copy(constArrayView(smallerImage),
			arrayView(copySmallerImage));

		savePcx(copySmallerImage, "output/adaptedview3.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("AdaptedView", testBegin);
	}

	CallFunction run(testAdd);

}
