// Description: Testing for adapted array

#include "test_pastelgfx.h"

#include "pastel/sys/array.h"
#include "pastel/sys/view.h"
#include "pastel/sys/extender/indexextenders.h"

#include "pastel/gfx/image_file/pcx.h"
#include "pastel/gfx/color/color_tools.h"
#include "pastel/gfx/resampling.h"
#include "pastel/gfx/image_tools.h"
#include "pastel/gfx/filter/filters.h"

#include "pastel/gfx/color/coloradapter.h"

TEST_CASE("AdaptedView (AdaptedView)")
{
	Array<uint32, 2> image;

	loadPcx("lena.pcx", image);

	Array<uint32, 2> smallerImage(Vector2i(200, 200));

	savePcx(image, "adaptedview1.pcx");

	resample<Color>(
		constRgb888View(image), clampExtender(), lanczosFilter(2),
		rgb888View(smallerImage));

	savePcx(smallerImage, "adaptedview2.pcx");

	Array<uint32, 2> copySmallerImage(smallerImage.extent());

	copy(constArrayView(smallerImage),
		arrayView(copySmallerImage));

	savePcx(copySmallerImage, "adaptedview3.pcx");
}
