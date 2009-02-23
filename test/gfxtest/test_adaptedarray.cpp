#include "pastelgfxtest.h"

#include "pastel/sys/array.h"
#include "pastel/sys/syscommon.h"
#include "pastel/sys/view_all.h"
#include "pastel/sys/indexextender_all.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/resample.h"
#include "pastel/gfx/image_tools.h"
#include "pastel/gfx/filter_all.h"

#include "pastel/gfx/coloradapter.h"

using namespace Pastel;

namespace
{

	void testBegin()
	{
		Array<2, uint32> image;

		loadPcx("lena.pcx", image);

		Array<2, uint32> smallerImage(200, 200);

		savePcx(image, "test_adaptedview1.pcx");

		resample(constRgb888View(image), clampExtender(), lanczosFilter(2),
			rgb888View(smallerImage));

		savePcx(smallerImage, "test_adaptedview2.pcx");

		Array<2, uint32> copySmallerImage(smallerImage.extent());

		copy(constArrayView(smallerImage),
			arrayView(copySmallerImage));

		savePcx(copySmallerImage, "test_adaptedview3.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("AdaptedView", testBegin);
	}

	CallFunction run(testAdd);

}
